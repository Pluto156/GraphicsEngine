#pragma once
class IManager
{
public:
    //virtual void OnCreate() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() {}

    //virtual void OnDestroy() = 0;
};

