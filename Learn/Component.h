#pragma once
enum class ComponentType
{
    Component,
    Transform,
    RigidBody,
    Collider,
    BoxCollider,
    PlaneCollider,
    MeshRenderer
};
class GameObject;
class Component {
public:
    GameObject* gameObject = nullptr;  // ����������������
    static ComponentType GetType();

    virtual void Start() {}   // ��ʼ��ʱ����
    virtual void Update() {}  // ÿ֡����
    virtual ~Component() = default;

};



