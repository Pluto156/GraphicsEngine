#include "stdafx.h"
#include "CharacterController.h"

void CharacterController::processKeyboard(unsigned char key, int x, int y)
{

}
void CharacterController::processSpecialKeys(int key, int x, int y)
{
    Transform* trans = gameObject->transform;
    PhysicsLit::RigidBodyPrimitive* rigidBody = trans->gameObject->GetComponent<RigidBody>()->rigidBodyPrimitive;
    //小车控制
    if (key == GLUT_KEY_UP)
    {
        rigidBody->SetPosition(trans->position + CVector3(0, 0, -0.1));
        rigidBody->SetRotation(CVector3(0, 0, -1).ToCMatrix().ToQuaternion());
        //std::cout << "GLUT_KEY_UP" << CVector3(0, 0, -1).ToCMatrix().ToQuaternion().ToCMatrix4().ToEuler().ToString() << std::endl;


        //transform->children[6]->SetLocalPositionDelta(0, 0, -0.1);
        //transform->children[6]->LookAt(-transform->Forward);

    }
    if (key == GLUT_KEY_DOWN)
    {
        rigidBody->SetPosition(trans->position + CVector3(0, 0, 0.1));
        rigidBody->SetRotation(CVector3(0, 0, 1).ToCMatrix().ToQuaternion());
        //std::cout << "GLUT_KEY_DOWN" << CVector3(0, 0, 1).ToCMatrix().ToQuaternion().ToCMatrix4().ToEuler().ToString() << std::endl;

        //transform->children[6]->SetLocalPositionDelta(0, 0, 0.1);
        //transform->children[6]->LookAt(transform->Forward);

    }
    if (key == GLUT_KEY_LEFT)
    {
        rigidBody->SetPosition(trans->position + CVector3(-0.1, 0, 0));
        rigidBody->SetRotation(CVector3(-1, 0, 0).ToCMatrix().ToQuaternion());
        //std::cout << "GLUT_KEY_LEFT" << CVector3(-1, 0, 0).ToCMatrix().ToQuaternion().ToCMatrix4().ToEuler().ToString() << std::endl;

        //transform->children[6]->SetLocalPositionDelta(-0.1, 0, 0);
        //transform->children[6]->LookAt(-transform->Right);

    }
    if (key == GLUT_KEY_RIGHT)
    {
        rigidBody->SetPosition(trans->position + CVector3(0.1, 0, 0));
        rigidBody->SetRotation(CVector3(1, 0, 0).ToCMatrix().ToQuaternion());
        //std::cout << "GLUT_KEY_RIGHT" << CVector3(1, 0, 0).ToCMatrix().ToQuaternion().ToCMatrix4().ToEuler().ToString() << std::endl;

        //transform->children[6]->SetLocalPositionDelta(0.1, 0, 0);
        //transform->children[6]->LookAt(transform->Right);
    }

}
void CharacterController::processMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            std::cout << "GLUT_LEFT_BUTTON " <<gameObject->transform->position.ToString() <<std::endl;

            GameObject* Sphere = ShapeFactory::CreateSphere("Sphere"+ std::to_string(cnt++), 0.2, gameObject->transform->position+gameObject->transform->Forward);

            auto rigidBody4 = Sphere->AddComponent<RigidBody>();
            rigidBody4->rigidBodyPrimitive->SetMass(1000);
            auto sphereCollider = Sphere->GetComponent<SphereCollider>();
            sphereCollider->mFriction = 10;
            sphereCollider->mBounciness = 0.5;
            sphereCollider->SynchronizeData();
            sphereCollider->mCollider->rigidBodyPrimitive = rigidBody4->rigidBodyPrimitive;
            sphereCollider->mCollider->isTrigger = true;
            sphereCollider->mCollider->SetLayer(PhysicsLit::Layer::BULLET, (1 << PhysicsLit::Layer::ENEMY));

            rigidBody4->rigidBodyPrimitive->mCollisionVolume = sphereCollider->mCollider;
            rigidBody4->rigidBodyPrimitive->SetInertiaTensor(sphereCollider->mCollider->GetInertiaTensor(rigidBody4->rigidBodyPrimitive->GetMass()));
            rigidBody4->rigidBodyPrimitive->AddForceGenerator(new PhysicsLit::ForceGravity(gameObject->transform->Forward));
            

            Sphere->transform->UpdateColliderTransform();
            Sphere->AddComponent<Bullet>();
            PhysicsLit::PhysicsManager::Instance().AddGameObject(Sphere);


        }
    }
}
void CharacterController::processMouseMotion(int x, int y)
{

}

void CharacterController::OnColliderEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderEnter " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive)<< std::endl;

}
void CharacterController::OnColliderStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderStay " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

}
void CharacterController::OnColliderExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderExit " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

}

void CharacterController::OnTriggerEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void CharacterController::OnTriggerStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void CharacterController::OnTriggerExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}