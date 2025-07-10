#pragma once
#include "ForceGenerator.h"
namespace PhysicsLit
{
	class ForceSpring : public ForceGenerator
	{
	public:
		ForceSpring(const CVector3& connectionPoint, const CVector3& otherConnectionPoint, RigidBodyPrimitive* other, float springConstant, float restLength);

		virtual void UpdateForce(RigidBodyPrimitive* rigidBody, float duration);

	private:
		// 弹簧在当前对象上的连接点(对象的局部空间)
		CVector4 mConnectionPoint;
		// 弹簧在另一个对象上的连接点(另一个对象的局部空间)
		CVector4 mOtherConnectionPoint;
		// 弹簧另一端的刚体
		RigidBodyPrimitive* mOther;
		// 弹簧系数
		float mSpringConstant;
		// 弹簧不受外力时的长度
		float mRestLength;
	};
}
