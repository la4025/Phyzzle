#include "ZeldaLight.h"

#include "ZeldaCamera.h"

using namespace DirectX;

ZeldaLight::ZeldaLight(LightType type) :
	ambient({ 0.2f, 0.2f, 0.2f, 1.0f }),
	diffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
	specular({ 1.0f, 1.0f, 1.0f, 1.0f }),
	position({ 0.0f, 0.0f, 0.0f, 0.0f }),
	direction({ 0.0f, 0.0f, 1.0f, 0.0f }),
	type(type),
	range(10.0f),
	angle(3.141592f * 0.1666f)
{

}

ZeldaLight::~ZeldaLight()
{

}

void ZeldaLight::SetAmbient(float red, float green, float blue)
{
	ambient = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetDiffuseColor(float red, float green, float blue)
{
	diffuse = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetSpecular(float red, float green, float blue)
{
	specular = XMFLOAT4(red, green, blue, 1.0f);
}

DirectX::XMFLOAT4 ZeldaLight::GetAmbient()
{
	return ambient;
}

DirectX::XMFLOAT4 ZeldaLight::GetDiffuseColor()
{
	return diffuse;
}

DirectX::XMFLOAT4 ZeldaLight::GetSpecular()
{
	return specular;
}

DirectX::XMMATRIX ZeldaLight::GetViewMatrix(ZeldaCamera* maincamera)
{
	DirectX::XMMATRIX cameraTM = maincamera->GetTransformMatrix();
	DirectX::XMMATRIX cameraRotation = cameraTM;
	DirectX::XMMATRIX cameraTranslation = cameraTM;
	
	cameraRotation.r[3].m128_f32[0] = 0.0f;
	cameraRotation.r[3].m128_f32[1] = 0.0f;
	cameraRotation.r[3].m128_f32[2] = 0.0f;

	cameraTranslation.r[0].m128_f32[0] = 1.0f;
	cameraTranslation.r[0].m128_f32[1] = 0.0f;
	cameraTranslation.r[0].m128_f32[2] = 0.0f;
	cameraTranslation.r[1].m128_f32[0] = 0.0f;
	cameraTranslation.r[1].m128_f32[1] = 1.0f;
	cameraTranslation.r[1].m128_f32[2] = 0.0f;
	cameraTranslation.r[2].m128_f32[0] = 0.0f;
	cameraTranslation.r[2].m128_f32[1] = 0.0f;
	cameraTranslation.r[2].m128_f32[2] = 1.0f;
	
	DirectX::XMVECTOR cameraDirection = DirectX::XMVector3Normalize(DirectX::XMVector4Transform({ 0.0f, 0.0f, 1.0f, 1.0f }, cameraRotation));
	DirectX::XMVECTOR cameraPosition = { cameraTM.r[3].m128_f32[0], cameraTM.r[3].m128_f32[1], cameraTM.r[3].m128_f32[2], 0.0f };

	DirectX::XMMATRIX worldMatrix;
	switch (type)
	{
		case LightType::Directional:
		{
			DirectX::XMVECTOR lightDirection = DirectX::XMVectorSet(direction.x, direction.y, direction.z, 0.0f);
			DirectX::XMVECTOR zVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			if (DirectX::XMVector4Equal(zVector, lightDirection)) {
				worldMatrix = DirectX::XMMatrixIdentity();
			}
			else
			{
				// 두 벡터의 내적을 계산하여 코사인 각도를 구함
				float cosTheta = DirectX::XMVector4Dot(zVector, lightDirection).m128_f32[0];

				// v1과 targetV가 평행하거나 반대 방향인 경우 아무런 회전도 필요하지 않음
				if (cosTheta >= 0.999999f || cosTheta <= -0.999999f) {
					worldMatrix = DirectX::XMMatrixIdentity();
				}
				else
				{
					// 각도를 계산
					float theta = std::acos(cosTheta);

					// 회전 축을 계산
					DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(zVector, lightDirection));

					// 회전 행렬 생성
					worldMatrix = DirectX::XMMatrixRotationAxis(rotationAxis, theta);
				}
			}
			
			worldMatrix =
				worldMatrix *
				DirectX::XMMatrixTranslationFromVector(-ShadowMap::Range * DirectX::XMVector3Normalize({ direction.x, direction.y, direction.z, 0.0f })) *
				DirectX::XMMatrixTranslationFromVector((ShadowMap::Range - ShadowMap::Offset) * DirectX::XMVector3Normalize({ cameraDirection.m128_f32[0], cameraDirection.m128_f32[1], cameraDirection.m128_f32[2], 0.0f})) *
				DirectX::XMMatrixTranslationFromVector(cameraPosition);

			break;
		}

		case LightType::Point:
		{
			assert(0);
			break;
		}

		case LightType::Spot:
		{
			assert(0);
			break;
		}

		default:
			assert(0);
			break;
	}

	return DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

DirectX::XMMATRIX ZeldaLight::GetOrthoMatrix()
{
	return DirectX::XMMatrixOrthographicLH(2.0f * ShadowMap::Range, 2.0f * ShadowMap::Range, 0.0f, 2.0f * ShadowMap::Range);
}

void ZeldaLight::SetDirection(float x, float y, float z)
{
	assert(type == LightType::Directional);
	direction = XMFLOAT4(x, y, z, 0.0f);
}

DirectX::XMFLOAT3 ZeldaLight::GetDirection()
{
	assert(type == LightType::Directional);
	return XMFLOAT3(direction.x, direction.y, direction.z);
}

void ZeldaLight::SetRange(float range)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->range = range;
}

void ZeldaLight::SetPosition(float x, float y, float z)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->position = XMFLOAT4(x, y, z, 0.0f);
}

float ZeldaLight::GetRange()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return range;
}

DirectX::XMFLOAT3 ZeldaLight::GetPosition()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return XMFLOAT3(position.x, position.y, position.z);
}

void ZeldaLight::SetAngle(float angle)
{
	assert(type == LightType::Spot);
	this->angle = angle;
}

float ZeldaLight::GetAngle()
{
	assert(type == LightType::Spot);
	return angle;
}

LightType ZeldaLight::GetLightType()
{
	return type;
}

LightInfo ZeldaLight::GetLightInfo()
{
	LightInfo lightInfo;
	lightInfo.color.ambient = ambient;
	lightInfo.color.diffuse = diffuse;
	lightInfo.color.specular = specular;
	lightInfo.position = position;
	lightInfo.direction = direction;
	lightInfo.type = type;
	lightInfo.range = range;
	lightInfo.angle = angle;

	return lightInfo;
}

DirectX::XMMATRIX ZeldaLight::GetWorldMatrix()
{
	switch (type)
	{
		case LightType::Directional:
		{
			// Directional Light는 WorldMatrix를 사용할 필요가 없기 때문에 그냥 적당히 넘겨준다.
			return DirectX::XMMatrixIdentity();
			break;
		}

		case LightType::Point:
		{
			float extra = 0.1f;

			DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(2.0f * range, 2.0f * range, 2.0f * range);
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

			return scaleMatrix * translation;
			break;
		}

		case LightType::Spot:
		{
			DirectX::XMVECTOR currentDir = { 0.0f, -1.0f, 0.0f, 0.0f };
			DirectX::XMVECTOR targetDir = XMVector3Normalize({ direction.x, direction.y, direction.z, 0.0f });
			DirectX::XMVECTOR axis = DirectX::XMVector3Cross(currentDir, targetDir);
			float angle = DirectX::XMScalarACos(DirectX::XMVector3Dot(currentDir, targetDir).m128_f32[0]);

			DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(range, range, range);
			DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(axis, angle);
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
			return scale * rotation * translation;
			break;
		}

		default:
			assert(0);
			break;
	}
}
