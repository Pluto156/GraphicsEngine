#include "stdafx.h"
#include "BVHNode.h"

namespace PhysicsLit
{
    BVHNode::BVHNode(BVHNode* parent, const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody)
        : mParent(parent), mBoundingVolume(boundingVolume), mRigidBody(rigidBody)
    {
        rigidBody->mBVHNode = this;
        mChildren[0] = mChildren[1] = nullptr;
    }

    BVHNode::~BVHNode()
    {
        if (mRigidBody && mRigidBody->mBVHNode == this)
            mRigidBody->mBVHNode = nullptr;

        if (mChildren[0]) {
            mChildren[0]->mParent = nullptr;
            delete mChildren[0];
        }
        if (mChildren[1]) {
            mChildren[1]->mParent = nullptr;
            delete mChildren[1];
        }
    }

    bool BVHNode::IsLeaf() const { return mRigidBody != nullptr; }

    void BVHNode::UpdateBoundingVolume(bool recurse)
    {
        if (!IsLeaf())
            mBoundingVolume = BoundingSphere(mChildren[0]->mBoundingVolume, mChildren[1]->mBoundingVolume);

        if (mParent && recurse)
            mParent->UpdateBoundingVolume(true);
    }

    void BVHNode::Insert(const BoundingSphere& volume, RigidBodyPrimitive* body)
    {
        if (IsLeaf())
        {
            mChildren[0] = new BVHNode(this, mBoundingVolume, mRigidBody);
            mChildren[1] = new BVHNode(this, volume, body);
            mRigidBody = nullptr;
            UpdateBoundingVolume();
        }
        else
        {
            if (mChildren[0]->mBoundingVolume.GetGrowth(volume) < mChildren[1]->mBoundingVolume.GetGrowth(volume))
                mChildren[0]->Insert(volume, body);
            else
                mChildren[1]->Insert(volume, body);
        }
    }

    void BVHNode::Remove()
    {
        if (!mParent) return; // 根节点不处理

        BVHNode* sibling = (mParent->mChildren[0] == this) ? mParent->mChildren[1] : mParent->mChildren[0];

        mParent->mRigidBody = sibling->mRigidBody;
        mParent->mBoundingVolume = sibling->mBoundingVolume;
        mParent->mChildren[0] = sibling->mChildren[0];
        mParent->mChildren[1] = sibling->mChildren[1];

        if (mParent->mRigidBody)
            mParent->mRigidBody->mBVHNode = mParent;

        if (mParent->mChildren[0]) mParent->mChildren[0]->mParent = mParent;
        if (mParent->mChildren[1]) mParent->mChildren[1]->mParent = mParent;

        sibling->mChildren[0] = sibling->mChildren[1] = nullptr;
        sibling->mParent = nullptr;
        sibling->mRigidBody = nullptr;

        delete sibling;
        delete this;
    }

    uint32_t BVHNode::GetPotentialContacts(PotentialContact* contacts, uint32_t limit) const
    {
        if (IsLeaf() || limit == 0)
            return 0;

        uint32_t count = mChildren[0]->GetPotentialContactsWith(mChildren[1], contacts, limit);
        if (limit > count)
            count += mChildren[0]->GetPotentialContacts(contacts + count, limit - count);
        if (limit > count)
            count += mChildren[1]->GetPotentialContacts(contacts + count, limit - count);

        return count;
    }

    uint32_t BVHNode::GetPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, uint32_t limit) const
    {
        if (!IsOverlapWith(other) || limit == 0)
            return 0;

        if (IsLeaf() && other->IsLeaf())
        {
            contacts->mRigidBodies[0] = mRigidBody;
            contacts->mRigidBodies[1] = other->mRigidBody;
            return 1;
        }

        if (other->IsLeaf() || (!IsLeaf() && mBoundingVolume.GetVolume() >= other->mBoundingVolume.GetVolume()))
        {
            uint32_t count = mChildren[0]->GetPotentialContactsWith(other, contacts, limit);
            if (limit > count)
                return count + mChildren[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
            return count;
        }
        else
        {
            uint32_t count = GetPotentialContactsWith(other->mChildren[0], contacts, limit);
            if (limit > count)
                return count + GetPotentialContactsWith(other->mChildren[1], contacts + count, limit - count);
            return count;
        }
    }

    bool BVHNode::IsOverlapWith(const BVHNode* other) const
    {
        return mBoundingVolume.IsOverlapWith(other->mBoundingVolume);
    }

    void BVHNode::QueryPotentialContacts(const BoundingSphere& volume, std::vector<BVHNode*>& results)
    {
        // 当前节点 BV 与查询球不重叠则剪枝
        if (!mBoundingVolume.IsOverlapWith(volume))
            return;

        // 如果是叶子，加入结果集
        if (IsLeaf()) {
            results.push_back(this);
            return;
        }

        // 递归查询子节点
        if (mChildren[0]) mChildren[0]->QueryPotentialContacts(volume, results);
        if (mChildren[1]) mChildren[1]->QueryPotentialContacts(volume, results);
    }
}