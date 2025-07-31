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
    GameObject* clone = GameObjectManager::Instance().Instantiate(name, transform->GetPosition(), transform->GetRotation(), transform->GetEulerAngles());

    ctx.RegisterPointer(this, clone);
    ctx.RegisterPointer(transform, clone->transform);

    // 第一步：只创建实例并注册，不拷贝字段，不调用 PostClone
    std::vector<Component*> clonedComponents;
    for (Component* comp : components) {

        const TypeInfo* typeInfo = comp->GetTypeInfo();
        Component* clonedComp;
        if (comp->GetTypeInfo() == TypeInfo::Get<Transform>()) {
            clonedComp = clone->transform;
        }
        else
        {
            clonedComp = static_cast<Component*>(typeInfo->creator());
            clone->AddComponentRaw(clonedComp);

        }
        ctx.RegisterPointer(comp, clonedComp);
        clonedComponents.push_back(clonedComp);
    }

    // 第二步：拷贝字段（不调用 PostClone）
    for (size_t i = 0; i < components.size(); ++i) {
        Component* src = components[i];
        Component* dst = clonedComponents[i];

        const TypeInfo* typeInfo = src->GetTypeInfo();
        const TypeInfo* current = typeInfo;
        while (current) {
            for (const auto& field : current->fields) {
                void* dstField = reinterpret_cast<char*>(dst) + field.offset;
                const void* srcField = reinterpret_cast<const char*>(src) + field.offset;

                if (field.copyType == FieldCopyType::RawCopy) {
                    std::memcpy(dstField, srcField, field.size);
                }
                else if (field.copyType == FieldCopyType::Custom && field.customCopy) {
                    field.customCopy(dst, src, ctx);
                }
            }
            current = current->base;
        }
    }

    // 第三步：统一调用 PostClone（此时所有组件都已克隆并注册）
    for (Component* comp : clonedComponents) {
        comp->PostClone(ctx);
    }

    // 克隆子对象
    for (Transform* child : transform->children) {
        GameObject* childGO = child->gameObject;
        GameObject* clonedChild = childGO->Clone(ctx);
        clonedChild->transform->SetParent(clone->transform);
    }

    return clone;
}



// 用于组件克隆，跳过 Start 调用（避免多次初始化）
void GameObject::AddComponentRaw(Component* comp) {
    comp->gameObject = this;
    comp->transform = this->transform;
    components.push_back(comp);

    if (comp->IsGameScript()) {
        AssignIfGameScript(comp, std::true_type{});
    }
    else {
        AssignIfGameScript(comp, std::false_type{});
    }
}