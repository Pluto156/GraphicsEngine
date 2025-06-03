#include "stdafx.h"
#include "OBB.h"
#include "CVector3.h"
#include "CMatrix4.h"
#include "PointCollision.h"
#include "GameObject.h"

void OBB::Update() {
    center = gameObject->transform->position;
    rotation = gameObject->transform->rotation;
}

float OBB::IntersectWithRayAndOBB(
    const CVector3& origin,
    const CVector3& direct,
    float length,
    PointCollision& intersection) const
{
    // 计算射线与 OBB 的交点
    float t = IntersectWithRayAndOBB(origin, direct, center, halfSizes, rotation, intersection);
    if (intersection.hit && intersection.t <= length) {
        return t;
    }
    return -1.0f;
}

float OBB::IntersectWithRayAndOBB(
    const CVector3& origin,
    const CVector3& direct,
    const CVector3& position,
    const CVector3& halfSize,
    const CMatrix4& orientation,
    PointCollision& intersection) const
{
    intersection.Reset();

    // 提取旋转矩阵的轴
    CVector3 axisX(orientation.m00, orientation.m01, orientation.m02);
    CVector3 axisY(orientation.m10, orientation.m11, orientation.m12);
    CVector3 axisZ(orientation.m20, orientation.m21, orientation.m22);

    // 计算从射线起点到包围盒中心的向量
    CVector3 p = position - origin;

    // 计算射线在 OBB 轴上的投影
    CVector3 f(
        Math::Dot(axisX,direct),
        Math::Dot(axisY,direct),
        Math::Dot(axisZ,direct)
    );

    // 计算射线起点到包围盒中心的距离在 OBB 轴上的投影
    CVector3 e(
        Math::Dot(axisX, p),
        Math::Dot(axisY, p),
        Math::Dot(axisZ, p)
    );

    // 存储 t 值的数组，表示各个轴的交点
    float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    CVector3 normals[6] = { axisX, axisX * -1, axisY, axisY * -1, axisZ, axisZ * -1 };

    // 检查 X 轴上的交点
    if (f.x == 0) {
        if (-e.x - halfSize.x > 0 || -e.x + halfSize.x < 0) {
            return -1.0f;
        }
        f.x = 0.00001f; // 防止除以零
    }
    t[0] = (e.x + halfSize.x) / f.x;
    t[1] = (e.x - halfSize.x) / f.x;

    // 检查 Y 轴上的交点
    if (f.y == 0) {
        if (-e.y - halfSize.y > 0 || -e.y + halfSize.y < 0) {
            return -1.0f;
        }
        f.y = 0.00001f; // 防止除以零
    }
    t[2] = (e.y + halfSize.y) / f.y;
    t[3] = (e.y - halfSize.y) / f.y;

    // 检查 Z 轴上的交点
    if (f.z == 0) {
        if (-e.z - halfSize.z > 0 || -e.z + halfSize.z < 0) {
            return -1.0f;
        }
        f.z = 0.00001f; // 防止除以零
    }
    t[4] = (e.z + halfSize.z) / f.z;
    t[5] = (e.z - halfSize.z) / f.z;

    // 计算交点的最小和最大 t 值
    float tmin = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
    float tmax = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

    if (tmax < 0) {
        return -1.0f;
    }

    if (tmin > tmax) {
        return -1.0f;
    }

    if (tmin < 0) {
        intersection.closestPoint = origin + direct * tmax;
        intersection.t = tmax;
        intersection.hit = true;
        for (int i = 0; i < 6; i++) {
            if (tmax == t[i]) {
                intersection.normal = normals[i];
                break;
            }
        }
        return tmax;
    }

    intersection.closestPoint = origin + direct * tmin;
    intersection.t = tmin;
    for (int i = 0; i < 6; i++) {
        if (tmin == t[i]) {
            intersection.normal = normals[i];
            break;
        }
    }
    intersection.hit = true;
    return tmin;
}
