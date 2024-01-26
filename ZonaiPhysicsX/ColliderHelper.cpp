
#include <PxShape.h>

#include "ColliderHelper.h"

#include "ZnLayer.h"
#include "ZnUtil.h"

namespace ZonaiPhysics
{
	void ColliderHelper::Release(void* _shape)
	{

	}

	void ColliderHelper::SetTrigger(void* _shape, bool _value)
	{
		assert(_shape != nullptr);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		_value ? _pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !_value) : 0;
		_pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, _value);
	}

	void ColliderHelper::SetLayer(void* _shape, uint32_t _layer)
	{
		assert(_shape != nullptr);
		assert(ZnLayer::MIN_LAYER <= _layer && _layer <= ZnLayer::MAX_LAYER);

		const auto _pxShape = static_cast<physx::PxShape*>(_shape);
		_pxShape->setSimulationFilterData({ _layer, 0, 0, 0 });
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
}
