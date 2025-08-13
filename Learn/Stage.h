#pragma once
#include "CVector3.h"
#include "GameObject.h"
#include "CQuaternion.h"

class Stage : public GameScript {
public:

    void Update() override;

    void StageDebug();


    double modelViewMatrix[16];
    CVector3 origin, direct;
    bool isRotate;
    float angle = 0;
    GameObject* curSelectShape;
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


    Stage() :
        curSelectShape(nullptr),
        camera(nullptr),
        Car(nullptr),
        Car2(nullptr),
        isRotate(false),
        angle(0.0f),
        prevMouseX(0.0f),
        prevMouseY(0.0f),
        isLeft(false),
        isRight(false),
        isBAnimation(false),
        BAnimationAngle(0.0f),
        maxHeight(0.0f),
        waveSpeed(0.0f),
        isCAnimation(false),
        CAnimationAngle(0.0f),
        // move defaults
        isMovingToTarget(false),
        moveDuration(2.0f),
        moveElapsed(0.0f),
        hasSavedInitial(false)
    {
    }

private:
    // generic move control
    bool isMovingToTarget;
    float moveDuration;   // seconds
    float moveElapsed;    // seconds

    CVector3 moveStartPos;
    CQuaternion moveStartQuat;
    CVector3 moveTargetPos;
    CQuaternion moveTargetQuat;

    // initial camera transform (saved once at program start)
    CVector3 savedInitialPos;
    CQuaternion savedInitialQuat;
    bool hasSavedInitial;

    // helper to start move
    void StartMove(const CVector3& targetPos, const CQuaternion& targetQuat)
    {
        if (!camera || !camera->transform) return;
        // start from the camera's current (world) transform
        moveStartPos = camera->transform->position;
        moveStartQuat = camera->transform->quaternion;
        moveTargetPos = targetPos;
        moveTargetQuat = targetQuat;

        moveElapsed = 0.0f;
        isMovingToTarget = true;
    }
};
