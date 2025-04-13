#pragma once
#include "IManager.h"
class RenderManager :public IManager
{
public:
    static RenderManager& Instance() {
        static RenderManager instance;
        return instance;
    }

    void Update()override;

    RenderManager(const RenderManager&) = delete;
    void operator=(const RenderManager&) = delete;


private:
    RenderManager() = default;
    ~RenderManager() = default;

};
