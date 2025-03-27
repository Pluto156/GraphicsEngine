#include "stdafx.h"
#include "Stage.h"
Stage::Stage(std::string name) :Transform(name,CVector()) {}
// Stage 构造函数，初始化各区域
Stage::Stage(std::string name,float posx, float posy, float posz) :Transform(name,CVector(posx,posy,posz)) {
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
                    closestBox = box; // 更新最近的 Box
                }
            }
        }
    }

    // 如果找到最近的 box，则将其设置为选中状态
    if (closestBox != nullptr) {
        closestBox->isSelect = !closestBox->isSelect;

        if (closestBox->isSelect)
        {
            std::cout << closestBox->ToString();
        }
    }

    // 可以考虑是否需要重置其他 box 的 isSelect 为 false
    // 如果是每次都要重新计算选择的 box，那么可以遍历并将其他 box 的 isSelect 设置为 false
    for (const auto area : Areas) {
        for (auto box : area->GetBoxes()) {
            if (box != closestBox) {
                box->isSelect = false;
            }
        }
    }
}

