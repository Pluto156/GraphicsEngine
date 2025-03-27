#pragma once
#include <vector>
#include "Box.h"  // ���� Box ��ͷ�ļ�
#include "CVector.h"
class Area:public Transform {
public:
    // ���캯������ʼ�����������������������
    Area(std::string name,float x, float y, float z);

    // ��ȡ������ Box ������
    size_t GetBoxCount() const;

    // ���������һ�� Box
    void AddBox(Box* box);

    // ���Ƹ������ڵ����� Box
    void Draw() const;

    std::vector<Box*> GetBoxes();
    // ����� Box �б�
    std::vector<Box*> boxes;
private:
    

};
