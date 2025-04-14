#pragma once
namespace PhysicsLit
{
    class ForceGenerator;
    class BVHNode;
    class RigidBody :public Component
    {
        private:
            CVector forceAccum;                // ���ۼ������洢��ǰ֡������������ʸ����
            CVector torqueAccum;                 // Ť���ۼ������洢��ǰ֡����Ť�ص�ʸ����
            float inverseMass = 1.0f;                   // ����������1/mass���������Ż�������㣬�������޴�ʱֵΪ
            CMatrix inverseInertiaTensorWorld;   // ����ռ��µ������������������ת����ѧ����
            CMatrix inverseInertiaTensor;         // �ֲ��ռ��µ�ԭʼ���������
            CMatrix transformMatrix;

            // �˸����ϵ��������������б�
            std::vector<ForceGenerator*> mForceGenerators;

            // �˶�����
            CVector velocity;                    // ���ٶȣ���λ����/�룩
            CVector angularVelocity;             // ���ٶ�
            CVector acceleration;                // ���ٶȣ����ֶ����û�ͨ�������㣩
            float angularDamping = 0.95f;                // ���ٶ�����ϵ����0-1����Ӱ����ת˥��
            float linearDamping = 1.0f;                 // ���ٶ�����ϵ����0-1����Ӱ���˶�˥��

        public:
            // ��Ӧ��BVH�ڵ�
            BVHNode* mBVHNode = nullptr;
            // �����������ݣ����������ռ����������
            void CalculateDerivedData();         // ���¹�������������ռ�任

            // ���ù����������贫��ֲ��ռ����������
            void SetInertiaTensor(const CMatrix& inertiaTensor); // ��ʼ���������������������

            // �����÷���
            void AddForce(const CVector& force); // ʩ������������������ϵ��
            void AddForceAtBodyPoint(const CVector& force, const CVector& point); // ������ֲ������ʩ����
            void AddForceAtPoint(const CVector& force, const CVector& point);     // �����������ʩ����

            void AddForceGenerator(ForceGenerator* generator);
            void IntegrateForceGenerators(float duration);
            
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
            // �ж������Ƿ�Ϊ�����
            bool IsInfiniteMass() const;

            void SetVelocity(const CVector& velocity);
            CVector GetVelocity() const;

            void SetAngularVelocity(const CVector& angularVelocity);
            CVector GetAngularVelocity() const;

            ~RigidBody();

    };
}


