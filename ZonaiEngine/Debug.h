#pragma once

namespace ZonaiMath
{
	class Matrix4x4;
	class Vector3D;
	class Vector4D;
	class Quaternion;
}

namespace ZonaiEngine
{
	class Debug
	{
	public:
		static void DrawLine(
			const ZonaiMath::Vector3D& _point1, 
			const ZonaiMath::Vector3D& _point2,
			const ZonaiMath::Vector4D& _color);
		
		static void DrawQuad(
			const ZonaiMath::Vector3D& _position, 
			const ZonaiMath::Quaternion& _rotation, 
			const ZonaiMath::Vector3D& _scale,
			const ZonaiMath::Vector4D& _color);

		static void DrawSphere(
			const ZonaiMath::Vector3D& _position,
			const ZonaiMath::Quaternion& _rotation,
			const ZonaiMath::Vector3D& _scale,
			const ZonaiMath::Vector4D& _color);

		static void DrawCube(
			const ZonaiMath::Vector3D& _position,
			const ZonaiMath::Quaternion& _rotation,
			const ZonaiMath::Vector3D& _scale,
			const ZonaiMath::Vector4D& _color);

		static void DebugRender();
	};
}
