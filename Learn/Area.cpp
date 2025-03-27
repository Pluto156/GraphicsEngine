#include "stdafx.h"
#include "Area.h"

// Area ���캯������ʼ�����������λ��
Area::Area(std::string name,float x, float y, float z)
    : Transform(name,CVector(x,y,z)) {
}

// ��ȡ������ Box ������
size_t Area::GetBoxCount() const {
    return boxes.size();
}

// ���������һ�� Box
void Area::AddBox(Box* box) {
    boxes.push_back(box);
    
    AddChild(box);  // ��� Box ���� Transform �б�
}

// ���Ƹ������ڵ����� Box
void Area::Draw() const {
    for (const auto box : boxes) {
        box->Draw();

    }
}

std::vector<Box*> Area::GetBoxes()
{
    return boxes;
}

