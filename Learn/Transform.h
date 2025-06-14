#pragma once
#include "CVector3.h"
#include "CMatrix4.h"
#include "CEuler.h"
#include "CQuaternion.h"
#include "Component.h"

#include <vector>
#include <string>
class Transform :public Component{
public:
    static ComponentType GetType();
public:
    CVector3 position;   // 世界坐标
    CMatrix4 rotation;   // 世界旋转矩阵
    CEuler eulerAngles; // 世界欧拉角（绕 X、Y、Z 轴的旋转角度）
    CQuaternion quaternion;

    CVector3 localPosition;    // 相对于父 Transform 的位置
    CMatrix4 localRotation;    // 相对于父 Transform 的旋转矩阵
    CEuler localEulerAngles;  // 相对于父 Transform 的欧拉角

    CVector3 localScale;

    CVector3 Up;
    CVector3 Forward;
    CVector3 Right;

    GameObject* gameobject = nullptr;
    Transform* parent = nullptr;     // 父对象
    std::vector<Transform*> children; // 子对象列表


    bool isShowLocalAxis;



    Transform(const CVector3& position = CVector3(),
        const CMatrix4& rotation = CMatrix4(),
        const CEuler& eulerAngles = CEuler(), bool isShowLocalAxis = false)
        :position(position), rotation(rotation),
        eulerAngles(eulerAngles), quaternion(CQuaternion()), localPosition(position),
        localRotation(rotation), localEulerAngles(eulerAngles),localScale(CVector3(1,1,1)), isShowLocalAxis(isShowLocalAxis)
    {
        UpdateTransformFromLocal(false); // 初始化时确保数据同步
        
    }

    void SetPosition(const CVector3& newPos);
    void SetPositionDelta(const CVector3& newPosDelta);
    void SetPositionDelta(float x, float y, float z);
    void SetLocalPosition(const CVector3& newPos);
    void SetLocalPositionDelta(const CVector3& newPosDelta);
    void SetLocalPositionDelta(float x, float y, float z);
    void SetLocalScale(const CVector3& newLocalScale);

    
    /// <summary>
    /// 该函数设置的这个旋转矩阵是应用于父坐标系的
    /// </summary>
    /// <param name="newRotation"></param>
    void SetRotation(const CMatrix4& newRotation);
    /// <summary>
    /// lookDir是基于世界坐标系的向量
    /// </summary>
    /// <param name="lookDir"></param>
    void LookAt(const CVector3& lookDir);
    void SetRotationDelta(const CMatrix4& newRotation);
    void SetRotationDelta(CEuler rotationDelta);
    void SetRotationDelta(float h, float p, float b);

    void SetQuaternion(CQuaternion quaternion);
    void SetQuaternionDelta(CQuaternion quaternionDelta);
    void SetQuaternionDelta(float w,float x,float y,float z);


    void SetEulerAngles(float h, float p, float b);
    void SetEulerAngles(const CEuler& newAngles);
    void SetEulerAnglesDelta(CEuler eulerAnglesDelta);
    void SetEulerAnglesDelta(float h, float p, float b);

    // 从本地坐标系更新世界坐标系
    void UpdateTransformFromLocal(bool UpdateColliderTransform = true);

    // 更新子物体变换
    void UpdateChildrenTransform();

    void UpdateRigidBodyTransform();

    void UpdateColliderTransform();

    void AddChild(Transform* child);

    // 获取位置
    CVector3 GetPosition() const {
        return position;
    }

    // 获取旋转矩阵
    CMatrix4 GetRotation() const {
        return rotation;
    }

    CQuaternion GetQuaternion() const {
        return quaternion;
    }

    // 获取欧拉角
    CEuler GetEulerAngles() const {
        return eulerAngles;
    }

    void ApplyTransform();

    // 获取本地位置
    CVector3 GetLocalPosition() const {
        return localPosition;
    }

    // 获取本地旋转矩阵
    CMatrix4 GetLocalRotation() const {
        return localRotation;
    }

    // 获取本地欧拉角
    CEuler GetLocalEulerAngles() const {
        return localEulerAngles;
    }

    void UpdateFRU()
    {
        Forward = rotation.GetForward();
        Right = rotation.GetRight();
        Up = rotation.GetUp();
    }
    CMatrix4 GetWorldTransformMatrix();

    virtual ~Transform() {
        for (auto& child : children) {
            if (child) delete child;
        }
    }



private:
    CMatrix4 worldTransformMatrix;//世界变换矩阵
};
