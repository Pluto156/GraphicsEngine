#include "stdafx.h"
#include "Stage.h"
Stage::Stage(std::string name) :Transform(name,CVector()),curSelectBox(nullptr) {}
// Stage 构造函数，初始化各区域
Stage::Stage(std::string name,float posx, float posy, float posz) :Transform(name,CVector(posx,posy,posz)), curSelectBox(nullptr) {
}

void Stage::AddArea(Area* area)
{
    this->Areas.push_back(area);
    AddChild(area);  
}

// 绘制舞台中的所有区域
void Stage::Draw() const {
    for (const auto area : Areas) {
        area->Draw();
    }
}
void Stage::Update()
{
    if (isRotate)
    {
        angle = (angle + 0.1) / 1;
        angle = angle >= 360 ? angle - 360 : angle;
        SetRotationDelta(CMatrix::CreateRotationMatrix(angle, CVector::Up()));
    }


    if (isBAnimation)
    {
        Area* B1 = Areas[1];
        Area* B2 = Areas[2];
        for (int i = 0; i < 14; ++i) {
            if (i < 4)
            {
                for (int j = 0; j < 15; ++j) {
                    // 计算每一排盒子在 y 方向上的波浪位移
                    // sin(BAnimationAngle) 控制波浪的高度，每一排的波浪同步
                    float waveHeight = sin((BAnimationAngle + i * 30.0f) * M_PI / 180) / 100.0f; // 每排不同相位

                    // 设置所有盒子在 y 方向上的升降
                    B2->boxes[i * 15 + j]->SetPositionDelta(0, waveHeight, 0);
                }
            }
            else
            {
                for (int j = 0; j < 9; ++j) {
                    // 计算每一排盒子在 y 方向上的波浪位移
                    // sin(BAnimationAngle) 控制波浪的高度，每一排的波浪同步
                    float waveHeight = sin((BAnimationAngle + i * 30.0f) * M_PI / 180) / 100.0f; // 每排不同相位

                    // 设置所有盒子在 y 方向上的升降
                    B1->boxes[(i-4) * 9 + j]->SetPositionDelta(0, waveHeight, 0);
                }
            }
            
        }

        // 增加动画角度，模拟波浪的传播
        BAnimationAngle += 1;
        BAnimationAngle = BAnimationAngle > 360 ? BAnimationAngle - 360 : BAnimationAngle;
    }

    if (isCAnimation)
    {
        Area* C = Areas[3];

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                C->boxes[i * 4 + j]->SetRotationDelta(CAnimationAngle, 0, 0);
                C->boxes[i * 4 + j]->SetPositionDelta(sin((CAnimationAngle+90) * M_PI / 180)/100, 0, 0);
            }
        }
        CAnimationAngle += 0.3;
        CAnimationAngle = CAnimationAngle > 360 ? CAnimationAngle - 360 : CAnimationAngle;
    }

    Draw();
}


void Stage::IntersectWithRay(
    const CVector& origin,
    const CVector& direct,
    float length) const
{
    float minDistance = FLT_MAX; // 初始值设置为最大浮动值
    Box* closestBox = nullptr; // 用于存储最近的 Box

    for (const auto area : Areas) {
        for (auto box : area->GetBoxes()) {
            PointCollision intersection;

            // 检测与射线的碰撞
            if (box->obb.IntersectWithRayAndOBB(origin, direct, length, intersection) > 0) {
                // 计算射线与交点的距离
                float distance = (intersection.closestPoint - origin).len();

                // 如果当前碰撞的距离比最小距离更小，更新最近的 Box
                if (distance < minDistance) {
                    minDistance = distance;
                    closestBox = box.get(); // 更新最近的 Box
                }
            }
        }
    }

    // 如果找到最近的 box，则将其设置为选中状态
    if (closestBox != nullptr) {
        closestBox->isSelect = !closestBox->isSelect;

        if (closestBox->isSelect)
        {
            curSelectBox = closestBox;
            //std::cout << curSelectBox->ToString();
        }
    }

    // 可以考虑是否需要重置其他 box 的 isSelect 为 false
    // 如果是每次都要重新计算选择的 box，那么可以遍历并将其他 box 的 isSelect 设置为 false
    for (const auto area : Areas) {
        for (auto box : area->GetBoxes()) {
            if (box.get() != closestBox) {
                box->isSelect = false;
            }
        }
    }
}

void Stage::processMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        isLeft = state == GLUT_DOWN;
        if (state == GLUT_DOWN)
        {
            prevMouseX = x;
            prevMouseY = y;
            
            float val;
            double modelview[16], project[16], pos[3];
            int viewport[4];
            //glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, project);
            glGetIntegerv(GL_VIEWPORT, viewport);
            y = viewport[3] - y;
            glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
            gluUnProject(x, y, val, modelViewMatrix, project, viewport, &pos[0], &pos[1], &pos[2]);

            //printf("%d:%d\t(%d:%d)\t%f\t(%.2f,%.2f,%.2f)\n", button, state, x, y, val, pos[0], pos[1], pos[2]);
            origin = camera->position;
            direct = CVector(pos[0], pos[1], pos[2]) - camera->position;
            direct.Normalize();
            PointCollision PointCollision;
            IntersectWithRay(origin, direct, 100);
        }
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        isRight = state == GLUT_DOWN;
    }
}

void Stage::processKeyboard(unsigned char key, int x, int y)
{
    if (key == '2')
    {
        isRotate = !isRotate;
    }
    else if (key == '3')
    {
        isBAnimation = !isBAnimation;
    }
    else if (key == '4')
    {
        isCAnimation = !isCAnimation;
    }
}
void Stage::processMouseMotion(int x, int y)
{
    if (curSelectBox != nullptr)
    {
        if (curSelectBox->name[0] == 'B')
        {
            if (isLeft)
            {
                curSelectBox->SetPositionDelta(0, (prevMouseY - y) > 0 ? 0.1 : -0.1, 0);
                prevMouseX = x;
                prevMouseY = y;
            }
        }
        else if (curSelectBox->name[0] == 'C')
        {
            if (isRight)
            {
                curSelectBox->SetRotationDelta((prevMouseX - x), 0, 0);
            }
            else if (isLeft)
            {
                curSelectBox->SetPositionDelta((prevMouseX - x) < 0 ? 0.1 : -0.1, 0, 0);
                prevMouseX = x;
                prevMouseY = y;
            }
        }

    }

}


