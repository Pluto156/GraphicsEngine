#pragma once
#include "Transform.h"
class Box :public Transform {
public:
    // ���캯������ʼ�����ӵĳ���ߺ���������
    Box(std::string name,float width, float height, float depth, CVector Position,float r=1,float g = 1,float b =1, bool isShowLocalAxis = false);
    Box(std::string name,float width, float height, float depth, CVector Position,CMatrix Rotation,CEuler EulerAngles ,float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

    // ���ú��ӵ�λ�úͳߴ�
    static Box* CreateBox(std::string name,float width, float height, float depth, CVector Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);
    static Box* CreateBox(std::string name, float width, float height, float depth, CVector Position, CMatrix Rotation,CEuler EulerAngles,float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

    // ���ƺ���
    void Draw() const override;

    float GetWidth();
    float GetHeight();
    float GetDepth();
    bool isSelect;
    std::string ToString();

private:
    float m_width, m_height, m_depth;
    float r, g, b;
};

