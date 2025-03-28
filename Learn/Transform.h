#pragma once
#include "CVector.h"
#include "CMatrix.h"
#include "CEuler.h"
#include "OBB.h"
#include <vector>
#include <string>

class Transform {
public:
    std::string name;
    CVector position;   // 世界坐标
    CMatrix rotation;   // 世界旋转矩阵
    CEuler eulerAngles; // 世界欧拉角（绕 X、Y、Z 轴的旋转角度）
    bool isShowLocalAxis;
    OBB obb;            // 绑定的有向包围盒

    CVector localPosition;    // 相对于父 Transform 的位置
    CMatrix localRotation;    // 相对于父 Transform 的旋转矩阵
    CEuler localEulerAngles;  // 相对于父 Transform 的欧拉角

    Transform* parent = nullptr;     // 父对象
    std::vector<Transform*> children; // 子对象列表

    Transform(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false)
        : name(name), position(position), rotation(rotation),
        eulerAngles(eulerAngles), obb(position, CVector(), rotation),
        isShowLocalAxis(isShowLocalAxis), localPosition(position),
        localRotation(rotation), localEulerAngles(eulerAngles)
    {
        UpdateTransformFromLocal(); // 初始化时确保数据同步
    }

    virtual void SetPosition(const CVector& newPos) {
        if (parent) {
            // 将世界坐标转换为父空间的本地坐标
            CVector parentSpacePos = newPos - parent->position;
            localPosition = parent->rotation.GetInverse().vecMul(parentSpacePos);
        }
        else {
            localPosition = newPos;
        }
        UpdateTransformFromLocal();
    }

    virtual void SetPositionDelta(const CVector& newPosDelta) {
        SetPosition(position + newPosDelta);
    }
    virtual void SetPositionDelta(float x,float y,float z) {
        SetPosition(position + CVector(x,y,z));
    }
    virtual void SetRotationDelta(const CMatrix& newRotation) {
        if (parent) {
            // 计算相对于父物体的本地旋转
            localRotation = parent->rotation.GetInverse() * newRotation;
        }
        else {
            localRotation = newRotation;
        }
        UpdateTransformFromLocal();
    }
    virtual void SetRotationDelta(CEuler rotationDelta)
    {
        SetRotationDelta(rotationDelta.ToCMatrix());
    }
    virtual void SetRotationDelta(float h, float p, float b)
    {
        SetRotationDelta(CEuler(h, p, b));
    }

    virtual void SetEulerAngles(const CEuler& newAngles) {
        if (parent) {
            // 计算相对于父物体的本地欧拉角
            CMatrix worldRot = newAngles.ToCMatrix();
            localRotation = parent->rotation.GetInverse() * worldRot;
            localEulerAngles = localRotation.ToEuler();
        }
        else {
            localRotation = newAngles.ToCMatrix();
            localEulerAngles = newAngles;
        }
        UpdateTransformFromLocal();
    }

    // 从本地坐标系更新世界坐标系
    void UpdateTransformFromLocal() {
        if (parent) {
            // 计算世界坐标系变换
            rotation = parent->rotation * localRotation;
            position = parent->position + parent->rotation.vecMul(localPosition);
        }
        else {
            rotation = localRotation;
            position = localPosition;
        }
        eulerAngles = rotation.ToEuler();

        obb.Update(position, rotation);
        UpdateChildrenTransform();
    }

    // 更新子物体变换
    void UpdateChildrenTransform() {
        for (auto& child : children) {
            if (!child) continue;

            // 子物体的世界旋转 = 父旋转 * 子本地旋转
            child->rotation = rotation * child->localRotation;
            // 子物体的世界位置 = 父位置 + 父旋转后的本地位置
            child->position = position + rotation.vecMul(child->localPosition);
            child->eulerAngles = child->rotation.ToEuler();
            child->obb.Update(child->position, child->rotation);

            // 递归更新子物体
            child->UpdateChildrenTransform();
        }
    }

    void AddChild(Transform* child) {
        if (!child) return;

        children.push_back(child);
        child->parent = this;

        // 初始化子物体的本地坐标
        child->localPosition = rotation.GetInverse().vecMul(child->position - position);
        child->localRotation = rotation.GetInverse() * child->rotation;
        child->localEulerAngles = child->localRotation.ToEuler();

        // 更新子物体的世界坐标
        child->UpdateTransformFromLocal();
    }

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

    virtual void Update()
    {
        
    }

    // 应用变换（包括平移和旋转）
    virtual void Draw() const {
        glTranslatef(position.x, position.y, position.z);
        glRotatef(eulerAngles.h, 0, 1, 0); // 绕 Y 轴旋转
        glRotatef(eulerAngles.p, 1, 0, 0); // 绕 X 轴旋转
        glRotatef(eulerAngles.b, 0, 0, 1); // 绕 Z 轴旋转

        if (isShowLocalAxis) {
            // 绘制本地坐标系
            glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f); // 红色（X 轴）
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(10.0f, 0.0f, 0.0f);
            glEnd();

            glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 0.0f); // 绿色（Y 轴）
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 10.0f, 0.0f);
            glEnd();

            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 1.0f); // 蓝色（Z 轴）
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 10.0f);
            glEnd();
        }
    }
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

    virtual ~Transform() {
        for (auto& child : children) {
            if (child) delete child;
        }
    }
};
