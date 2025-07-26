#include "stdafx.h"
#include "Stage.h"


// 绘制舞台中的所有区域
void Stage::Update() {

    auto& input = InputManager::Instance();
    if (input.GetKeyDown(KeyCode::L))
    {
        LightManager::Instance().EnableLighting = !LightManager::Instance().EnableLighting;
        LightManager::Instance().InitLighting();  // 切换光照时重新初始化
    }

    if (input.GetKeyDown(KeyCode::Semicolon))
    {
        DebugManager::Instance().EnableGizmos = !DebugManager::Instance().EnableGizmos;
    }
    StageDebug();
}



void Stage::IntersectWithRay(
    const CVector3& origin,
    const CVector3& direct,
    float length) const
{
    //float minDistance = FLT_MAX; // 初始值设置为最大浮动值
    //GameObject* closestShape = nullptr; // 用于存储最近的Shape 
    //PhysicsLit::Ray ray(origin,direct);

    //for (const auto area : gameObject->transform->children) {
    //    for (auto shape : area->children) {
    //        PhysicsLit::RayHitInfo rayHitInfo;
    //        Box* boxPtr = dynamic_cast<Box*>(shape->gameObject);
    //        if (boxPtr)
    //        {
    //            // 检测与射线的碰撞
    //            if (boxPtr->boxCollider->IntersectRay(ray, rayHitInfo)) {
    //                // 计算射线与交点的距离
    //                float distance = rayHitInfo.distance;

    //                // 如果当前碰撞的距离比最小距离更小，更新最近的 Box
    //                if (distance < minDistance) {
    //                    minDistance = distance;
    //                    closestShape = boxPtr; // 更新最近的 Box
    //                }
    //            }
    //        }
    //    }
    //}

    //// 如果找到最近的 box，则将其设置为选中状态
    //if (closestShape != nullptr) {
    //    closestShape->isSelect = !closestShape->isSelect;

    //    if (closestShape->isSelect)
    //    {
    //        curSelectShape = closestShape;

    //        //curSelectShape->rigidBody->AddForce(CVector3(0, 100, 0));
    //        //std::cout << curSelectBox->ToString();
    //    }
    //}
    //else
    //{
    //    curSelectShape = nullptr;
    //}

    //// 可以考虑是否需要重置其他 box 的 isSelect 为 false
    //// 如果是每次都要重新计算选择的 box，那么可以遍历并将其他 box 的 isSelect 设置为 false
    //for (const auto area : gameObject->transform->children) {
    //    for (auto shape : area->children) {
    //        Shape* boxPtr = dynamic_cast<Shape*>(shape->gameObject);
    //        if (boxPtr != closestShape) {
    //            boxPtr->isSelect = false;
    //        }
    //    }
    //}
}
//
//void Stage::processMouse(int button, int state, int x, int y)
//{
//    //射线检测
//    if (button == GLUT_LEFT_BUTTON)
//    {
//        isLeft = state == GLUT_DOWN;
//        if (state == GLUT_DOWN)
//        {
//            prevMouseX = x;
//            prevMouseY = y;
//            
//            float val;
//            double modelview[16], project[16], pos[3];
//            int viewport[4];
//            //glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//            glGetDoublev(GL_PROJECTION_MATRIX, project);
//            glGetIntegerv(GL_VIEWPORT, viewport);
//            y = viewport[3] - y;
//            glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
//            gluUnProject(x, y, val, modelViewMatrix, project, viewport, &pos[0], &pos[1], &pos[2]);
//
//            //printf("%d:%d\t(%d:%d)\t%f\t(%.2f,%.2f,%.2f)\n", button, state, x, y, val, pos[0], pos[1], pos[2]);
//            origin = camera->transform->position;
//            direct = CVector3(pos[0], pos[1], pos[2]) - camera->transform->position;
//            direct.Normalize();
//            IntersectWithRay(origin, direct, 100);
//        }
//    }
//    else if (button == GLUT_RIGHT_BUTTON)
//    {
//        isRight = state == GLUT_DOWN;
//    }
//}
//
//void Stage::processKeyboard(unsigned char key, int x, int y)
//{
//    if (key == '2')
//    {
//        isRotate = !isRotate;
//    }
//    else if (key == '3')
//    {
//        isBAnimation = !isBAnimation;
//    }
//    else if (key == '4')
//    {
//        isCAnimation = !isCAnimation;
//    }
//}
//
//void Stage::processSpecialKeys(int key, int x, int y)
//{
//    
//}
//void Stage::processMouseMotion(int x, int y)
//{
//    if (curSelectShape != nullptr)
//    {
//        if (curSelectShape->name[0] == 'B')
//        {
//            if (isLeft)
//            {
//                curSelectShape->transform->SetLocalPositionDelta(0, (prevMouseY - y) > 0 ? 0.1 : -0.1, 0);
//                prevMouseX = x;
//                prevMouseY = y;
//            }
//        }
//        else if (curSelectShape->name[0] == 'C'&& curSelectShape->name[1] == '_')
//        {
//            if (isRight)
//            {
//                curSelectShape->transform->SetRotationDelta((prevMouseX - x), 0, 0);
//            }
//            else if (isLeft)
//            {
//                curSelectShape->transform->SetLocalPositionDelta((prevMouseX - x) < 0 ? 0.1 : -0.1, 0, 0);
//                prevMouseX = x;
//                prevMouseY = y;
//            }
//        }
//    }
//}

void Stage::StageDebug()
{
    // 构建显示信息
    std::ostringstream oss;
    //CVector3 tpos = Car2->GetComponent<RigidBody>()->rigidBodyPrimitive->GetPosition();
    oss << "Camera Position: ("
        << camera->transform->position.x << ", "
        << camera->transform->position.y << ", "
        << camera->transform->position.z << ")\n"
        << "Camera ControlViewMode:(" << camera->ControlViewMode << ")\n"
        << "Camera EulerAngles(h,p,b): ("
        << camera->transform->eulerAngles.h << ", "
        << camera->transform->eulerAngles.p << ", "
        << camera->transform->eulerAngles.b << ")\n"
        << "Select Shape: ("
        << (curSelectShape ? curSelectShape->name : "null") << ")\n"
        << "Car Position: ("
        << Car->transform->position.x << ", "
        << Car->transform->position.y << ", "
        << Car->transform->position.z << ")\n";
        //<< "Car2 Position: ("
        //<< Car2->transform->position.x << ", "
        //<< Car2->transform->position.y << ", "
        //<< Car2->transform->position.z << ")\n"
        //<< tpos.x << ", "
        //<< tpos.y << ", "
        //<< tpos.z << ")\n";

    if (curSelectShape)
    {
        oss << "position: ("
            << curSelectShape->transform->position.ToString() << ")\n"
            << "eulerAngles: ("
            << curSelectShape->transform->eulerAngles.ToString() << ")\n"
            << "local position: ("
            << curSelectShape->transform->localPosition.ToString() << ")\n"
            << "local eulerAngles: ("
            << curSelectShape->transform->localEulerAngles.ToString() << ")\n"
            << "parent name: ("
            << (curSelectShape->transform->parent ? curSelectShape->transform->parent->gameObject->name : "null") << ")\n";
    
        if (curSelectShape->transform->parent!=nullptr&& curSelectShape->transform->parent->gameObject-> GetComponent<RigidBody>() != nullptr)
        {
            auto rigidbody = curSelectShape->transform->parent->gameObject->GetComponent<RigidBody>()->rigidBodyPrimitive;
            oss << "Velocity: ("
                << rigidbody->GetVelocity().ToString() << ")\n"
                << "AngularVelocity: ("
                << rigidbody->GetAngularVelocity().ToString() << ")\n"
                << "rigidbody Position: ("
                << rigidbody->GetPosition().ToString() << ")\n"
                << "rigidbody Rotation: ("
                << rigidbody->GetRotation().ToCMatrix4().ToEuler().ToString() << ")\n";
        }
    
    
    }

    // 绘制文本
    gameObject->infoFont.DrawString(oss.str());
    
}


