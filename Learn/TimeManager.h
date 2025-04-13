#pragma once
#include "IManager.h"
#include <chrono>
#include <iostream>

class TimeManager : public IManager
{
public:
	/// <summary>
	/// 当前系统时间戳（秒）
	/// </summary>
	static long long curSysTime;
	/// <summary>
	/// 当前系统时间戳（微秒）
	/// </summary>
	static long long curSysTime_micro;
	/// <summary>
	/// 当前系统启动时间戳（秒）
	/// </summary>
	static long long curSysStartTime;
	/// <summary>
	/// 当前系统启动时间戳（微秒）
	/// </summary>
	static long long curSysStartTime_micro;
	/// <summary>
	/// 从游戏启动到当前的时间（秒）
	/// </summary>
	static float curTime;
	/// <summary>
	/// 从游戏启动到当前的时间（微秒）
	/// </summary>
	static long long curTime_micro;
	/// <summary>
	/// 从上一帧到现在的时间（秒）
	/// </summary>
	static float deltaTime;
	/// <summary>
	/// 从上一帧到现在的时间（微秒）
	/// </summary>
	static long long deltaTime_micro;
	/// <summary>
	/// 物理帧率
	/// </summary>
	static const int fixedFrameRate;
	/// <summary>
	/// 物理帧时间（秒）
	/// </summary>
	static const float fixedDeltaTime;
	/// <summary>
	/// 物理帧时间（微秒）
	/// </summary>
	static const long long fixedDeltaTime_micro;


    static TimeManager& Instance() {
        static TimeManager instance;
        return instance;
    }

	void Update() override;

    // 获取从程序启动到当前的运行时间（单位：毫秒）
    long long GetElapsedTime() const {
        return curSysTime_micro;
    }

    // 删除拷贝构造函数和赋值运算符，确保单例模式
    TimeManager(const TimeManager&) = delete;
    void operator=(const TimeManager&) = delete;

private:
    TimeManager() = default; // 构造函数私有化，确保单例
    ~TimeManager() = default; // 析构函数私有化，确保单例
};

