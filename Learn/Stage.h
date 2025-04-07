#pragma once
#include "Area.h"  // 引入 Area 类头文件
#include "CVector.h"
class Stage : public Transform,public InputManager::IInputControl {
public:
    // 构造函数，初始化舞台并设置每个区域的中心位置
    Stage(std::string name);
    Stage(std::string name,float posx, float posy, float posz);

    // 增加区域
    void AddArea(Area* area);

    // 绘制所有区域
    void Draw() override;

    void IntersectWithRay(
        const CVector& origin,
        const CVector& direct,
        float length) const;
    mutable std::shared_ptr<Shape> curSelectShape;

    void processKeyboard(unsigned char key, int x, int y)override;
    void processSpecialKeys(int key, int x, int y) override;
    void processMouse(int button, int state, int x, int y)override;
    void processMouseMotion(int x, int y)override;

    double modelViewMatrix[16];
    CVector origin,direct;
    bool isRotate;
    float angle = 0;
    Camera* camera;

    float prevMouseX;
    float prevMouseY;
    bool isLeft, isRight;

    bool isBAnimation;
    float BAnimationAngle;
    float maxHeight;  // 最大高度 
    float waveSpeed;  // 升降速度
    bool isCAnimation;
    float CAnimationAngle;


private:
    // 舞台的Area列表
    std::vector<Area*> Areas;
};
