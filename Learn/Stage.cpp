#include "stdafx.h"
#include "Stage.h"
Stage::Stage(std::string name) :Transform(name,CVector()) {}
// Stage ���캯������ʼ��������
Stage::Stage(std::string name,float posx, float posy, float posz) :Transform(name,CVector(posx,posy,posz)) {
}

void Stage::AddArea(Area* area)
{
    this->Areas.push_back(area);
    AddChild(area);  
}

// ������̨�е���������
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
    float minDistance = FLT_MAX; // ��ʼֵ����Ϊ��󸡶�ֵ
    Box* closestBox = nullptr; // ���ڴ洢����� Box

    for (const auto area : Areas) {
        for (auto box : area->GetBoxes()) {
            PointCollision intersection;

            // ��������ߵ���ײ
            if (box->obb.IntersectWithRayAndOBB(origin, direct, length, intersection) > 0) {
                // ���������뽻��ľ���
                float distance = (intersection.closestPoint - origin).len();

                // �����ǰ��ײ�ľ������С�����С����������� Box
                if (distance < minDistance) {
                    minDistance = distance;
                    closestBox = box; // ��������� Box
                }
            }
        }
    }

    // ����ҵ������ box����������Ϊѡ��״̬
    if (closestBox != nullptr) {
        closestBox->isSelect = !closestBox->isSelect;

        if (closestBox->isSelect)
        {
            std::cout << closestBox->ToString();
        }
    }

    // ���Կ����Ƿ���Ҫ�������� box �� isSelect Ϊ false
    // �����ÿ�ζ�Ҫ���¼���ѡ��� box����ô���Ա����������� box �� isSelect ����Ϊ false
    for (const auto area : Areas) {
        for (auto box : area->GetBoxes()) {
            if (box != closestBox) {
                box->isSelect = false;
            }
        }
    }
}

