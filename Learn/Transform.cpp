#include "stdafx.h"
#include "Transform.h"
void Transform::SetPosition(const CVector& newPos) {
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
 void Transform::SetPositionDelta(const CVector& newPosDelta) {
    SetPosition(position + newPosDelta);
}
 void Transform::SetPositionDelta(float x, float y, float z) {
    SetPosition(position + CVector(x, y, z));
}
 void Transform::SetLocalPosition(const CVector& newPos) {
    localPosition = newPos;
    UpdateTransformFromLocal();
}
 void Transform::SetLocalPositionDelta(const CVector& newPosDelta) {
    SetLocalPosition(localPosition + newPosDelta);
}
 void Transform::SetLocalPositionDelta(float x, float y, float z) {
    SetLocalPosition(localPosition + CVector(x, y, z));
}

/// <summary>
/// 该函数设置的这个旋转矩阵是应用于父坐标系的
/// </summary>
/// <param name="newRotation"></param>
 void Transform::SetRotation(const CMatrix& newRotation) {
    localRotation = newRotation;
    UpdateTransformFromLocal();
}
/// <summary>
/// lookDir是基于世界坐标系的向量
/// </summary>
/// <param name="lookDir"></param>
 void Transform::LookAt(const CVector& lookDir) {

    CVector t = lookDir;
    CMatrix worldMatrix = lookDir.ToCMatrix();
    if (parent)
    {
        localRotation = parent->rotation.GetInverse() * worldMatrix;
    }
    else
    {
        localRotation = worldMatrix;
    }
    UpdateTransformFromLocal();
}

 void Transform::SetRotationDelta(const CMatrix& newRotation) {
    if (parent) {
        // 计算相对于父物体的本地旋转
        localRotation = parent->rotation * newRotation;
    }
    else {
        localRotation = localRotation * newRotation;
    }
    UpdateTransformFromLocal();
}
 void Transform::SetRotationDelta(CEuler rotationDelta)
{
    SetRotationDelta(rotationDelta.ToCMatrix());
}
 void Transform::SetRotationDelta(float h, float p, float b)
{
    SetRotationDelta(CEuler(h, p, b));
}

 void Transform::SetEulerAngles(float h, float p, float b)
{
    SetEulerAngles(CEuler(h, p, b));
}

 void Transform::SetEulerAngles(const CEuler& newAngles) {
    // 限制垂直视角的范围，避免摄像机完全翻转
    CEuler newAnglesCopy = newAngles;
    if (newAnglesCopy.p > 89.0f) newAnglesCopy.p = 89.0f;
    if (newAnglesCopy.p < -89.0f) newAnglesCopy.p = -89.0f;
    if (parent) {
        // 计算相对于父物体的本地欧拉角
        CMatrix worldRot = newAnglesCopy.ToCMatrix();
        localRotation = parent->rotation.GetInverse() * worldRot;
        localEulerAngles = localRotation.ToEuler();
    }
    else {
        localRotation = newAnglesCopy.ToCMatrix();
        localEulerAngles = newAnglesCopy;
    }
    UpdateTransformFromLocal();
}

 void Transform::SetEulerAnglesDelta(CEuler eulerAnglesDelta)
{
    SetEulerAngles(eulerAngles + eulerAnglesDelta);
}

 void Transform::SetEulerAnglesDelta(float h, float p, float b)
{
    SetEulerAngles(eulerAngles + CEuler(h, p, b));
}

// 从本地坐标系更新世界坐标系
void Transform::UpdateTransformFromLocal() {
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
    localEulerAngles = localRotation.ToEuler();
    UpdateFRU();
    UpdateChildrenTransform();
}

// 更新子物体变换
void Transform::UpdateChildrenTransform() {
    for (auto& child : children) {
        if (!child) continue;

        // 子物体的世界旋转 = 父旋转 * 子本地旋转
        child->rotation = rotation * child->localRotation;
        // 子物体的世界位置 = 父位置 + 父旋转后的本地位置
        child->position = position + rotation.vecMul(child->localPosition);
        child->eulerAngles = child->rotation.ToEuler();
        // 递归更新子物体
        child->UpdateChildrenTransform();
    }
}

void Transform::AddChild(Transform* child) {
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

void Transform::ApplyTransform()
{
    //Transform 变换部分
    glTranslatef(position.x, position.y, position.z);
    glRotatef(eulerAngles.h, 0, 1, 0); // 绕 Y 轴旋转
    glRotatef(eulerAngles.p, 1, 0, 0); // 绕 X 轴旋转
    glRotatef(eulerAngles.b, 0, 0, 1); // 绕 Z 轴旋转

    if (isShowLocalAxis) {
        // 绘制本地坐标系
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // 红色（X 轴）
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f); // 绿色（Y 轴）
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f); // 蓝色（Z 轴）
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
    }
    
}
