#include "stdafx.h"
#include "GameObject.h"
//ÓÃÓÚäÖÈ¾Ö¡¸üÐÂ
void GameObject::Update()
{
    glPushMatrix();  
    transform->ApplyTransform();
    Draw();
    glPopMatrix();  // »Ö¸´¾ØÕó×´Ì¬

    for (auto comp : components) {
        comp->Update();
    }
}

void GameObject::AddChild(GameObject* child)
{
    this->transform->AddChild(child->transform);
}