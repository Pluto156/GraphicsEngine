#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"

GameObject::GameObject(const std::string& name, const CVector3& position,
    const CMatrix4& rotation,
    const CEuler& eulerAngles, bool isShowLocalAxis) : name(name),
    components() 
{
    transform = AddComponent<Transform>(position, rotation, eulerAngles, isShowLocalAxis);
    transform->gameObject = this;
    infoFont.SetColor(1.0f, 1.0f, 0.5f);  // 淡黄色
    infoFont.SetPosition(10, 30);
}

//用于渲染帧更新
void GameObject::Update()
{
    if (isPendingDestroy)return;
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

    if (GetComponent<SphereCollider>() != nullptr)
    {
        float radius = GetComponent<SphereCollider>()->radius;
        glColor3f(0, 1.0f, 0);
        glLineWidth(2);
        glutWireSphere(radius, 16, 16);  // 使用 GLUT 绘制线框球
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

GameObject* GameObject::Clone()  const {
    // 新建一个 GameObject，复制名称和变换（可定制）
    GameObject* clone = new GameObject(name, transform->GetPosition(), transform->GetRotation(), transform->GetEulerAngles());

    // 克隆组件（注意：每个组件类型需支持深拷贝或自带 Clone 方法）
    for (auto comp : components) {
        // 伪代码：你需要为每个 Component 派生类实现 Clone()
        Component* cloned = comp->Clone();
        clone->AddComponentRaw(cloned); // 下面说明 AddComponentRaw 是什么
    }

    return clone;
}

// 用于组件克隆，跳过 Start 调用（避免多次初始化）
void GameObject::AddComponentRaw(Component* comp) {
    comp->gameObject = this;
    comp->transform = this->transform;
    components.push_back(comp);

    // 检查是否是 GameScript 类型
    using DecayedT = std::decay_t<decltype(*comp)>;
    constexpr bool is_script = std::is_base_of<GameScript, DecayedT>::value;
    AssignIfGameScript(comp, std::bool_constant<is_script>{});
}