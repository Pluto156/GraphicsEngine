#include "stdafx.h"
#include "GameObject.h"
//������Ⱦ֡����
void GameObject::Update()
{
    glPushMatrix();  
    transform->ApplyTransform();
    Draw();
    glPopMatrix();  // �ָ�����״̬

    for (auto comp : components) {
        comp->Update();
    }
}

void GameObject::AddChild(GameObject* child)
{
    this->transform->AddChild(child->transform);
}