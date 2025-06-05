#include "stdafx.h"
#include "CharacterController.h"
ComponentType CharacterController::GetType()
{
    return ComponentType::RigidBody;
}

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

}
void CharacterController::processMouseMotion(int x, int y)
{

}