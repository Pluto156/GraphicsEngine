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
    CVector position;   // ��������
    CMatrix rotation;   // ������ת����
    CEuler eulerAngles; // ����ŷ���ǣ��� X��Y��Z �����ת�Ƕȣ�
    bool isShowLocalAxis;
    OBB obb;            // �󶨵������Χ��

    CVector localPosition;    // ����ڸ� Transform ��λ��
    CMatrix localRotation;    // ����ڸ� Transform ����ת����
    CEuler localEulerAngles;  // ����ڸ� Transform ��ŷ����

    Transform* parent = nullptr;     // ������
    std::vector<Transform*> children; // �Ӷ����б�

    Transform(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false)
        : name(name), position(position), rotation(rotation),
        eulerAngles(eulerAngles), obb(position, CVector(), rotation),
        isShowLocalAxis(isShowLocalAxis), localPosition(position),
        localRotation(rotation), localEulerAngles(eulerAngles)
    {
        UpdateTransformFromLocal(); // ��ʼ��ʱȷ������ͬ��
    }

    virtual void SetPosition(const CVector& newPos) {
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

    virtual void SetRotation(const CMatrix& newRotation) {
        if (parent) {
            // ��������ڸ�����ı�����ת
            localRotation = parent->rotation.GetInverse() * newRotation;
        }
        else {
            localRotation = newRotation;
        }
        UpdateTransformFromLocal();
    }

    virtual void SetEulerAngles(const CEuler& newAngles) {
        if (parent) {
            // ��������ڸ�����ı���ŷ����
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

    // �ӱ�������ϵ������������ϵ
    void UpdateTransformFromLocal() {
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

        obb.Update(position, rotation);
        UpdateChildrenTransform();
    }

    // ����������任
    void UpdateChildrenTransform() {
        for (auto& child : children) {
            if (!child) continue;

            // �������������ת = ����ת * �ӱ�����ת
            child->rotation = rotation * child->localRotation;
            // �����������λ�� = ��λ�� + ����ת��ı���λ��
            child->position = position + rotation.vecMul(child->localPosition);
            child->eulerAngles = child->rotation.ToEuler();
            child->obb.Update(child->position, child->rotation);

            // �ݹ����������
            child->UpdateChildrenTransform();
        }
    }

    void AddChild(Transform* child) {
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

    // ��ȡλ��
    virtual CVector GetPosition() const {
        return position;
    }

    // ��ȡ��ת����
    virtual CMatrix GetRotation() const {
        return rotation;
    }

    // ��ȡŷ����
    virtual CEuler GetEulerAngles() const {
        return eulerAngles;
    }

    // Ӧ�ñ任������ƽ�ƺ���ת��
    virtual void Draw() const {
        glTranslatef(position.x, position.y, position.z);
        glRotatef(eulerAngles.h, 0, 1, 0); // �� Y ����ת
        glRotatef(eulerAngles.p, 1, 0, 0); // �� X ����ת
        glRotatef(eulerAngles.b, 0, 0, 1); // �� Z ����ת

        if (isShowLocalAxis) {
            // ���Ʊ�������ϵ
            glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ��X �ᣩ
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(10.0f, 0.0f, 0.0f);
            glEnd();

            glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ��Y �ᣩ
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 10.0f, 0.0f);
            glEnd();

            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ��Z �ᣩ
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 10.0f);
            glEnd();
        }
    }
    // ��ȡ����λ��
    CVector GetLocalPosition() const {
        return localPosition;
    }

    // ��ȡ������ת����
    CMatrix GetLocalRotation() const {
        return localRotation;
    }

    // ��ȡ����ŷ����
    CEuler GetLocalEulerAngles() const {
        return localEulerAngles;
    }

    virtual ~Transform() {
        for (auto& child : children) {
            if (child) delete child;
        }
    }
};
