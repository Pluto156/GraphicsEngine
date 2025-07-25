#pragma once
class DebugManager:public IManager
{
public:
    static DebugManager& Instance() {
        static DebugManager instance;
        return instance;
    }
    void Update() override;
    bool EnableGizmos = true;

private:
    DebugManager() = default;
    ~DebugManager() = default;
    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;
};

