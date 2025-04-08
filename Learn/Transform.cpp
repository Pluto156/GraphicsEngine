#include "stdafx.h"
#include "Transform.h"
void Transform::SetPosition(const CVector& newPos) {
    if (parent) {
        // ����������ת��Ϊ���ռ�ı�������
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
/// �ú������õ������ת������Ӧ���ڸ�����ϵ��
/// </summary>
/// <param name="newRotation"></param>
 void Transform::SetRotation(const CMatrix& newRotation) {
    localRotation = newRotation;
    UpdateTransformFromLocal();
}
/// <summary>
/// lookDir�ǻ�����������ϵ������
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

// �ӱ�������ϵ������������ϵ
void Transform::UpdateTransformFromLocal() {
    if (parent) {
        // ������������ϵ�任
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

// ����������任
void Transform::UpdateChildrenTransform() {
    for (auto& child : children) {
        if (!child) continue;

        // �������������ת = ����ת * �ӱ�����ת
        child->rotation = rotation * child->localRotation;
        // �����������λ�� = ��λ�� + ����ת��ı���λ��
        child->position = position + rotation.vecMul(child->localPosition);
        child->eulerAngles = child->rotation.ToEuler();
        // �ݹ����������
        child->UpdateChildrenTransform();
    }
}

void Transform::AddChild(Transform* child) {
    if (!child) return;

    children.push_back(child);
    child->parent = this;

    // ��ʼ��������ı�������
    child->localPosition = rotation.GetInverse().vecMul(child->position - position);
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
