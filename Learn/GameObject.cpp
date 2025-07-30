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

GameObject* GameObject::Clone() const {
    CloneContext ctx;
    return Clone(ctx);
}
GameObject* GameObject::Clone(CloneContext& ctx) const {
    // 创建新 GameObject，复制基础属性
    GameObject* clone = new GameObject(name, transform->GetPosition(), transform->GetRotation(), transform->GetEulerAngles());

    // 注册当前对象的克隆映射，供后续修复指针引用用
    ctx.RegisterPointer(this, clone);
    ctx.RegisterPointer(transform, clone->transform); // 可选：transform 指针也注册一下

    // 克隆组件（每个组件必须实现 Clone(CloneContext&) 方法）
    for (Component* comp : components) {
        Component* clonedComp = comp->Clone(ctx);
        clone->AddComponentRaw(clonedComp);  // 注意不要触发 Awake/Start 等生命周期函数
    }

    // 克隆子对象
    for (Transform* child : transform->children) {
        GameObject* childGO = child->gameObject;
        GameObject* clonedChild = childGO->Clone(ctx); // 递归克隆
        clonedChild->transform->SetParent(clone->transform); // 建立父子关系
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