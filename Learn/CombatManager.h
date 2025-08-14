#pragma once
#include "CountDownTimerManager.h"
#include <vector>
#include <random>
#include <unordered_map>

class CombatManager : public GameScript
{
public:
    static CombatManager& Instance() {
        return *instance;
    }

    CombatManager() {
        instance = this;  // 在构造时绑定单例
    }

    // 预制体
    GameObject* Car = nullptr;
    GameObject* Car2 = nullptr;
    GameObject* HealthPackPrefab = nullptr;

    // ==== 可配置参数 ====
    int maxHealthPacks = 5; // 场上最大血包数
    float minSpawnInterval = 2.0f; // 最短生成间隔
    float maxSpawnInterval = 5.0f; // 最长生成间隔
    CVector3 spawnPositionMin = CVector3(-5, 1, -5);
    CVector3 spawnPositionMax = CVector3(5, 1, 5);

    // Breakable wall respawn 配置
    float breakWallRespawnMin = 2.0f;
    float breakWallRespawnMax = 6.0f;

    // ==== 车辆回收 ====
    void RecycleCar(GameObject* car);
    void ReSpawnCar(GameObject* car);

    // 回收 BreakableWall（会在随机时间后尝试重生）
    void RecycleBreakWall(GameObject* wall);

    // ==== 血包回收 ====
    void RecycleHealthPack(GameObject* pack);

    // 主更新
    void Update() override;

    // Try schedule spawn
    void TryScheduleHealthPackSpawn();

    // Spawn health pack (uses overlap test)
    void SpawnHealthPack();

    // ---- Maze helper: 在给定外围墙区域内部（按原格子单位）生成迷宫，
    // 并在内部 grid == wall 的格子实例化 BreakableWallPrefab。
    // outerRows/outerCols：包括外围墙（和你原来循环的上限一致，例如 10x9）
    // spacing、yPos：与外墙一致
    // 该方法会把所有内部网格位置（无论是否产生墙）注册到管理列表，以便后续重生使用
    void GenerateAndPlaceMazeInsideArea_CellGrid(
        const CVector3& AreaPos,
        int outerRows,
        int outerCols,
        float spacing,
        float yPos,
        GameObject* BreakableWallPrefab,
        float targetPassageFraction = 0.35f // 可调整：通路占内区比例
    );

private:
    static CombatManager* instance;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    void operator=(const CombatManager&) = delete;

    std::vector<GameObject*> activeHealthPacks;
    bool isSpawningHealthPack = false;

    // ---- Breakable wall 管理 ----
    // 所有在 Generate 时记录的可用网格世界坐标（内部格子，仅内部，不含外围墙）
    std::vector<CVector3> breakWallSpawnPositions;
    // 与 breakWallSpawnPositions 对应，记录该索引是否目前被占用
    std::vector<bool> breakWallOccupied;
    // 从被实例化的 BreakableWall GameObject* 到 spawnPositions 索引
    std::unordered_map<GameObject*, int> breakWallIndexMap;
    // 最近一次用于生成迷宫的 BreakableWall prefab（用于后续重生克隆）
    GameObject* lastBreakablePrefab = nullptr;
};
