#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"


GameObject::GameObject(const std::string& name, const CVector3& position,
    const CMatrix4& rotation,
    const CEuler& eulerAngles, bool isShowLocalAxis) : name(name),
    transform(new Transform(position, rotation, eulerAngles, isShowLocalAxis)),
    components() // 👈 明确初始化
{
    GameObjectManager::Instance().Register(this);
    transform = AddComponent<Transform>(position, rotation, eulerAngles, isShowLocalAxis);
    transform->gameObject = this;
    infoFont.SetColor(1.0f, 1.0f, 0.5f);  // 淡黄色
    infoFont.SetPosition(10, 30);
}

//用于渲染帧更新
void GameObject::Update()
{
    glPushMatrix();  
    transform->ApplyTransform();
    Draw();
    glPopMatrix();  // 恢复矩阵状态
    for (auto comp : components) {
        comp->Update();
    }
}

void GameObject::Draw()
{
    if (GetComponent<RigidBody>() != nullptr && GetComponent<BoxCollider>() != nullptr)
    {
        glBegin(GL_LINES);

        CVector3 BoxColliderHalfSize = GetComponent<BoxCollider>()->halfSizes;
        auto m_width = BoxColliderHalfSize.x * 2;
        auto m_height = BoxColliderHalfSize.y * 2;
        auto m_depth = BoxColliderHalfSize.z * 2;


        glColor3f(0, 1, 0);

        // 前面
        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

        // 后面
        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

        // 上面
        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

        // 下面
        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

        // 左面
        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

        glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

        glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

        // 右面
        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

        glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
        glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
        glEnd();
    }

    if (GetComponent<MeshRenderer>() != nullptr)
    {
        glColor3f(1, 1, 1);
        GetComponent<MeshRenderer>()->Draw();
    }

}

void GameObject::AddChild(GameObject* child)
{
    this->transform->AddChild(child->transform);
}