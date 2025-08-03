#pragma once
#include <vector>
#include "Component.h"
#include "CVector3.h"

struct PointLightData {
    CVector3 position;
    CVector3 color;
    float intensity;
    float range;
};

enum class LightType {
    Point,
    Directional,
    Spot
};
class LightManager;
class Light : public Component {
    REGISTER_COMPONENT_DERIVED(Light, ComponentType::Light, Component)
public:
    LightType type = LightType::Point;
    CVector3 color = CVector3(1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
    float range = 10.0f;

    void Awake() override;
    void Update() override;
};

class LightManager : public IManager {
public:
    std::vector<Light*> pointLights;
    // 是否启用光照
    bool EnableLighting = false;
    static LightManager& Instance() {
        static LightManager instance;
        return instance;
    }

    void Update() override {
        // 可在主循环调用：刷新状态
    }

    void RegisterLight(Light* light) {
        if (light->type == LightType::Point) {
            pointLights.push_back(light);
        }
    }

    void UpdatePointLight(Light* light, const CVector3& newPos) {
        // 简单更新，不用做太多
        // 此处可添加灯光移动通知逻辑
    }

    void GatherLightUniforms(std::vector<PointLightData>& outData) {
        outData.clear();
        for (auto* light : pointLights) {
            PointLightData data;
            data.position = light->transform->position;
            data.color = light->color;
            data.intensity = light->intensity;
            data.range = light->range;
            outData.push_back(data);
        }
    }
    // 初始化光照的函数
    void InitLighting()
    {
        if (EnableLighting) {
            glDisable(GL_LIGHTING);
            return;
        }
        glEnable(GL_LIGHTING);         // 启用光照系统
        glEnable(GL_COLOR_MATERIAL);  // 启用颜色材质绑定
        // 让 glColor 影响材质的 ambient 和 diffuse
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    }

private:
    LightManager() = default;
    ~LightManager() = default;
    LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;
};
