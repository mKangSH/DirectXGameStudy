#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

void Frustum::FinalUpdate()
{
	Matrix matViewInv = Camera::S_MatView.Invert();
	Matrix matProjectionInv = Camera::S_MatProjection.Invert();
	Matrix matInv = matProjectionInv * matViewInv;

	// XMVector3TransformCoord : w값을 1로 만들어 준다.
	vector<Vec3> worldPos =
	{
		::XMVector3TransformCoord(Vec3(-1.0f, 1.0f, 0.0f), matInv),
		::XMVector3TransformCoord(Vec3(1.0f, 1.0f, 0.0f), matInv),
		::XMVector3TransformCoord(Vec3(1.0f, -1.0f, 0.0f), matInv),
		::XMVector3TransformCoord(Vec3(-1.0f, -1.0f, 0.0f), matInv),
		::XMVector3TransformCoord(Vec3(-1.0f, 1.0f, 1.0f), matInv),
		::XMVector3TransformCoord(Vec3(1.0f, 1.0f, 1.0f), matInv),
		::XMVector3TransformCoord(Vec3(1.0f, -1.0f, 1.0f), matInv),
		::XMVector3TransformCoord(Vec3(-1.0f, -1.0f, 1.0f), matInv),
	};

	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]); // CW (Clock-Wise)
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]); // CCW (Counter Clock-Wise)
	_planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]); // CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]); // CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]); // CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]); // CCW
}

bool Frustum::ContainsSphere(const Vec3& pos, float radius)
{
	// BoundingFrustum : DX12 제공 절두체

	for (const Vec4& plane : _planes)
	{
		Vec3 normal = Vec3(plane.x, plane.y, plane.z);

		// 평면 방정식: ax + by + cz + d = 0
		// normal : (a, b, c) == (plane.x, plane.y, plane.z)
		// pos : 임의의 좌표 (X, Y, Z) 
		// plane.w : d
		// radius : 물체의 범위
		if (normal.Dot(pos) + plane.w > radius)
		{
			return false;
		}
	}

	return true;
}
