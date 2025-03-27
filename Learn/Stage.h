#pragma once
#include "Area.h"  // ���� Area ��ͷ�ļ�
#include "CVector.h"
class Stage : public Transform{
public:
    // ���캯������ʼ����̨������ÿ�����������λ��
    Stage(std::string name);
    Stage(std::string name,float posx, float posy, float posz);

    // ��������
    void AddArea(Area* area);

    // ������������
    void Draw() const;

    void IntersectWithRay(
        const CVector& origin,
        const CVector& direct,
        float length) const;

private:
    // ��̨��Area�б�
    std::vector<Area*> Areas;
};
