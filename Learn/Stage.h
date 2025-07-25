#pragma once
#include "CVector3.h"
#include "GameObject.h"
class Stage : public GameScript {
public:

    void Update()override;

    void IntersectWithRay(
        const CVector3& origin,
        const CVector3& direct,
        float length) const;
    mutable GameObject* curSelectShape;


    void StageDebug();


    double modelViewMatrix[16];
    CVector3 origin,direct;
    bool isRotate;
    float angle = 0;
    Camera* camera;

    GameObject* Car;
    GameObject* Car2;

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

};
