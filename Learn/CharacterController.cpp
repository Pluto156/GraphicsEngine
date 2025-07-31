#include "stdafx.h"
#include "CharacterController.h"


void CharacterController::Update()
{
    auto& input = InputManager::Instance();
    Transform* trans = gameObject->transform;
    PhysicsLit::RigidBodyPrimitive* rigidBody = trans->gameObject->GetComponent<RigidBody>()->rigidBodyPrimitive;
    //小车控制
    if (input.GetKey(UpKey))
    {
        rigidBody->SetPosition(trans->position + CVector3(0, 0, -0.1));
        rigidBody->SetRotation(CVector3(0, 0, 1).ToCMatrix().ToQuaternion());

    }
    if (input.GetKey(DownKey))
    {
        rigidBody->SetPosition(trans->position + CVector3(0, 0, 0.1));
        rigidBody->SetRotation(CVector3(0, 0, -1).ToCMatrix().ToQuaternion());
    }
    if (input.GetKey(LeftKey))
    {
        rigidBody->SetPosition(trans->position + CVector3(-0.1, 0, 0));
        rigidBody->SetRotation(CVector3(1, 0, 0).ToCMatrix().ToQuaternion());
    }
    if (input.GetKey(RightKey))
    {
        rigidBody->SetPosition(trans->position + CVector3(0.1, 0, 0));
        rigidBody->SetRotation(CVector3(-1, 0, 0).ToCMatrix().ToQuaternion());
    }

    if (input.GetKeyDown(FireKey))
    {
        //GameObject* Sphere = GameObjectManager::Instance().Instantiate("Sphere" + std::to_string(cnt++), gameObject->transform->position + gameObject->transform->Forward);
        //Sphere->AddComponent<MeshFilter>()->SetPrimitive(PrimitiveType::Sphere, 0.2f, 32, 16);
        //Sphere->AddComponent<MeshRenderer>();

        //auto rigidBody4 = Sphere->AddComponent<RigidBody>();
        //rigidBody4->rigidBodyPrimitive->SetMass(1);
        //auto sphereCollider = Sphere->AddComponent<SphereCollider>(0.2);
        //sphereCollider->mFriction = 10;
        //sphereCollider->mBounciness = 0.5;
        //sphereCollider->SynchronizeData();
        //sphereCollider->mCollider->rigidBodyPrimitive = rigidBody4->rigidBodyPrimitive;
        //sphereCollider->mCollider->isTrigger = true;
        //sphereCollider->mCollider->SetLayer(PhysicsLit::Layer::BULLET);

        //rigidBody4->rigidBodyPrimitive->mCollisionVolume = sphereCollider->mCollider;
        //rigidBody4->rigidBodyPrimitive->SetInertiaTensor(sphereCollider->mCollider->GetInertiaTensor(rigidBody4->rigidBodyPrimitive->GetMass()));
        ////rigidBody4->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(gameObject->transform->Forward));
        //rigidBody4->rigidBodyPrimitive->SetGameObjectName("Sphere" + std::to_string(cnt));

        //Sphere->transform->UpdateColliderTransform();
        //Bullet* bullet = Sphere->AddComponent<Bullet>();
        //bullet->StartPos = Sphere->transform->position;
        //bullet->MoveDir = gameObject->transform->Forward;

        GameObject* Sphere = GameObjectManager::Instance().Clone(BullutPrefab);
        Sphere->name = "Sphere" + std::to_string(cnt++);
        Sphere->transform->SetPosition(gameObject->transform->position + gameObject->transform->Forward);
        Bullet* bullet = Sphere->GetComponent<Bullet>();
        bullet->StartPos = Sphere->transform->position;
        bullet->MoveDir = gameObject->transform->Forward;
        PhysicsLit::PhysicsManager::Instance().AddGameObject(Sphere);

    }
}


void CharacterController::BindUpKey(KeyCode key)
{
    this->UpKey = key;
}
void CharacterController::BindDownKey(KeyCode key)
{
    this->DownKey = key;
}
void CharacterController::BindLeftKey(KeyCode key)
{
    this->LeftKey = key;
}
void CharacterController::BindRightKey(KeyCode key)
{
    this->RightKey = key;
}
void CharacterController::BindFireKey(KeyCode key)
{
    this->FireKey = key;
}




