#include "stdafx.h"
#include "Deadray/Core.h"
#include <limits>

using namespace Deadray;

bool Deadray::RayHitsTriangle(Vector3 rayStart, Vector3 rayDir, const Triangle& triangle, Vector3& hitPoint)
{
	float epsilon = std::numeric_limits<float>::epsilon();

	Vector3 edge1 = triangle.B - triangle.A;
    Vector3 edge2 = triangle.C - triangle.A;
	Vector3 ray_cross_e2 = Vector3::Cross(rayDir, edge2);

	float det = Vector3::Dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector3 s = rayStart - triangle.A;
    float u = inv_det * Vector3::Dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

	Vector3 s_cross_e1 = Vector3::Cross(s, edge1);
    float v = inv_det * Vector3::Dot(rayDir, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * Vector3::Dot(edge2, s_cross_e1);

    if (t > epsilon) // ray intersection
    {
        hitPoint = rayStart + rayDir * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}