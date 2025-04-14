#pragma once
namespace PhysicsLit
{
    class ForceGenerator;
    class BVHNode;
    class RigidBody :public Component
    {
        private:
            CVector forceAccum;                // 力累加器，存储当前帧所有作用力的矢量和
            CVector torqueAccum;                 // 扭矩累加器，存储当前帧所有扭矩的矢量和
            float inverseMass = 1.0f;                   // 质量倒数（1/mass），用于优化物理计算，质量无限大时值为
            CMatrix inverseInertiaTensorWorld;   // 世界空间下的逆惯性张量，用于旋转动力学计算
            CMatrix inverseInertiaTensor;         // 局部空间下的原始逆惯性张量
            CMatrix transformMatrix;

            // 此刚体上的作用力生成器列表
            std::vector<ForceGenerator*> mForceGenerators;

            // 运动属性
            CVector velocity;                    // 线速度（单位：米/秒）
            CVector angularVelocity;             // 角速度
            CVector acceleration;                // 加速度（需手动设置或通过力计算）
            float angularDamping = 0.95f;                // 角速度阻尼系数（0-1），影响旋转衰减
            float linearDamping = 1.0f;                 // 线速度阻尼系数（0-1），影响运动衰减

        public:
            // 对应的BVH节点
            BVHNode* mBVHNode = nullptr;
            // 计算派生数据（如更新世界空间惯性张量）
            void CalculateDerivedData();         // 更新惯性张量的世界空间变换

            // 设置惯性张量（需传入局部空间惯性张量）
            void SetInertiaTensor(const CMatrix& inertiaTensor); // 初始化惯性张量并计算逆矩阵

            // 力作用方法
            void AddForce(const CVector& force); // 施加作用力（世界坐标系）
            void AddForceAtBodyPoint(const CVector& force, const CVector& point); // 在物体局部坐标点施加力
            void AddForceAtPoint(const CVector& force, const CVector& point);     // 在世界坐标点施加力

            void AddForceGenerator(ForceGenerator* generator);
            void IntegrateForceGenerators(float duration);
            
            // 清除累积器
            void ClearAccumulator();             // 每帧物理计算后重置力和扭矩累积

            // 物理积分计算
            void Integrate(float duration);      // 执行物理模拟积分，更新位置和旋转

            // 坐标转换
            CVector GetPointInWorldSpace(const CVector& point) const; // 局部坐标转世界坐标

            // 质量状态判断
            bool hasFiniteMass() const;          // 判断物体是否具有有效质量（非无限大）

            // 质量属性访问
            float GetMass() const;               // 获取质量（若为无限大返回0）
            void SetMass(float mass);            // 设置质量并计算其倒数
            // 判断质量是否为无穷大
            bool IsInfiniteMass() const;

            void SetVelocity(const CVector& velocity);
            CVector GetVelocity() const;

            void SetAngularVelocity(const CVector& angularVelocity);
            CVector GetAngularVelocity() const;

            ~RigidBody();

    };
}


