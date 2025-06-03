#pragma once
namespace PhysicsLit
{
	class Ray;
	class CollisionBox;
	class CollisionPlane;
	class CollisionSphere;
	class CollisionCircle2D;
	class IntersectionDetector
	{
	public:
		static bool Detect(const Ray& ray, const CollisionBox& box);
		static bool Detect(const Ray& ray, const CollisionBox& box, RayHitInfo& hit);
		static bool Detect(const Ray& localRay, const CVector3& boxHalfSize, RayHitInfo& hit);
		static bool Detect(const Ray& ray, const CollisionPlane& plane);
		static bool Detect(const Ray& ray, const CollisionSphere& sphere);
		static bool Detect(const Ray& ray, const CollisionCircle2D& circle);
		static bool Detect(const Ray& ray, const CollisionCircle2D& circle, RayHitInfo& hit);
		static bool Detect(const Ray& ray, const CVector3& v0, const CVector3& v1, const CVector3& v2, float& t);
		static bool Detect(const CollisionBox& box1, const CollisionBox& box2);
		static bool Detect(const CollisionSphere& sphere1, const CollisionSphere& sphere2);
		static bool Detect(const CollisionBox& box, const CollisionPlane& plane);
		static bool Detect(const CollisionSphere& sphere, const CollisionPlane& plane);

		// �ж�����BoxͶӰ��ĳ�����ϻ᲻�����ص�
		static bool IsOverlapOnAxis(const CollisionBox& box1, const CollisionBox& box2, const CVector3& axis, const CVector3& centerLine);
		// ��ȡ����BoxͶӰ��ĳ�����ϵ��ص�����(����ֵ����0��ʾ�ص���С��0��ʾδ�ص�)
		static float GetPenetrationOnAxis(const CollisionBox& box1, const CollisionBox& box2, const CVector3& axis, const CVector3& centerLine);
		// ��������߶��Ƿ��ཻ(����ͨ����7��������ȡ)
		static bool DetectLineSegmentContact(const CVector3& midPoint1, const CVector3& dir1, float halfLength1, const CVector3& midPoint2, const CVector3& dir2, float halfLength2, CVector3& contactPoint, bool useOne = true);

	private:
		static bool DetectBoxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane);
		static bool DetectSphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane);
	};
}

