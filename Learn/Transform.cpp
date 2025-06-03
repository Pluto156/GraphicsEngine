#include "stdafx.h"
#include "Transform.h"
ComponentType Transform::GetType()
{
    return ComponentType::Transform;
}
void Transform::SetPosition(const CVector3& newPos) {
    if (parent) {
        // ����������ת��Ϊ���ռ�ı�������
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

/// <summary>
/// �ú������õ������ת������Ӧ���ڸ�����ϵ��
/// </summary>
/// <param name="newRotation"></param>
 void Transform::SetRotation(const CMatrix4& newRotation) {
    localRotation = newRotation;
    UpdateTransformFromLocal();
}
/// <summary>
/// lookDir�ǻ�����������ϵ������
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
        // ��������ڸ�����ı�����ת
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
    // ���ƴ�ֱ�ӽǵķ�Χ�������������ȫ��ת
    CEuler newAnglesCopy = newAngles;
    if (newAnglesCopy.p > 89.0f) newAnglesCopy.p = 89.0f;
    if (newAnglesCopy.p < -89.0f) newAnglesCopy.p = -89.0f;
    if (parent) {
        // ��������ڸ�����ı���ŷ����
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

// �ӱ�������ϵ������������ϵ
void Transform::UpdateTransformFromLocal(bool updateColliderTransform) {
    if (parent) {
        // ������������ϵ�任
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

// ����������任
void Transform::UpdateChildrenTransform() {
    for (auto& child : children) {
        if (!child) continue;

        // �������������ת = ����ת * �ӱ�����ת
        child->rotation = rotation * child->localRotation;
        // �����������λ�� = ��λ�� + ����ת��ı���λ��
        child->position = position + rotation.vecMulVector3(child->localPosition);
        child->eulerAngles = child->rotation.ToEuler();
        // �ݹ����������
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

    // ��ʼ��������ı�������
    child->localPosition = rotation.GetInverse().vecMulVector3(child->position - position);
    child->localRotation = rotation.GetInverse() * child->rotation;
    child->localEulerAngles = child->localRotation.ToEuler();

    // �������������������
    child->UpdateTransformFromLocal();
}

void Transform::ApplyTransform()
{
    //Transform �任����
    glTranslatef(position.x, position.y, position.z);
    glRotatef(eulerAngles.h, 0, 1, 0); // �� Y ����ת
    glRotatef(eulerAngles.p, 1, 0, 0); // �� X ����ת
    glRotatef(eulerAngles.b, 0, 0, 1); // �� Z ����ת

    if (isShowLocalAxis) {
        // ���Ʊ�������ϵ
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ��X �ᣩ
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ��Y �ᣩ
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ��Z �ᣩ
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
