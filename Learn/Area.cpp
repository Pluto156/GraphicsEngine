#include "stdafx.h"
#include "Area.h"

// Area 构造函数，初始化区域的中心位置
Area::Area(std::string name,float x, float y, float z)
    : Transform(name,CVector(x,y,z)) {
}

// 获取区域内 Box 的数量
size_t Area::GetShapeCount() const {
    return shapes.size();
}

// 向区域添加一个 Box
void Area::AddShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
    AddChild(shape.get());  // 添加 Box 到子 Transform 列表
}

// 绘制该区域内的所有 Box
void Area::Draw() {
    for (const auto &shape : shapes) {
        shape->Update();
    }
}

std::vector<std::shared_ptr<Shape>> Area::GetShapes()
{
    return shapes;
}

