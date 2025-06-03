#pragma once
namespace PhysicsLit
{
	class BoundingSphere
	{
	public:
		float mRadius;
		CVector3 mCenter;

		BoundingSphere(const CVector3& center, float radius);
		BoundingSphere(const BoundingSphere& bs1, const BoundingSphere& bs2);
		~BoundingSphere() {};

		// ��ȡ��Χ�����
		float GetVolume() const;
		// ����һ����Χ���Ƿ��ཻ
		bool IsOverlapWith(const BoundingSphere& other) const;
		// ��������Χ��Ҫ����һ����Χ�����ȥ����Ҫ������
		// ���ص���ֵֻ�Ǹ��������ȣ���һ���Ǿ����ʲô�뾶����������������ʵ���������ֵ
		float GetGrowth(const BoundingSphere& other) const;
	};
}

