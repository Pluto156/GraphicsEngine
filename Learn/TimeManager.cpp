#include "stdafx.h"
#include "TimeManager.h"
long long TimeManager::curSysTime = 0;
long long TimeManager::curSysTime_micro = 0;
long long TimeManager::curSysStartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
long long TimeManager::curSysStartTime_micro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
float TimeManager::curTime = 0.0f;
long long TimeManager::curTime_micro = 0;
float TimeManager::deltaTime = 0.0f;
long long TimeManager::deltaTime_micro = 0;
const int TimeManager::fixedFrameRate = 100;
const float TimeManager::fixedDeltaTime = 1.0f / TimeManager::fixedFrameRate;
const long long TimeManager::fixedDeltaTime_micro = 1'000'000 / TimeManager::fixedFrameRate;

void TimeManager::Update()
{
	long long time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long long time_micro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// ����ǵ�һ��Update������deltaTimeΪ0���䣬�������ʱ���
	if (curSysTime_micro != 0)
	{
		// ������΢�����deltaTime���ú����п��ܾ��Ȳ���
		deltaTime_micro = time_micro - curSysTime_micro;
		deltaTime = static_cast<float>(deltaTime_micro) / 1'000'000.0f;
	}

	curTime += deltaTime;
	curTime_micro += deltaTime_micro;

	curSysTime = time;
	curSysTime_micro = time_micro;
}

