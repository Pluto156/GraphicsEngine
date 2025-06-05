#include "stdafx.h"
#include "Transform.h"
ComponentType Transform::GetType()
{
    return ComponentType::Transform;
}
void Transform::SetPosition(const CVector3& newPos) {
    if (parent) {
        // 将世界坐标转换为父空间的本地坐标
        CVector3 parentSpacePos = newPos - parent->position;
        localPosition = parent->rotation.GetInverse().vecMulVector3(parentSpacePos);
    }
    else {
        localPosition = newPos;
    }
    UpdateTransformFromLocal();
}
 void Transform::SetPositionDelta(const CVector3& newPosDelta) {
    SetPosition(position + newPosDelta);
}
 void Transform::SetPositionDelta(float x, float y, float z) {
    SetPosition(position + CVector3(x, y, z));
}
 void Transform::SetLocalPosition(const CVector3& newPos) {
    localPosition = newPos;
    UpdateTransformFromLocal();
}
 void Transform::SetLocalPositionDelta(const CVector3& newPosDelta) {
    SetLocalPosition(localPosition + newPosDelta);
}
 void Transform::SetLocalPositionDelta(float x, float y, float z) {
    SetLocalPosition(localPosition + CVector3(x, y, z));
}
 void Transform::SetLocalScale(const CVector3& newLocalScale)
 {
     localScale = newLocalScale;
 }


/// <summary>
/// 该函数设置的这个旋转矩阵是应用于父坐标系的
/// </summary>
/// <param name="newRotation"></param>
 void Transform::SetRotation(const CMatrix4& newRotation) {
    localRotation = newRotation;
    UpdateTransformFromLocal();
}
/// <summary>
/// lookDir是基于世界坐标系的向量
/// </summary>
/// <param name="lookDir"></param>
 void Transform::LookAt(const CVector3& lookDir) {

    CVector3 t = lookDir;
    CMatrix4 worldMatrix = lookDir.ToCMatrix();
    if (parent)
    {
        localRotation = parent->rotation.GetInverse() * worldMatrix;
    }
    else
    {
        localRotation = worldMatrix;
    }

    /*if (gameObject->name == "Car")
    {
        std::cout << localRotation.ToString();

    }*/
    UpdateTransformFromLocal();
}

void Transform::SetRotationDelta(const CMatrix4& newRotation) {
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

void Transform::SetQuaternion(CQuaternion quaternion)
{
    SetRotation(quaternion.ToCMatrix4());
}
void Transform::SetQuaternionDelta(CQuaternion quaternionDelta)
{
    SetQuaternion(quaternion + quaternionDelta);
}
void Transform::SetQuaternionDelta(float w, float x, float y, float z)
{
    SetQuaternion(quaternion + CQuaternion(w,x,y,z));
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
        CMatrix4 worldRot = newAnglesCopy.ToCMatrix();
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
void Transform::UpdateTransformFromLocal(bool updateColliderTransform) {
    if (parent) {
        // 计算世界坐标系变换
        rotation = parent->rotation * localRotation;
        position = parent->position + parent->rotation.vecMulVector3(localPosition);
    }
    else {
        rotation = localRotation;
        position = localPosition;
    }


    eulerAngles = rotation.ToEuler();
    quaternion = rotation.ToQuaternion();
    localEulerAngles = localRotation.ToEuler();
    UpdateChildrenTransform();
    UpdateFRU();

    if (updateColliderTransform)
    {
        UpdateColliderTransform();
    }

}

// 更新子物体变换
void Transform::UpdateChildrenTransform() {
    for (auto& child : children) {
        if (!child) continue;

        // 子物体的世界旋转 = 父旋转 * 子本地旋转
        child->rotation = rotation * child->localRotation;
        // 子物体的世界位置 = 父位置 + 父旋转后的本地位置
        child->position = position + rotation.vecMulVector3(child->localPosition);
        child->eulerAngles = child->rotation.ToEuler();
        // 递归更新子物体
        child->UpdateChildrenTransform();
    }
}

void Transform::UpdateRigidBodyTransform()
{
    auto rigidbody = gameObject->GetComponent<RigidBody>();
    if (rigidbody == nullptr)return;
    
    rigidbody->SetTransform(GetWorldTransformMatrix());

    for (auto child : gameObject->transform->children)
    {
        child->UpdateRigidBodyTransform();
    }
}

void Transform::UpdateColliderTransform()
{
    auto collider = gameObject->GetComponent<Collider>();
    if (collider != nullptr)
    {
        collider->SynchronizeTransform(GetWorldTransformMatrix());
    }

    for (auto child : gameObject->transform->children)
    {
        child->UpdateColliderTransform();
    }
}

void Transform::AddChild(Transform* child) {
    if (!child) return;

    children.push_back(child);
    child->parent = this;

    // 初始化子物体的本地坐标
    child->localPosition = rotation.GetInverse().vecMulVector3(child->position - position);
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

CMatrix4 Transform::GetWorldTransformMatrix()
{
    worldTransformMatrix = rotation;
    worldTransformMatrix.m03 = position.x;
    worldTransformMatrix.m13 = position.y;
    worldTransformMatrix.m23 = position.z;
    return worldTransformMatrix;
}
