#pragma once
#include <vector>
#include "Box.h"  // 引入 Box 类头文件
#include "CVector.h"
class Area:public Transform {
public:
    Area(std::string name,float x, float y, float z);
    size_t GetShapeCount() const;
    void AddShape(std::shared_ptr<Shape> shape);
    void Draw() override;
    std::vector<std::shared_ptr<Shape>> GetShapes();
    std::vector<std::shared_ptr<Shape>> shapes;

private:
    

};
