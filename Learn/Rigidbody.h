#pragma once
class RigidBody:public Component
{
private:
                      // ���ۼ������洢��ǰ֡������������ʸ����
    CVector torqueAccum;                 // Ť���ۼ������洢��ǰ֡����Ť�ص�ʸ����
    float inverseMass;                   // ����������1/mass���������Ż�������㣬�������޴�ʱֵΪ
    CMatrix inverseInertiaTensorWorld;   // ����ռ��µ������������������ת����ѧ����
    CMatrix inverseInertiaTensor;         // �ֲ��ռ��µ�ԭʼ���������
    CMatrix transformMatrix;

public:
    CVector forceAccum;
    // �����������ݣ����������ռ����������
    void CalculateDerivedData();         // ���¹�������������ռ�任

    // ���ù����������贫��ֲ��ռ����������
    void SetInertiaTensor(const CMatrix& inertiaTensor); // ��ʼ���������������������

    // �����÷���
    void AddForce(const CVector& force); // ʩ������������������ϵ��
    void AddForceAtBodyPoint(const CVector& force, const CVector& point); // ������ֲ������ʩ����
    void AddForceAtPoint(const CVector& force, const CVector& point);     // �����������ʩ����

    // ����ۻ���
    void ClearAccumulator();             // ÿ֡����������������Ť���ۻ�

    // ������ּ���
    void Integrate(float duration);      // ִ������ģ����֣�����λ�ú���ת

    // ����ת��
    CVector GetPointInWorldSpace(const CVector& point) const; // �ֲ�����ת��������

    // ����״̬�ж�
    bool hasFiniteMass() const;          // �ж������Ƿ������Ч�����������޴�

    // �������Է���
    float GetMass() const;               // ��ȡ��������Ϊ���޴󷵻�0��
    void SetMass(float mass);            // ���������������䵹��

    // �˶�����
    CVector velocity;                    // ���ٶȣ���λ����/�룩
    CVector angularVelocity;
    CVector acceleration;                // ���ٶȣ����ֶ����û�ͨ�������㣩
    float angularDamping;                // ���ٶ�����ϵ����0-1����Ӱ����ת˥��
    float linearDamping;                 // ���ٶ�����ϵ����0-1����Ӱ���˶�˥��


    ~RigidBody();

};

