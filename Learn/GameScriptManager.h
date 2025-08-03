#pragma once
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
#include "IManager.h"
#include "GameScript.h"

// -----------------------------------------------------------------------------
// GameScriptManager
// -----------------------------------------------------------------------------
// 负责在脚本层派发碰撞 & 触发事件（Enter / Stay / Exit）。
// 改动要点：
//   1. 立刻在 ReportTrigger 中调用 OnTriggerEnter / OnTriggerStay，实时反馈。
//   2. 仅在 Update() 末尾比较 last & current 集合来派发 OnTriggerExit。
//   3. ReportCollision 保持按帧累积，Enter/Stay/Exit 全在 Update() 比较。
//   4. 支持 layer & layerMask 过滤。
// -----------------------------------------------------------------------------

class GameScriptManager : public IManager {
public:
    static GameScriptManager& Instance() {
        static GameScriptManager instance;
        return instance;
    }

    /*---------------------------------------------------------------------
     |   碰撞（非 Trigger）
     *---------------------------------------------------------------------*/
    void ReportCollision(PhysicsLit::RigidBodyPrimitive* a, PhysicsLit::RigidBodyPrimitive* b) {
        auto key = MakePairKey(a, b);
        currentFrameContacts.insert(key);
    }

    /*---------------------------------------------------------------------
     |   Trigger：实时派发 Enter / Stay
     *---------------------------------------------------------------------*/
    void ReportTrigger(PhysicsLit::RigidBodyPrimitive* a, PhysicsLit::RigidBodyPrimitive* b) {
        
        auto key = MakePairKey(a, b);

        // 插入 currentFrameTriggers；返回 true 表示本帧第一次插入
        bool firstInsertThisFrame = currentFrameTriggers.insert(key).second;

        if (firstInsertThisFrame) {
            // 判断历史记录以决定是 Enter 还是 Stay
            if (lastFrameTriggers.find(key) == lastFrameTriggers.end()) {
                Notify(a, b, &GameScript::OnTriggerEnter);
            }
            else {
                Notify(a, b, &GameScript::OnTriggerStay);
            }
        }
        else {
            // 已经存在于 current，本帧再次检测到 => OnTriggerStay
            Notify(a, b, &GameScript::OnTriggerStay);
        }
    }

    /*---------------------------------------------------------------------
     |   帧结束：派发 Collision 事件全部阶段，Trigger 仅 Exit
     *---------------------------------------------------------------------*/
    void Update() override {
        DispatchCollisionEvents();   // Enter / Stay / Exit for collisions
        DispatchTriggerExitEvents(); // only Exit for triggers

        // 备份集合，为下帧比较做准备
        std::swap(currentFrameContacts, lastFrameContacts);
        std::swap(currentFrameTriggers, lastFrameTriggers);
        currentFrameContacts.clear();
        currentFrameTriggers.clear();
    }
    /*------------------------------------------------------------
 | 对象失效清理
 | 当 RigidBodyPrimitive 被销毁时请调用 InvalidateObject(obj)
 | 这样能移除所有包含该指针的记录，避免新对象复用同地址造成误判
 *-----------------------------------------------------------*/
    void InvalidateObject(PhysicsLit::RigidBodyPrimitive* obj) {
        auto pred = [obj](const PairKey& p) {
            return p.first == obj || p.second == obj;
            };
        EraseIf(currentFrameContacts, pred);
        EraseIf(lastFrameContacts, pred);
        EraseIf(currentFrameTriggers, pred);
        EraseIf(lastFrameTriggers, pred);
    }
private:
    GameScriptManager() = default;
    ~GameScriptManager() = default;
    GameScriptManager(const GameScriptManager&) = delete;
    GameScriptManager& operator=(const GameScriptManager&) = delete;

    // ---------------------------------------------
    // 类型别名 & 数据成员
    // ---------------------------------------------
    using RigidPtr = PhysicsLit::RigidBodyPrimitive*;
    using PairKey = std::pair<RigidPtr, RigidPtr>;

    std::set<PairKey> currentFrameContacts;
    std::set<PairKey> lastFrameContacts;
    std::set<PairKey> currentFrameTriggers;
    std::set<PairKey> lastFrameTriggers;

    // ---------------------------------------------
    // 工具函数
    // ---------------------------------------------
    static PairKey MakePairKey(RigidPtr a, RigidPtr b) {
        return (a < b) ? PairKey(a, b) : PairKey(b, a);
    }

    bool CanLayersCollide(PhysicsLit::CollisionPrimitive* a, PhysicsLit::CollisionPrimitive* b) const {
        return (((1u << b->layer) & a->layerMask) != 0) &&
            (((1u << a->layer) & b->layerMask) != 0);
    }

    using CollisionCallback = void (GameScript::*)(Collider*);



    template<typename SetT, typename Pred>
    static void EraseIf(SetT& st, Pred pr) {
        for (auto it = st.begin(); it != st.end(); ) {
            if (pr(*it)) it = st.erase(it);
            else ++it;
        }
    }

    // ---------------------------------------------
    // 工具函数

    void Notify(RigidPtr a, RigidPtr b, CollisionCallback cb) {
        GameScript* ga = PhysicsLit::PhysicsManager::Instance().TryGetGameScript(a);
        GameScript* gb = PhysicsLit::PhysicsManager::Instance().TryGetGameScript(b);
        if (ga&&gb)
        {
            (ga->*cb)(gb->gameObject->GetComponent<Collider>());
            (gb->*cb)(ga->gameObject->GetComponent<Collider>());
        }
    }

    // ---------------------------------------------
    // Collision 事件派发
    // ---------------------------------------------
    void DispatchCollisionEvents() {
        // Enter / Stay
        for (const auto& pair : currentFrameContacts) {
            if (lastFrameContacts.find(pair) == lastFrameContacts.end())
                Notify(pair.first, pair.second, &GameScript::OnColliderEnter);
            else
                Notify(pair.first, pair.second, &GameScript::OnColliderStay);
        }
        // Exit
        for (const auto& pair : lastFrameContacts) {
            if (currentFrameContacts.find(pair) == currentFrameContacts.end())
                Notify(pair.first, pair.second, &GameScript::OnColliderExit);
        }
    }

    // ---------------------------------------------
    // Trigger Exit 派发
    // ---------------------------------------------
    void DispatchTriggerExitEvents() {
        for (const auto& pair : lastFrameTriggers) {
            if (currentFrameTriggers.find(pair) == currentFrameTriggers.end())
                Notify(pair.first, pair.second, &GameScript::OnTriggerExit);
        }
    }
};
