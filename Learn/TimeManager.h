#pragma once
#include "IManager.h"
#include <chrono>
#include <iostream>

class TimeManager : public IManager
{
public:
	/// <summary>
	/// ��ǰϵͳʱ������룩
	/// </summary>
	static long long curSysTime;
	/// <summary>
	/// ��ǰϵͳʱ�����΢�룩
	/// </summary>
	static long long curSysTime_micro;
	/// <summary>
	/// ��ǰϵͳ����ʱ������룩
	/// </summary>
	static long long curSysStartTime;
	/// <summary>
	/// ��ǰϵͳ����ʱ�����΢�룩
	/// </summary>
	static long long curSysStartTime_micro;
	/// <summary>
	/// ����Ϸ��������ǰ��ʱ�䣨�룩
	/// </summary>
	static float curTime;
	/// <summary>
	/// ����Ϸ��������ǰ��ʱ�䣨΢�룩
	/// </summary>
	static long long curTime_micro;
	/// <summary>
	/// ����һ֡�����ڵ�ʱ�䣨�룩
	/// </summary>
	static float deltaTime;
	/// <summary>
	/// ����һ֡�����ڵ�ʱ�䣨΢�룩
	/// </summary>
	static long long deltaTime_micro;
	/// <summary>
	/// ����֡��
	/// </summary>
	static const int fixedFrameRate;
	/// <summary>
	/// ����֡ʱ�䣨�룩
	/// </summary>
	static const float fixedDeltaTime;
	/// <summary>
	/// ����֡ʱ�䣨΢�룩
	/// </summary>
	static const long long fixedDeltaTime_micro;


    static TimeManager& Instance() {
        static TimeManager instance;
        return instance;
    }

	void Update() override;

    // ��ȡ�ӳ�����������ǰ������ʱ�䣨��λ�����룩
    long long GetElapsedTime() const {
        return curSysTime_micro;
    }

    // ɾ���������캯���͸�ֵ�������ȷ������ģʽ
    TimeManager(const TimeManager&) = delete;
    void operator=(const TimeManager&) = delete;

private:
    TimeManager() = default; // ���캯��˽�л���ȷ������
    ~TimeManager() = default; // ��������˽�л���ȷ������
};

