#include "stdafx.h"
#include "LightManager.h"
void Light::Awake(){
    LightManager::Instance().RegisterLight(this);
}

void Light::Update(){
    // 点光源位置每帧同步更新
    LightManager::Instance().UpdatePointLight(this, transform->position);
}

void Light::RegisterFields(TypeInfo& info) {
    REGISTER_FIELD(Light, type);
    REGISTER_FIELD(Light, color);
    REGISTER_FIELD(Light, intensity);
    REGISTER_FIELD(Light, range);
}
void Light::PostClone(CloneContext& ctx) {

}