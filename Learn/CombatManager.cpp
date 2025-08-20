#include "stdafx.h"
#include "CombatManager.h"
#include "Math.h"
#include "GameObjectManager.h"
#include "PhysicsManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "LayerDef.h"
#include <limits>
#include <queue>
#include <stack>
#include <algorithm>

CombatManager* CombatManager::instance = nullptr;

void CombatManager::RecycleCar(GameObject* car)
{
    auto unit = car->GetComponent<Unit>();
    if (unit) unit->ReSet();

    PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
    if (rigidBody) rigidBody->SetPosition(CVector3(0, -1000, 0));
    if (car->name == "Car")
    {
        ++Car2Score;
    }
    else if (car->name == "Car2")
    {
        ++CarScore;
    }
    CountDownTimerManager::Instance().AddTimer(3.0f, [this, car]() {
        this->ReSpawnCar(car);
        Debug::Log("3秒后重生车辆");
        });
}

void CombatManager::ReSpawnCar(GameObject* car)
{
    PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
    if (car->name == "Car")
    {
        if (rigidBody) rigidBody->SetPosition(CVector3(-1.5, 0.5 + 2.5, 7.5));
    }
    else if (car->name == "Car2")
    {
        if (rigidBody) rigidBody->SetPosition(CVector3(3, 0.5f + 2.5f, 0));
    }
}

// ------------- 修改点：回收 BreakableWall（复用同一对象，延迟搬回合适格子） -------------
void CombatManager::RecycleBreakWall(GameObject* wall)
{
    if (!wall) return;

    // 查找该墙对象当前所占的索引（如果没有记录，就尝试按位置匹配）
    int oldIndex = -1;
    auto it = breakWallIndexMap.find(wall);
    if (it != breakWallIndexMap.end()) {
        oldIndex = it->second;
        // 标记为未占用，但保留 map 条目，稍后会更新为新的索引
        if (oldIndex >= 0 && oldIndex < (int)breakWallOccupied.size()) {
            breakWallOccupied[oldIndex] = false;
        }
    }
    else {
        // 通过位置查找一个近似匹配的网格索引（容错）
        CVector3 pos = wall->transform->position;
        for (size_t i = 0; i < breakWallSpawnPositions.size(); ++i) {
            if ((breakWallSpawnPositions[i] - pos).lenSquared() < 1e-4f) {
                oldIndex = (int)i;
                breakWallOccupied[i] = false;
                break;
            }
        }
        // 即使未找到索引，也允许后续重用该对象
    }

    // 把对象暂时移出场景（移动到地图外，使其在视觉/物理上不可见）
    if (auto rb = wall->GetComponent<RigidBody>()) {
        if (rb->rigidBodyPrimitive) rb->rigidBodyPrimitive->SetPosition(CVector3(0, -1000, 0));
    }
    wall->transform->SetPosition(CVector3(0, -1000, 0));
    wall->transform->UpdateRigidBodyTransform();
    wall->transform->UpdateColliderTransform();

    // 随机延迟后重用同一对象放回某个未占用格子
    float delay = Math::RandomFloat(breakWallRespawnMin, breakWallRespawnMax);

    // 我们使用 shared_ptr 记录重试次数和 wall 指针等以便 lambda 可修改
    auto retryCount = std::make_shared<int>(0);
    const int maxRetries = 3; // 每次定时器触发最多内部尝试次数
    std::shared_ptr<GameObject*> wallPtr = std::make_shared<GameObject*>(wall);
    CountDownTimerManager::Instance().AddTimer(delay, [this, wallPtr, retryCount, oldIndex, maxRetries]() {
        GameObject* recycledWall = *wallPtr;
        if (!recycledWall) return;

        // 决定 prefab halfExtents（用于 OverlapBox 检测）
        CVector3 halfExtents(0.5f, 0.5f, 0.5f);
        if (lastBreakablePrefab) {
            if (auto box = lastBreakablePrefab->GetComponent<BoxCollider>()) {
                halfExtents = box->mCollider->mHalfSize;
            }
            else if (auto sphere = lastBreakablePrefab->GetComponent<SphereCollider>()) {
                float r = sphere->mCollider->mRadius;
                halfExtents = CVector3(r, r, r);
            }
        }
        else {
            // 若没有记录 prefab，尝试从 recycledWall 本身读取 collider
            if (auto box = recycledWall->GetComponent<BoxCollider>()) {
                halfExtents = box->mCollider->mHalfSize;
            }
            else if (auto sphere = recycledWall->GetComponent<SphereCollider>()) {
                float r = sphere->mCollider->mRadius;
                halfExtents = CVector3(r, r, r);
            }
        }

        // 收集未占用位置的索引
        std::vector<int> candidates;
        for (size_t i = 0; i < breakWallSpawnPositions.size(); ++i) {
            if (!breakWallOccupied[i]) candidates.push_back((int)i);
        }

        if (candidates.empty()) {
            // 没有可用格子，稍后重试一次（延迟再安排一次）
            if (*retryCount < 2) {
                *retryCount += 1;
                float nextDelay = Math::RandomFloat(breakWallRespawnMin, breakWallRespawnMax);
                CountDownTimerManager::Instance().AddTimer(nextDelay, [this, wallPtr, retryCount, oldIndex, maxRetries]() {
                    // 递归调用：此处简单地复制上面的行为（再次尝试）
                    // 为避免代码重复我们直接再调一次 RecycleBreakWall 的逻辑：但这里我们实现简单重试
                    //（为代码简洁起见，直接再次调用 RecycleBreakWall；也可把尝试逻辑封装）
                    if (wallPtr && *wallPtr) {
                        // 重新调度一个小任务去尝试放置
                        RecycleBreakWall(*wallPtr);
                    }
                    });
            }
            return;
        }

        // 随机打乱 candidates
        std::shuffle(candidates.begin(), candidates.end(), std::mt19937((unsigned)std::random_device{}()));

        bool placed = false;
        // 尝试在候选点中放置（检测重叠）
        for (int idxCandidate : candidates) {
            CVector3 spawnPos = breakWallSpawnPositions[idxCandidate];

            // 使用全层掩码（或改为仅检查某些层），同时包含 trigger
            unsigned int layerMask = (uint32_t)PhysicsLit::Layer::Wall | (uint32_t)PhysicsLit::Layer::PLAYER | (uint32_t)PhysicsLit::Layer::Item;
            auto overlapped = PhysicsLit::PhysicsManager::Instance().OverlapBox(spawnPos, halfExtents, CQuaternion(), layerMask, true);

            if (overlapped.empty()) {
                // 找到了合适位置，直接复用 recycledWall（不克隆）
                recycledWall->transform->SetPosition(spawnPos);
                recycledWall->transform->UpdateRigidBodyTransform();
                recycledWall->transform->UpdateColliderTransform();

                // 如刚体存在，也同步设置其内部位置（有些实现上 UpdateRigidBodyTransform 已经完成）
                if (auto rb = recycledWall->GetComponent<RigidBody>()) {
                    if (rb->rigidBodyPrimitive) rb->rigidBodyPrimitive->SetPosition(spawnPos);
                }

                // 更新占用标志与映射
                breakWallOccupied[idxCandidate] = true;
                breakWallIndexMap[recycledWall] = idxCandidate;

                // 如果之前旧索引存在且不同于新索引，清除旧索引的映射（防止残留）
                if (oldIndex >= 0 && oldIndex != idxCandidate && oldIndex < (int)breakWallOccupied.size()) {
                    // 旧索引已在回收时标为 false，这里不需要额外处理，但我们要移除任何可能指向旧索引的 map 条目
                    // 遍历 map 寻找任何指向 oldIndex 的条目并移除（通常旧条目就是 recycledWall 自己）
                    for (auto it = breakWallIndexMap.begin(); it != breakWallIndexMap.end(); ) {
                        if (it->second == oldIndex && it->first != recycledWall) {
                            // 如果有其他对象也指向该旧索引，保持不动（正常情况下不会发生）
                            ++it;
                        }
                        else ++it;
                    }
                }

                placed = true;
                break;
            }
            // 否则继续尝试下一个候选位置
        }

        if (!placed) {
            // 没有找到合适位置：尝试有限重试（避免无限循环）
            if (*retryCount < maxRetries) {
                *retryCount += 1;
                float nextDelay = Math::RandomFloat(CombatManager::Instance().breakWallRespawnMin, CombatManager::Instance().breakWallRespawnMax);
                CountDownTimerManager::Instance().AddTimer(nextDelay, [this, wallPtr, retryCount, oldIndex, maxRetries]() {
                    if (wallPtr && *wallPtr) {
                        RecycleBreakWall(*wallPtr); // 重新走回收路线来再调度一次（也会再次尝试重生）
                    }
                    });
            }
            else {
                // 达到重试上限，放弃本次重生（可以记录日志）
                Debug::Log("BreakableWall: failed to respawn after retries, leaving inactive.");
            }
        }
        });
}

// -------------------- 血包回收/生成 与 迷宫逻辑（保持原实现） --------------------

void CombatManager::RecycleHealthPack(GameObject* pack)
{
    // 移出管理列表
    auto it = std::find(activeHealthPacks.begin(), activeHealthPacks.end(), pack);
    if (it != activeHealthPacks.end())
        activeHealthPacks.erase(it);

    // 将血包移到地图外（可替换为销毁）
    pack->transform->SetPosition(CVector3(0, -1000, 0));
    pack->transform->UpdateRigidBodyTransform();
    pack->transform->UpdateColliderTransform();

    // 尝试安排下一次生成
    TryScheduleHealthPackSpawn();
}

void CombatManager::Update()
{
    // 每帧检查是否需要安排生成血包
    if (!isSpawningHealthPack && activeHealthPacks.size() < (size_t)maxHealthPacks) {
        TryScheduleHealthPackSpawn();
    }
}

void CombatManager::TryScheduleHealthPackSpawn()
{
    if (isSpawningHealthPack) return;
    if (activeHealthPacks.size() >= (size_t)maxHealthPacks) return;

    isSpawningHealthPack = true;

    float delay = Math::RandomFloat(minSpawnInterval, maxSpawnInterval);
    CountDownTimerManager::Instance().AddTimer(delay, [this]() {
        SpawnHealthPack();
        isSpawningHealthPack = false;
        });
}

void CombatManager::SpawnHealthPack()
{
    if (!HealthPackPrefab) {
        Debug::Log("HealthPackPrefab 未设置，无法生成血包！");
        return;
    }
    if (activeHealthPacks.size() >= (size_t)maxHealthPacks) return;

    // 决定用于检测的 halfExtents（尽量使用预制体自身的碰撞器）
    CVector3 halfExtents(0.5f, 0.5f, 0.5f); // 默认半尺寸
    {
        if (auto box = HealthPackPrefab->GetComponent<BoxCollider>()) {
            halfExtents = box->mCollider->mHalfSize;
            halfExtents.x = std::max(halfExtents.x, 0.01f);
            halfExtents.y = std::max(halfExtents.y, 0.01f);
            halfExtents.z = std::max(halfExtents.z, 0.01f);
        }
        else if (auto sphere = HealthPackPrefab->GetComponent<SphereCollider>()) {
            float r = sphere->mCollider->mRadius;
            r = std::max(r, 0.01f);
            halfExtents = CVector3(r, r, r);
        }
    }

    // 构造 layer mask（假设 Layer 是按位掩码类型）
    uint32_t checkMask = 0;
    // 如果你的 LayerDef 存储的是位掩码值而非索引，请根据实际情况调整
    checkMask = (uint32_t)PhysicsLit::Layer::Wall | (uint32_t)PhysicsLit::Layer::PLAYER | (uint32_t)PhysicsLit::Layer::Item;

    // 尝试多次随机位置（避免无限循环）
    const int maxAttempts = 30;
    bool found = false;
    CVector3 chosenPos;

    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        float x = Math::RandomFloat(spawnPositionMin.x, spawnPositionMax.x);
        float y = 3;
        float z = Math::RandomFloat(spawnPositionMin.z, spawnPositionMax.z);

        CVector3 candidate(x, y, z);

        // 调用 PhysicsManager::OverlapBox 进行检测
        CQuaternion rot = CQuaternion(); // 默认单位四元数
        auto overlapped = PhysicsLit::PhysicsManager::Instance().OverlapBox(candidate, halfExtents, rot, checkMask, true);

        // 如果返回为空，说明在指定层内没有重叠，可以使用该位置
        if (overlapped.empty())
        {
            found = true;
            chosenPos = candidate;
            break;
        }
        else
        {
            Debug::Log("在指定层内有重叠，不可以使用该位置 " + candidate.ToString());
        }
    }

    if (!found) {
        Debug::Log("未找到合适的生成点，放弃本次生成（尝试次数已达上限）");
        return;
    }

    // 生成血包
    Debug::Log("Begin Clone HealthPack");
    GameObject* go = GameObjectManager::Instance().Clone(HealthPackPrefab);

    go->transform->SetPosition(chosenPos);
    go->transform->UpdateRigidBodyTransform();
    go->transform->UpdateColliderTransform();

    PhysicsLit::PhysicsManager::Instance().AddGameObject(go);
    activeHealthPacks.push_back(go);

    Debug::Log("SpawnHealthPack at " + go->transform->position.ToString());
}

// -------------------- Maze generator 移入 CombatManager --------------------
// 以原始格子为单位（与外墙格子对齐）生成迷宫并放置 BreakableWallPrefab（不会删除已有物体）
// 你可以通过调用 CombatManager::Instance().GenerateAndPlaceMazeInsideArea_CellGrid(...) 来使用
void CombatManager::GenerateAndPlaceMazeInsideArea_CellGrid(
    const CVector3& AreaPos,
    int outerRows,
    int outerCols,
    float spacing,
    float yPos,
    GameObject* BreakableWallPrefab,
    float targetPassageFraction)
{
    if (!BreakableWallPrefab) return;

    // 保存最后使用的 prefab（用于后续重生）
    lastBreakablePrefab = BreakableWallPrefab;

    // 内部可用格子（排除外围一圈）
    int rows = outerRows - 2;
    int cols = outerCols - 2;
    if (rows <= 0 || cols <= 0) return;

    float cellSize = 1.0f + spacing;

    // grid: 0=wall, 1=passage
    std::vector<uint8_t> grid(rows * cols, 0);
    auto idx = [&](int r, int c) { return r * cols + c; };

    std::mt19937 rng((unsigned)std::random_device{}());
    int total = rows * cols;
    float targetFraction = targetPassageFraction;
    targetFraction = Math::Clamp(targetFraction, 0.05f, 0.95f);
    int targetPassages = std::max(1, (int)std::round(total * targetFraction));

    // start seed
    int startR = Math::RandomInt(0, rows - 1);
    int startC = Math::RandomInt(0, cols - 1);
    grid[idx(startR, startC)] = 1;
    int passageCount = 1;

    // frontier expansion (randomized)
    std::vector<std::pair<int, int>> frontier;
    auto pushFrontierIfWall = [&](int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[idx(r, c)] == 0) {
            // avoid duplicates
            bool exist = false;
            for (auto& p : frontier) if (p.first == r && p.second == c) { exist = true; break; }
            if (!exist) frontier.emplace_back(r, c);
        }
        };

    const int dr[4] = { -1,0,1,0 };
    const int dc[4] = { 0,1,0,-1 };
    pushFrontierIfWall(startR - 1, startC);
    pushFrontierIfWall(startR + 1, startC);
    pushFrontierIfWall(startR, startC - 1);
    pushFrontierIfWall(startR, startC + 1);

    while (passageCount < targetPassages && !frontier.empty()) {
        std::uniform_int_distribution<int> dist(0, (int)frontier.size() - 1);
        int idxSel = dist(rng);
        auto sel = frontier[idxSel];
        frontier.erase(frontier.begin() + idxSel);

        int r = sel.first, c = sel.second;

        // find adjacent passages
        std::vector<std::pair<int, int>> adjacentPassages;
        for (int k = 0;k < 4;++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[idx(nr, nc)] == 1)
                adjacentPassages.emplace_back(nr, nc);
        }

        if (!adjacentPassages.empty()) {
            grid[idx(r, c)] = 1;
            passageCount++;
            for (int k = 0;k < 4;++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[idx(nr, nc)] == 0)
                    pushFrontierIfWall(nr, nc);
            }
        }

        // if frontier empty but still need more passages, try to seed more frontiers
        if (frontier.empty() && passageCount < targetPassages) {
            for (int attempt = 0; attempt < 10 && passageCount < targetPassages; ++attempt) {
                int rr = Math::RandomInt(0, rows - 1);
                int cc = Math::RandomInt(0, cols - 1);
                if (grid[idx(rr, cc)] == 0) {
                    // ensure adjacent to passage to maintain connectivity
                    bool hasAdj = false;
                    for (int k = 0;k < 4;++k) {
                        int nr = rr + dr[k], nc = cc + dc[k];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[idx(nr, nc)] == 1) { hasAdj = true; break; }
                    }
                    if (hasAdj) { pushFrontierIfWall(rr, cc); break; }
                }
            }
        }
    }

    // BFS reachable from start
    auto bfsReach = [&](int sr, int sc)->std::vector<char> {
        std::vector<char> vis(rows * cols, 0);
        std::queue<std::pair<int, int>> q;
        if (grid[idx(sr, sc)] == 1) {
            vis[idx(sr, sc)] = 1;
            q.push({ sr,sc });
        }
        while (!q.empty()) {
            auto p = q.front(); q.pop();
            int r = p.first, c = p.second;
            for (int k = 0;k < 4;++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !vis[idx(nr, nc)] && grid[idx(nr, nc)] == 1) {
                    vis[idx(nr, nc)] = 1;
                    q.push({ nr,nc });
                }
            }
        }
        return vis;
        };

    auto reachable = bfsReach(startR, startC);

    // connect any unreachable passage cells to reachable set
    for (int r = 0;r < rows;++r) for (int c = 0;c < cols;++c) {
        if (grid[idx(r, c)] == 1 && !reachable[idx(r, c)]) {
            // BFS until hitting reachable
            std::queue<std::pair<int, int>> q;
            std::vector<int> parent(rows * cols, -1);
            std::vector<char> seen(rows * cols, 0);
            q.push({ r,c }); seen[idx(r, c)] = 1;
            bool found = false; int foundIdx = -1;
            while (!q.empty() && !found) {
                auto p = q.front(); q.pop();
                int cr = p.first, cc = p.second;
                for (int k = 0;k < 4;++k) {
                    int nr = cr + dr[k], nc = cc + dc[k];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !seen[idx(nr, nc)]) {
                        seen[idx(nr, nc)] = 1;
                        parent[idx(nr, nc)] = idx(cr, cc);
                        q.push({ nr,nc });
                        if (reachable[idx(nr, nc)]) { found = true; foundIdx = idx(nr, nc); break; }
                    }
                }
            }
            if (found) {
                int cur = foundIdx;
                while (cur != idx(r, c) && cur != -1) {
                    if (grid[cur] == 0) { grid[cur] = 1; passageCount++; }
                    cur = parent[cur];
                }
                reachable = bfsReach(startR, startC);
            }
        }
    }

    // braid (remove many dead ends)
    auto passageNeighborsCount = [&](int rr, int cc)->int {
        int cnt = 0;
        for (int k = 0;k < 4;++k) {
            int nr = rr + dr[k], nc = cc + dc[k];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[idx(nr, nc)] == 1) ++cnt;
        }
        return cnt;
        };

    const int maxBraid = total * 4;
    int braidIter = 0;
    bool anyBraid = true;
    while (anyBraid && braidIter < maxBraid) {
        anyBraid = false;
        std::vector<std::pair<int, int>> deadEnds;
        for (int r = 0;r < rows;++r) for (int c = 0;c < cols;++c) {
            if (grid[idx(r, c)] == 1 && passageNeighborsCount(r, c) == 1) deadEnds.emplace_back(r, c);
        }
        if (!deadEnds.empty()) {
            std::shuffle(deadEnds.begin(), deadEnds.end(), rng);
            int process = std::max(1, (int)deadEnds.size() / 3);
            for (int i = 0;i < process && i < (int)deadEnds.size(); ++i) {
                int r = deadEnds[i].first, c = deadEnds[i].second;
                std::vector<std::pair<int, int>> candidates;
                for (int k = 0;k < 4;++k) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[idx(nr, nc)] == 0) {
                        int adjPass = 0;
                        for (int kk = 0;kk < 4;++kk) {
                            int xr = nr + dr[kk], xc = nc + dc[kk];
                            if (xr >= 0 && xr < rows && xc >= 0 && xc < cols && grid[idx(xr, xc)] == 1) ++adjPass;
                        }
                        if (adjPass >= 1) candidates.emplace_back(nr, nc);
                    }
                }
                if (!candidates.empty()) {
                    auto sel = candidates[Math::RandomInt(0, (int)candidates.size() - 1)];
                    grid[idx(sel.first, sel.second)] = 1;
                    passageCount++;
                    anyBraid = true;
                }
            }
        }
        braidIter++;
    }

    // 最终：把内部 grid==0 的格子实例化 BreakableWallPrefab（与外墙格子对齐）
    // 记录所有内部网格位置到 breakWallSpawnPositions（包含已生成墙和空格）
    // 确保 breakWallSpawnPositions / breakWallOccupied 索引对齐
    size_t preCount = breakWallSpawnPositions.size();
    breakWallSpawnPositions.reserve(preCount + rows * cols);
    breakWallOccupied.reserve(preCount + rows * cols);

    for (int r = 0;r < rows;++r) {
        for (int c = 0;c < cols;++c) {
            // 内部第 (r,c) 对应世界位置：AreaPos + ((c+1), (r+1)) * cellSize
            float xPos = AreaPos.x + (c + 1) * cellSize;
            float zPos = AreaPos.z + (r + 1) * cellSize;
            CVector3 worldPos(xPos, yPos, zPos);

            // 先把位置注册到表（所有内部格子都会注册）
            breakWallSpawnPositions.push_back(worldPos);
            breakWallOccupied.push_back(false); // 先默认未占用，稍后如果生成则标记为占用
            int thisIndex = (int)breakWallSpawnPositions.size() - 1;

            if (grid[idx(r, c)] == 0) {
                // 生成 BreakableWall
                GameObject* t = GameObjectManager::Instance().Clone(BreakableWallPrefab);
                t->transform->SetPosition(worldPos);
                t->transform->UpdateRigidBodyTransform();
                t->transform->UpdateColliderTransform();
                PhysicsLit::PhysicsManager::Instance().AddGameObject(t);

                // 标记占用并建立映射
                breakWallOccupied[thisIndex] = true;
                breakWallIndexMap[t] = thisIndex;
            }
            else {
                // 通路格子，不生成内墙（保留为未占用）
            }
        }
    }
}
