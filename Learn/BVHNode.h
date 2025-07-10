#pragma once

#include <vector>
#include "BoundingSphere.h"
#include "PhysicsLitEnumStruct.h"

namespace PhysicsLit
{
    class RigidBodyPrimitive;

    // Bounding Volume Hierarchy Node
    class BVHNode
    {
    public:
        // 父节点
        BVHNode* mParent;
        // 子节点
        BVHNode* mChildren[2];
        // 该节点下的刚体(通常叶子节点才有)
        RigidBodyPrimitive* mRigidBody;
        // 包含该节点和所有子节点下的对象的BoundingVolume
        BoundingSphere mBoundingVolume;

        // parent 为空表示是根节点
        BVHNode(BVHNode* parent, const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody);
        // 析构函数会递归删除所有子节点
        ~BVHNode();

        // 是否为叶子节点
        bool IsLeaf() const;
        // 更新当前节点的BV，默认会向根节点递归更新
        void UpdateBoundingVolume(bool recurse = true);
        // 在当前节点下插入一个刚体
        void Insert(const BoundingSphere& volume, RigidBodyPrimitive* body);
        // 从 BVH 中安全移除自身
        void Remove();
        // 检测潜在碰撞
        uint32_t GetPotentialContacts(PotentialContact* contacts, uint32_t limit) const;

        // 根据给定包围球查询可能相交的叶子节点（用于即时碰撞检测）
        void QueryPotentialContacts(const BoundingSphere& volume, std::vector<BVHNode*>& results);

    private:
        bool IsOverlapWith(const BVHNode* other) const;
        uint32_t GetPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, uint32_t limit) const;
    };
}