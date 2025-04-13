#pragma once
#include "IManager.h"
class EngineMainLoop:public IManager
{
public:
    static EngineMainLoop& Instance() {
        static EngineMainLoop instance;
        return instance;
    }

    void Update()override;

    EngineMainLoop(const EngineMainLoop&) = delete;
    void operator=(const EngineMainLoop&) = delete;


private:
    EngineMainLoop() = default;
    ~EngineMainLoop() = default;

};

