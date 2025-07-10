#include "stdafx.h"
#include "ForceSpring.h"
namespace PhysicsLit
{
    ForceSpring::ForceSpring(const CVector3& connectionPoint, const CVector3& otherConnectionPoint, RigidBodyPrimitive* other, float springConstant, float restLength) :
        mConnectionPoint(connectionPoint, 1.0f),
        mOtherConnectionPoint(otherConnectionPoint, 1.0f),
        mOther(other),
        mSpringConstant(springConstant),
        mRestLength(restLength)
    {
        mType = ForceGeneratorType::Spring;
    }

    void ForceSpring::UpdateForce(RigidBodyPrimitive* rigidBody, float duration)
    {
        // 弹簧链接的两端在世界坐标系下的位置
        CVector3 lws = rigidBody->GetTransform() * mConnectionPoint;
        CVector3 ows = mOther->GetTransform() * mOtherConnectionPoint;
        // 间距
        CVector3 dis = lws - ows;
        // 弹簧当前长度
        float length = dis.len();
        // 弹簧伸缩长度
        float delta = length - mRestLength;
        // 弹力大小
        float forceScalar = delta * mSpringConstant;
        // 弹力向量
        CVector3 force = -forceScalar * dis.GetNormalize();
        // 施加到当前刚体上
        rigidBody->AddForceAtPoint(force, lws);
    }
}