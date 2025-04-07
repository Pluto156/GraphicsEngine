#pragma once
#include "CVector.h"
#include "CMatrix.h"
#include "CEuler.h"
#include "CglFont.h"
#include "OBB.h"

#include <vector>
#include <string>
class Transform {
public:
    std::string name;
    CVector position;   // 世界坐标
    CMatrix rotation;   // 世界旋转矩阵
    CEuler eulerAngles; // 世界欧拉角（绕 X、Y、Z 轴的旋转角度）

    CVector localPosition;    // 相对于父 Transform 的位置
    CMatrix localRotation;    // 相对于父 Transform 的旋转矩阵
    CEuler localEulerAngles;  // 相对于父 Transform 的欧拉角

    CVector Up;
    CVector Forward;
    CVector Right;

    Transform* parent = nullptr;     // 父对象
    std::vector<Transform*> children; // 子对象列表


    bool isShowLocalAxis;

    // Debug
    CglFont infoFont;
    char cameraInfo[128];

    Transform(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(), bool isShowLocalAxis = false)
        : name(name), position(position), rotation(rotation),
        eulerAngles(eulerAngles),localPosition(position),
        localRotation(rotation), localEulerAngles(eulerAngles),isShowLocalAxis(isShowLocalAxis)
    {
        UpdateTransformFromLocal(); // 初始化时确保数据同步
        infoFont.SetColor(1.0f, 1.0f, 0.5f);  // 淡黄色
        infoFont.SetPosition(10, 30);
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
    virtual void SetPositionDelta(float x, float y, float z) {
        SetPosition(position + CVector(x, y, z));
    }
    virtual void SetLocalPosition(const CVector& newPos) {
        localPosition = newPos;
        UpdateTransformFromLocal();
    }
    virtual void SetLocalPositionDelta(const CVector& newPosDelta) {
        SetLocalPosition(localPosition + newPosDelta);
    }
    virtual void SetLocalPositionDelta(float x, float y, float z) {
        SetLocalPosition(localPosition + CVector(x, y, z));
    }
    
    /// <summary>
    /// 该函数设置的这个旋转矩阵是应用于父坐标系的
    /// </summary>
    /// <param name="newRotation"></param>
    virtual void SetRotation(const CMatrix& newRotation) {
        localRotation = newRotation;
        UpdateTransformFromLocal();
    }
    /// <summary>
    /// lookDir是基于世界坐标系的向量
    /// </summary>
    /// <param name="lookDir"></param>
    virtual void LookAt(const CVector& lookDir) {

        CVector t = lookDir;
        CMatrix worldMatrix = lookDir.ToCMatrix();
        if (parent)
        {
            localRotation = parent->rotation.GetInverse()* worldMatrix;
        }
        else
        {
            localRotation = worldMatrix;
        }
        UpdateTransformFromLocal();
    }

    virtual void SetRotationDelta(const CMatrix& newRotation) {
        if (parent) {
            // 计算相对于父物体的本地旋转
            localRotation = parent->rotation * newRotation;
        }
        else {
            localRotation = localRotation*newRotation;
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

    virtual void SetEulerAngles(float h, float p, float b)
    {
        SetEulerAngles(CEuler(h, p, b));
    }

    virtual void SetEulerAngles(const CEuler& newAngles) {
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

    virtual void SetEulerAnglesDelta(CEuler eulerAnglesDelta)
    {
        SetEulerAngles(eulerAngles+ eulerAnglesDelta);
    }

    virtual void SetEulerAnglesDelta(float h,float p,float b)
    {
        SetEulerAngles(eulerAngles + CEuler(h,p,b));
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
        localEulerAngles = localRotation.ToEuler();
        UpdateFRU();
        UpdateSomeDataAfterTransformChange();
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
            child->UpdateSomeDataAfterTransformChange();
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

    //用于渲染帧更新
    virtual void Update()
    {
        //Transform 变换部分
        glPushMatrix();  // 保存当前矩阵状态
        if (parent)
        {
            glTranslatef(localPosition.x, localPosition.y, localPosition.z);
            glRotatef(localEulerAngles.h, 0, 1, 0); // 绕 Y 轴旋转
            glRotatef(localEulerAngles.p, 1, 0, 0); // 绕 X 轴旋转
            glRotatef(localEulerAngles.b, 0, 0, 1); // 绕 Z 轴旋转
        }
        else
        {
            glTranslatef(position.x, position.y, position.z);
            glRotatef(eulerAngles.h, 0, 1, 0); // 绕 Y 轴旋转
            glRotatef(eulerAngles.p, 1, 0, 0); // 绕 X 轴旋转
            glRotatef(eulerAngles.b, 0, 0, 1); // 绕 Z 轴旋转
        }

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
        Draw();
        glPopMatrix();  // 恢复矩阵状态
    }

    virtual void Draw()
    {

    }
    //变换后更新数据
    virtual void UpdateSomeDataAfterTransformChange()
    {
        
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
