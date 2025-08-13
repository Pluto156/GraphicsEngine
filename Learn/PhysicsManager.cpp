#include "stdafx.h"
#include "PhysicsManager.h"
#include "CollisionData.h"
#include "CollisionDetector.h"
#include "ContactResolver.h"
namespace PhysicsLit
{

	PhysicsManager::PhysicsManager()
	{
		mCollisionData = new CollisionData(1000);
		mContactResolver = new ContactResolver(1000);
		mPotentialContacts = new PotentialContact[mMaxPotentialContacts];
	}

	void PhysicsManager::RemoveGameObject(GameObject* gameobject) {

		auto it = GoToRigidBody.find(gameobject);
		if (it != GoToRigidBody.end()) {
			GameScriptManager::Instance().InvalidateObject(it->second);

			if (it->second) {
				RemoveBoundingVolume(it->second);
				RigidBodyToGO.erase(it->second);
			}
			GoToRigidBody.erase(it);
		}
	}

	void PhysicsManager::Update() {
		long long targetFrame = TimeManager::curTime_micro / TimeManager::fixedDeltaTime_micro;
		long long deltaFrame = targetFrame - mCurPhyFrame;

		// 当前游戏帧数高于物理引擎的目标帧数，跳过这一帧的物理更新
		if (deltaFrame <= 0)
			return;
		// 当前游戏帧数低于物理引擎的目标帧数，需要补物理帧，但是不能补太多，否则会导致游戏帧率进一步降低，恶性循环然后卡死
		else if (deltaFrame > 10)
			deltaFrame = 10;

		for (long long i = 0; i < deltaFrame; i++)
		{
			BeginFrame();

			UpdatePhysics(TimeManager::fixedDeltaTime);
			EndFrame();
		}

		// 无论有没有补帧，或者补了多少帧，都当作已经追上了目标帧数
		mCurPhyFrame = targetFrame;
	}

	void PhysicsManager::BeginFrame()
	{
		if (mBVHRoot == nullptr)
			return;
		// 重置碰撞数据
		mCollisionData->Reset();

		for (auto& iter : GoToRigidBody)
		{
			// 清除刚体在上一帧累计的力和力矩
			iter.second->ClearAccumulators();
			// 更新刚体在这一帧的相关数据
			iter.second->CalculateDerivedData();
		}
	}
	void PhysicsManager::UpdatePhysics(const float deltaTime)
	{
		if (mBVHRoot == nullptr)
			return;

		// 更新刚体的位置和旋转
		for (auto& iter : GoToRigidBody)
		{
			iter.second->Integrate(deltaTime);
		}

		// 生成潜在碰撞
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);

		// 清空当前碰撞数据计数
		mCollisionData->mCurContactCount = 0;

		for (uint32_t i = 0; i < potentialContactCount; ++i)
		{
			auto& contact = mPotentialContacts[i];
			CollisionPrimitive* a = contact.mRigidBodies[0]->mCollisionVolume;
			CollisionPrimitive* b = contact.mRigidBodies[1]->mCollisionVolume;
			if (!CanCollide(a, b) || a->isTrigger || b->isTrigger)
			{
				continue;
			}


			// 普通碰撞检测，成功则记录碰撞信息
			uint32_t collisionCount = CollisionDetector::Detect(a, b, mCollisionData);
			//if (collisionCount > 0)
			//{
			//	GameScriptManager::Instance().ReportCollision(contact.mRigidBodies[0], contact.mRigidBodies[1]);
			//}
			
		}

		// 统一处理所有碰撞响应
		mContactResolver->ResolveContacts(mCollisionData->mContactArray, mCollisionData->mCurContactCount, deltaTime);
		for (uint32_t i = 0; i < potentialContactCount; ++i)
		{
			auto& contact = mPotentialContacts[i];
			CollisionPrimitive* a = contact.mRigidBodies[0]->mCollisionVolume;
			CollisionPrimitive* b = contact.mRigidBodies[1]->mCollisionVolume;
			if (!CanCollide(a, b))
			{
				continue;
			}
			/*if (contact.mRigidBodies[0]->GetGameObjectName() == "Car2")
			{
				std::cout<<"After " << contact.mRigidBodies[0]->GetPosition().ToString() << std::endl;

			}*/

			if (a->isTrigger || b->isTrigger)
			{
				// 碰撞检测确认是否真正重叠
				uint32_t triggerCollisionCount = CollisionDetector::Detect(a, b, &CollisionData(1));
				if (triggerCollisionCount > 0) {
					GameScriptManager::Instance().ReportTrigger(contact.mRigidBodies[0], contact.mRigidBodies[1]);
				}
			}
			else
			{
				// 普通碰撞检测，成功则记录碰撞信息
				uint32_t collisionCount = CollisionDetector::Detect(a, b, mCollisionData);
				if (collisionCount > 0)
				{
					GameScriptManager::Instance().ReportCollision(contact.mRigidBodies[0], contact.mRigidBodies[1]);
				}
			}
		}

		
	}

	void PhysicsManager::EndFrame()
	{
		if (mBVHRoot == nullptr)
			return;

		for (auto& iter : GoToRigidBody)
		{
			auto transform = iter.first->GetComponent<Transform>();
			transform->SetPosition(iter.second->GetPosition());
			transform->SetQuaternion(iter.second->GetRotation());
			//std::cout << "GetRotation" << iter.second->GetRotation().ToCMatrix4().ToEuler().ToString() << std::endl;

			// 更新BV的位置
			if (iter.second->mBVHNode)
			{
				iter.second->mBVHNode->mBoundingVolume.mCenter = iter.second->GetPosition();
				iter.second->mBVHNode->UpdateBoundingVolume();
			}
		}
	}

	void PhysicsManager::AddBoundingVolume(const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody)
	{
		if (mBVHRoot)
			mBVHRoot->Insert(boundingVolume, rigidBody);
		else
			mBVHRoot = new BVHNode(nullptr, boundingVolume, rigidBody);
	}

	void PhysicsLit::PhysicsManager::RemoveBoundingVolume(RigidBodyPrimitive* rigidBody)
	{
		if (!rigidBody || !rigidBody->mBVHNode)
			return;

		BVHNode* node = rigidBody->mBVHNode;

		// 如果是根节点，直接清空整棵树
		if (node == mBVHRoot) {
			delete mBVHRoot;
			mBVHRoot = nullptr;
		}
		else {
			node->Remove();  // 正确方式：由 BVHNode::Remove() 执行结构重组和 delete
		}

		rigidBody->mBVHNode = nullptr;
	}
	void PhysicsManager::CheckImmediateCollision(RigidBodyPrimitive* bullet)
	{
		if (!bullet || !bullet->mCollisionVolume || !bullet->mBVHNode || mBVHRoot == nullptr)
			return;

		if (bullet->mCollisionVolume->isTrigger)return;

		std::vector<BVHNode*> candidates;
		mBVHRoot->QueryPotentialContacts(bullet->mBVHNode->mBoundingVolume, candidates);

		for (auto* node : candidates) {
			if (!node || node->mRigidBody == nullptr || node->mRigidBody == bullet)
				continue;

			uint32_t collisionCount = CollisionDetector::Detect(
				bullet->mCollisionVolume,
				node->mRigidBody->mCollisionVolume,
				mCollisionData
			);

			if (collisionCount > 0) {
				GameScriptManager::Instance().ReportCollision(bullet, node->mRigidBody);
			}
		}
	}
	// --- Raycast implementation (replace previous placeholder) ---
	bool PhysicsManager::Raycast(
		const CVector3& origin,
		const CVector3& direction,
		RaycastHit& outHit,
		float maxDistance,
		unsigned int layerMask,
		bool includeTriggers)
	{
		outHit = RaycastHit(); // reset

		if (mBVHRoot == nullptr)
			return false;

		// 规范化方向（保留原始 maxDistance）
		CVector3 dir = direction;
		float dirLen = dir.len();
		if (dirLen <= 0.0f) return false;
		if (fabs(dirLen - 1.0f) > 1e-6f) dir.Normalize();

		// 如果 maxDistance 非有限值，使用一个足够大的半长用于包围球 (以便 BVH 粗筛)
		const float INF_HALF = 1e6f;
		float halfLen = (maxDistance == FLT_MAX) ? INF_HALF : (maxDistance * 0.5f);
		CVector3 mid = origin + dir * halfLen;
		BoundingSphere rayCoverSphere(mid, halfLen + 1e-3f);

		// 查询 BVH 中可能的节点
		std::vector<BVHNode*> candidates;
		mBVHRoot->QueryPotentialContacts(rayCoverSphere, candidates);

		if (candidates.empty())
			return false;

		// 最短命中距离
		float closestDist = std::numeric_limits<float>::infinity();
		bool found = false;

		// 为相交检测构造 Ray
		Ray ray(origin, dir); // 假设 Ray 有 (origin, direction) 构造函数

		for (auto* node : candidates)
		{
			if (!node || !node->mRigidBody) continue;
			RigidBodyPrimitive* rb = node->mRigidBody;

			CollisionPrimitive* prim = rb->mCollisionVolume;
			if (!prim) continue;

			// 层过滤：primitive->layer 与 layerMask 做按位与
			if (((prim->layer) & layerMask) == 0) continue;

			// trigger 过滤
			if (!includeTriggers && prim->isTrigger) continue;

			// 根据碰撞体类型调用相应的相交检测函数（能返回 hit info）
			RayHitInfo hitInfo; // 你工程里应该有这个类型
			bool hit = false;

			ColliderType type = prim->GetType();
			switch (type)
			{
			case ColliderType::Box:
			{
				// 将基类转换为 CollisionBox
				CollisionBox* box = static_cast<CollisionBox*>(prim);
				if (box)
				{
					// 使用 IntersectionDetector 的重载：
					// bool Detect(const Ray& ray, const CollisionBox& box, RayHitInfo& hit);
					hit = IntersectionDetector::Detect(ray, *box, hitInfo);
				}
				break;
			}
			case ColliderType::Sphere:
			{
				CollisionSphere* sphere = static_cast<CollisionSphere*>(prim);
				if (sphere)
				{
					hit = IntersectionDetector::Detect(ray, *sphere);
				}
				break;
			}
			case ColliderType::Plane:
			{
				CollisionPlane* plane = static_cast<CollisionPlane*>(prim);
				if (plane)
				{
					hit = IntersectionDetector::Detect(ray, *plane);
				}
				break;
			}
			case ColliderType::Circle2D:
			{
				CollisionCircle2D* circle = static_cast<CollisionCircle2D*>(prim);
				if (circle)
				{
					// 有时 Circle2D 仅有不带 hit info 的版本和带 hit info 的重载两种
					hit = IntersectionDetector::Detect(ray, *circle, hitInfo);
				}
				break;
			}
			default:
				// 若有其他类型，可能需要扩展
				break;
			}

			if (!hit) continue;

			// 一般 RayHitInfo.distance 表示从 origin 沿 dir 的距离
			// 过滤距离为负（在射线起点后方）或者超过 maxDistance
			if (hitInfo.distance < 0.0f) continue;
			if (hitInfo.distance > maxDistance) continue;

			if (hitInfo.distance < closestDist)
			{
				closestDist = hitInfo.distance;
				found = true;

				outHit.hit = true;
				outHit.rigidbody = rb;
				auto it = RigidBodyToGO.find(rb);
				outHit.gameObject = (it != RigidBodyToGO.end()) ? it->second : nullptr;
				outHit.distance = hitInfo.distance;
				outHit.point = origin + dir * hitInfo.distance;
				//outHit.normal = hitInfo.normal;
			}
		} // end for candidates

		return found;
	}


	std::string PhysicsManager::GetGameObjectName(RigidBodyPrimitive* rigidbody) {
		auto it = RigidBodyToGO.find(rigidbody);
		if (it != RigidBodyToGO.end() && it->second) {
			return it->second->name;
		}
		return "NULL";
	}


}
