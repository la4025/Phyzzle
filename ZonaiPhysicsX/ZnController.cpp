#include "PxPhysicsAPI.h"

#include "ZnUtil.h"
#include "ZnController.h"


namespace ZonaiPhysics
{
	ZnController::ZnController(physx::PxController* _controller)
	{
	}
	void ZnController::Move(Eigen::Vector3f _vec, float _dis, float _dt)
	{
		physx::PxControllerCollisionFlags flag = controller->move(EigenToPhysx(_vec), _dis, _dt, physx::PxControllerFilters());
		// flag 

	}
}