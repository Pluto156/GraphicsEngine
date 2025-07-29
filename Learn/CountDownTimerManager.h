#pragma once
#include "IManager.h"
#include <unordered_map>
#include <functional>
#include <vector>

class CountDownTimerManager : public IManager
{
public:
    static CountDownTimerManager& Instance() {
        static CountDownTimerManager instance;
        return instance;
    }

    using TimerID = int;
    using Callback = std::function<void()>;

    struct Timer
    {
        float duration;
        float remaining;
        Callback onFinished;
        bool finished = false;
    };

    TimerID AddTimer(float durationSeconds, Callback onFinished);
    void RemoveTimer(TimerID id);

    void Update() override;
    void LateUpdate() override {}

private:
    CountDownTimerManager() : nextTimerID(1), inUpdate(false) {}
    ~CountDownTimerManager() = default;
    CountDownTimerManager(const CountDownTimerManager&) = delete;
    CountDownTimerManager& operator=(const CountDownTimerManager&) = delete;

    std::unordered_map<TimerID, Timer> timers;
    std::unordered_map<TimerID, Timer> pendingAdditions;
    std::vector<TimerID> pendingRemovals;

    TimerID nextTimerID;
    bool inUpdate;
};
