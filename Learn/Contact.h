#pragma once
namespace PhysicsLit
{
	class RigidBodyPrimitive;
	/// <summary>
	/// ��ʾ �����������ײ����ͨ����صķ�����������ײ��ķ�Ӧ��������ײ����ٶȡ�λ���������Լ�Ħ�������ָ����ļ��㡣����һ����ײ�������зǳ���Ҫ����ɲ��֣�ͨ�����������������ڴ��� ��ײ��Ӧ��
	/// </summary>
	class Contact
	{
		friend class ContactResolver;
	public:
		// ��ײ������
		CVector3 mContactPoint;
		// ��ײ����
		CVector3 mContactNormal;
		// ��ײ���(����ײ���ཻʱ������)
		float mPenetration = 0.0f;
		// �ָ�ϵ��
		float mRestitution = 0.0f;
		// Ħ��ϵ��
		float mFriction = 0.0f;

		Contact(RigidBodyPrimitive* rigidBody1 = nullptr, RigidBodyPrimitive* rigidBody2 = nullptr);

		void SetRigidBodies(RigidBodyPrimitive* rigidBody1, RigidBodyPrimitive* rigidBody2);

	private:
		// ��ײ����Ӧ�ĸ���
		RigidBodyPrimitive* mRigidBodies[2];
		// ����ײ����ϵ����������ϵ����ת����
		CMatrix3 mContactToWorld;

		// ��ײ����������������λ��
		CVector3 mRelativeContactPosition[2];
		// ��ײ��ıպ��ٶ�(�����������໥�ӽ����ٶȣ���ײ�ռ�)
		CVector3 mContactVelocity;
		// ��ǰ��ײ�������������ٶȱ仯��(�պ��ٶ�����ײ�����ϵı仯��)
		float mDesiredDeltaVelocity = 0.0f;

		// ��̬������ײϵ����Ħ���ͻָ�ϵ��������ȷ����ײ��Ӧ����ʱ����ʹ����ȷ�������������Щϵ�������������е���ײ��Ӧ������Ҫ��������������ײ�����Ϊ���練����������
		void UpdateCoefficient();
		// �����������壬ͬʱ����ײ����ȡ��(���ǲ������������ر����������Ҫ�����ֶ�����UpdateInternalDatas)
		void SwapRigidBodies();
		// ������ײ�еĸ���״̬���������һ��������Awake����һ��Ҳ����Awake
		void MatchAwakeState();

		// ������ײ��͸��ͨ���ƶ�����ת�������壬�������������󲻽���
		// Ȼ��ͨ��ǰ����ָ��������ش�����Ϣ
		// PS: �˺�����Ȼ���ڴ����ཻ��Ҳ��ı�����λ�ú���ת״̬�����ǲ�����ֱ�Ӹı�mPenetration�������ɵ��õĵط�ȥ����mPenetration
		void ResolvePenetration(CVector3 linearChange[2], CVector3 angularChange[2], float penetration);
		// ������ײ�е��ٶȱ仯����������������ٶȱ仯����Ȼ��ͨ����������
		void ResolveVelocityChange(CVector3 linearVelocityChange[2], CVector3 angularVelocityChange[2]);

		// ������Ħ��������µĳ���
		CVector3 CalculateFrictionImpulse(CMatrix3* inverseInertiaTensor);
		// ������Ħ��������µĳ���
		CVector3 CalculateFrictionlessImpulse(CMatrix3* inverseInertiaTensor);

		// ���µ�ǰ��ײ��ĸ����ڲ�����
		void UpdateInternalDatas(float duration);
		// ������ײ����ϵ����������ϵ����ת����
		void UpdateOrthogonalBasis();
		// ���㵱ǰ��ײ�������������ٶȱ仯��(�պ��ٶ�)
		void UpdateDesiredDeltaVelocity(float duration);

		// �����index�������������ײ����ٶ�
		CVector3 CalculateLocalVelocity(uint32_t index, float duration);
	};
}

