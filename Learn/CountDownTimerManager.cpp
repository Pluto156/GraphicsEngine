#include "stdafx.h"
#include "CountDownTimerManager.h"


CountDownTimerManager::TimerID CountDownTimerManager::AddTimer(float durationSeconds, Callback onFinished)
{
    Timer timer;
    timer.duration = durationSeconds;
    timer.remaining = durationSeconds;
    timer.onFinished = std::move(onFinished);
    timer.finished = false;

    TimerID id = nextTimerID++;

    if (inUpdate)
        pendingAdditions[id] = std::move(timer);
    else
        timers[id] = std::move(timer);

    return id;
}

void CountDownTimerManager::RemoveTimer(TimerID id)
{
    if (inUpdate)
        pendingRemovals.push_back(id);
    else
        timers.erase(id);
}

void CountDownTimerManager::Update()
{
    inUpdate = true;

    float deltaTime = TimeManager::deltaTime;
    std::vector<TimerID> toRemove;

    for (auto it = timers.begin(); it != timers.end(); ++it)
    {
        TimerID id = it->first;
        Timer& timer = it->second;

        if (timer.finished) continue;

        timer.remaining -= deltaTime;
        if (timer.remaining <= 0.0f)
        {
            timer.finished = true;
            if (timer.onFinished)
                timer.onFinished();
            toRemove.push_back(id);
        }
    }

    // 标记的结束计时器
    for (size_t i = 0; i < toRemove.size(); ++i)
    {
        timers.erase(toRemove[i]);
    }

    inUpdate = false;

    // 应用所有待添加的计时器
    for (auto it = pendingAdditions.begin(); it != pendingAdditions.end(); ++it)
    {
        TimerID id = it->first;
        Timer& timer = it->second;
        timers[id] = std::move(timer);
    }
    pendingAdditions.clear();

    // 应用所有待移除的计时器
    for (size_t i = 0; i < pendingRemovals.size(); ++i)
    {
        timers.erase(pendingRemovals[i]);
    }
    pendingRemovals.clear();
}

