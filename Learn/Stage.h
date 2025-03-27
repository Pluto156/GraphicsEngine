#pragma once
#include "Area.h"  // 引入 Area 类头文件
#include "CVector.h"
class Stage : public Transform{
public:
    // 构造函数，初始化舞台并设置每个区域的中心位置
    Stage(std::string name);
    Stage(std::string name,float posx, float posy, float posz);

    // 增加区域
    void AddArea(Area* area);

    // 绘制所有区域
    void Draw() const;

    void IntersectWithRay(
        const CVector& origin,
        const CVector& direct,
        float length) const;

private:
    // 舞台的Area列表
    std::vector<Area*> Areas;
};
