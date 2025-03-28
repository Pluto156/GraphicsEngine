#include "stdafx.h"
#include "Area.h"

// Area 构造函数，初始化区域的中心位置
Area::Area(std::string name,float x, float y, float z)
    : Transform(name,CVector(x,y,z)) {
}

// 获取区域内 Box 的数量
size_t Area::GetBoxCount() const {
    return boxes.size();
}

// 向区域添加一个 Box
void Area::AddBox(Box* box) {
    boxes.push_back(box);
    
    AddChild(box);  // 添加 Box 到子 Transform 列表
}

// 绘制该区域内的所有 Box
void Area::Draw() const {
    for (const auto box : boxes) {
        box->Draw();

    }
}

std::vector<Box*> Area::GetBoxes()
{
    return boxes;
}

