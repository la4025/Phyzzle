#include <PxShape.h>

#include "ZnLayer.h"
#include "ZnUtil.h"
#include "ZnBound3.h"
#include "ZnTransform.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	void ColliderHelper::Release(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape**>(_shape);
		(*_pxShape)->release();
		*_pxShape = nullptr;
	}

	void ColliderHelper::DisableQuery(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		_pxShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, !_value);
	}

	void ColliderHelper::DisableSimulation(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		if (_value)
			_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, !_value);
		
		_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, _value);
	}

	void ColliderHelper::SetTrigger(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);

		if (_value)
			_pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !_value);

		_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _value);
	}

	void ColliderHelper::SetLayer(void* _shape, uint32_t _layer)
	{
		assert(_shape != nullptr);
		assert(ZnLayer::MIN_LAYER <= _layer && _layer <= ZnLayer::MAX_LAYER);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		_pxShape->setSimulationFilterData({ _layer, _layer, _layer, _layer });
		physx::PxU32 layerData = 1 << _layer;
		_pxShape->setQueryFilterData({ layerData, layerData, layerData, layerData });
	}

	Eigen::Vector3f ColliderHelper::GetLocalPosition(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getLocalPose().p);
	}

	void ColliderHelper::SetLocalPosition(void* _shape, const Eigen::Vector3f& _pos)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getLocalPose();
		t.p = EigenToPhysx(_pos);
		_pxShape->setLocalPose(t);
	}

	Eigen::Quaternionf ColliderHelper::GetLocalQuaternion(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getLocalPose().q);
	}

	void ColliderHelper::SetLocalQuaternion(void* _shape, const Eigen::Quaternionf& _quat)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getLocalPose();
		t.q = EigenToPhysx(_quat);
		_pxShape->setLocalPose(t);
	}

	Eigen::Vector3f ColliderHelper::GetGlobalPosition(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getActor()->getGlobalPose().p);
	}

	void ColliderHelper::SetGlobalPosition(void* _shape, const Eigen::Vector3f& _pos)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getActor()->getGlobalPose();
		t.p = EigenToPhysx(_pos);
		_pxShape->getActor()->setGlobalPose(t);
	}

	Eigen::Quaternionf ColliderHelper::GetGlobalQuaternion(void* _shape)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		return PhysxToEigen(_pxShape->getActor()->getGlobalPose().q);
	}

	void ColliderHelper::SetGlobalQuaternion(void* _shape, const Eigen::Quaternionf& _quat)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		physx::PxTransform t = _pxShape->getActor()->getGlobalPose();
		t.q = EigenToPhysx(_quat);
		_pxShape->getActor()->setGlobalPose(t);
	}

	ZonaiPhysics::ZnBound3 ColliderHelper::GetBoundingBox(void* _shape, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot)
	{
		const auto pxShape = static_cast<physx::PxShape*>(_shape);

		const physx::PxGeometry& geom = pxShape->getGeometry();
		physx::PxTransform pose(EigenToPhysx(_pos), EigenToPhysx(_rot));
		physx::PxBounds3 aabb;
		physx::PxGeometryQuery::computeGeomBounds(aabb, geom, pose, 0.1f);

		return ZnBound3(PhysxToEigen(aabb.minimum), PhysxToEigen(aabb.maximum));
	}
}
