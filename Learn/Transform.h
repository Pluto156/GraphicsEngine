#pragma once
#include "CVector.h"
#include "CMatrix.h"
#include "CEuler.h"
#include "Component.h"

#include <vector>
#include <string>
class Transform :public Component{
public:
    CVector position;   // 世界坐标
    CMatrix rotation;   // 世界旋转矩阵
    CEuler eulerAngles; // 世界欧拉角（绕 X、Y、Z 轴的旋转角度）

    CVector localPosition;    // 相对于父 Transform 的位置
    CMatrix localRotation;    // 相对于父 Transform 的旋转矩阵
    CEuler localEulerAngles;  // 相对于父 Transform 的欧拉角

    CVector Up;
    CVector Forward;
    CVector Right;

    GameObject* gameobject = nullptr;
    Transform* parent = nullptr;     // 父对象
    std::vector<Transform*> children; // 子对象列表


    bool isShowLocalAxis;



    Transform(const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(), bool isShowLocalAxis = false)
        :position(position), rotation(rotation),
        eulerAngles(eulerAngles),localPosition(position),
        localRotation(rotation), localEulerAngles(eulerAngles),isShowLocalAxis(isShowLocalAxis)
    {
        UpdateTransformFromLocal(); // 初始化时确保数据同步
        
    }

    virtual void SetPosition(const CVector& newPos);
    virtual void SetPositionDelta(const CVector& newPosDelta);
    virtual void SetPositionDelta(float x, float y, float z);
    virtual void SetLocalPosition(const CVector& newPos);
    virtual void SetLocalPositionDelta(const CVector& newPosDelta);
    virtual void SetLocalPositionDelta(float x, float y, float z);
    
    /// <summary>
    /// 该函数设置的这个旋转矩阵是应用于父坐标系的
    /// </summary>
    /// <param name="newRotation"></param>
    virtual void SetRotation(const CMatrix& newRotation);
    /// <summary>
    /// lookDir是基于世界坐标系的向量
    /// </summary>
    /// <param name="lookDir"></param>
    virtual void LookAt(const CVector& lookDir);

    virtual void SetRotationDelta(const CMatrix& newRotation);
    virtual void SetRotationDelta(CEuler rotationDelta);
    virtual void SetRotationDelta(float h, float p, float b);
    virtual void SetEulerAngles(float h, float p, float b);
    virtual void SetEulerAngles(const CEuler& newAngles);

    virtual void SetEulerAnglesDelta(CEuler eulerAnglesDelta);
    virtual void SetEulerAnglesDelta(float h, float p, float b);

    // 从本地坐标系更新世界坐标系
    void UpdateTransformFromLocal();

    // 更新子物体变换
    void UpdateChildrenTransform();

    void AddChild(Transform* child);

    // 获取位置
    virtual CVector GetPosition() const {
        return position;
    }

    // 获取旋转矩阵
    virtual CMatrix GetRotation() const {
        return rotation;
    }

    // 获取欧拉角
    virtual CEuler GetEulerAngles() const {
        return eulerAngles;
    }

    void ApplyTransform();




    // 获取本地位置
    CVector GetLocalPosition() const {
        return localPosition;
    }

    // 获取本地旋转矩阵
    CMatrix GetLocalRotation() const {
        return localRotation;
    }

    // 获取本地欧拉角
    CEuler GetLocalEulerAngles() const {
        return localEulerAngles;
    }

    virtual void UpdateFRU()
    {
        Forward = rotation.GetForward();
        Right = rotation.GetRight();
        Up = rotation.GetUp();
    }

    virtual ~Transform() {
        for (auto& child : children) {
            if (child) delete child;
        }
    }
};
