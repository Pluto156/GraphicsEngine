#include "stdafx.h"
#include "LightManager.h"
void Light::Start(){
    LightManager::Instance().RegisterLight(this);
}

void Light::Update(){
    // 点光源位置每帧同步更新
    LightManager::Instance().UpdatePointLight(this, transform->position);
}