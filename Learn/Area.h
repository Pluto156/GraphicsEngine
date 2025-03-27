#pragma once
#include <vector>
#include "Box.h"  // 引入 Box 类头文件
#include "CVector.h"
class Area:public Transform {
public:
    // 构造函数，初始化区域并设置区域的中心坐标
    Area(std::string name,float x, float y, float z);

    // 获取区域内 Box 的数量
    size_t GetBoxCount() const;

    // 向区域添加一个 Box
    void AddBox(Box* box);

    // 绘制该区域内的所有 Box
    void Draw() const;

    std::vector<Box*> GetBoxes();
    // 区域的 Box 列表
    std::vector<Box*> boxes;
private:
    

};
