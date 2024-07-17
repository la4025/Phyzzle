// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef PX_RIGID_DYNAMIC_H
#define PX_RIGID_DYNAMIC_H
/** \addtogroup physics
@{
*/

#include "PxRigidBody.h"

#if !PX_DOXYGEN
namespace physx
{
#endif


/**
\brief Collection of flags providing a mechanism to lock motion along/around a specific axis.

@see PxRigidDynamic.setRigidDynamicLockFlag(), PxRigidBody.getRigidDynamicLockFlags()
*/
struct PxRigidDynamicLockFlag
{
	enum Enum
	{
		eLOCK_LINEAR_X = (1 << 0),
		eLOCK_LINEAR_Y = (1 << 1),
		eLOCK_LINEAR_Z = (1 << 2),
		eLOCK_ANGULAR_X = (1 << 3),
		eLOCK_ANGULAR_Y = (1 << 4),
		eLOCK_ANGULAR_Z = (1 << 5)
	};
};

typedef PxFlags<PxRigidDynamicLockFlag::Enum, PxU8> PxRigidDynamicLockFlags;
PX_FLAGS_OPERATORS(PxRigidDynamicLockFlag::Enum, PxU8)

/**
\brief PxRigidDynamic represents a dynamic rigid simulation object in the physics SDK.

<h3>Creation</h3>
Instances of this class are created by calling #PxPhysics::createRigidDynamic() and deleted with #release().


<h3>Visualizations</h3>
\li #PxVisualizationParameter::eACTOR_AXES
\li #PxVisualizationParameter::eBODY_AXES
\li #PxVisualizationParameter::eBODY_MASS_AXES
\li #PxVisualizationParameter::eBODY_LIN_VELOCITY
\li #PxVisualizationParameter::eBODY_ANG_VELOCITY

@see PxRigidBody  PxPhysics.createRigidDynamic()  release()
*/

class PxRigidDynamic : public PxRigidBody
{
public:
	// Runtime modifications


/************************************************************************************************/
/** @name Kinematic Actors
*/

	/**
	\brief 게임 세계에서 동적으로 제어되는 액터를 이동시킵니다.

	PxRigidBodyFlag::eKINEMATIC 플래그를 사용하여 동적 액터를 키네마틱으로 설정합니다.
	setRigidBodyFlag()를 사용하세요.

	이동 명령은 몸체를 원하는 자세로 이동시키는 속도를 결과로 합니다.
	이동이 단일 시간 단계 동안 수행된 후 속도가 0으로 반환됩니다.
	따라서 키네마틱 액터를 계속해서 호출하여 경로를 따라 이동하도록해야 합니다.

	이 함수는 다음 시뮬레이션 단계가 처리될 때까지 이동 대상을 저장하기만 합니다.
	연속 호출은 단순히 저장된 대상 변수를 덮어씁니다.

	운동은 항상 완전히 수행됩니다.

	\note:	이 메소드를 사용하는 것은 액터가 이미 씬에 추가되어 있지 않거나
			PxActorFlag::eDISABLE_SIMULATION 플래그가 설정된 경우에는 잘못됩니다.

	<b>슬리핑:</b>
			이 호출은 액터가 잠들어 있으면 깨우고
			wake 카운터를 #PxSceneDesc::wakeCounterResetValue로 설정합니다.

	\param[in]	destination 
				키네마틱 액터의 원하는 자세입니다. 전역 프레임 내에서. <b>범위:</b> 강체 변환.

	@see getKinematicTarget() PxRigidBodyFlag setRigidBodyFlag()
	*/
	virtual		void				setKinematicTarget(const PxTransform& destination) = 0;

	/**
	\brief Get target pose of a kinematically controlled dynamic actor.

	\param[out] target Transform to write the target pose to. Only valid if the method returns true.
	\return True if the actor is a kinematically controlled dynamic and the target has been set, else False.

	@see setKinematicTarget() PxRigidBodyFlag setRigidBodyFlag()
	*/
	virtual		bool				getKinematicTarget(PxTransform& target)	const	= 0;


/************************************************************************************************/
/** @name Sleeping
*/

	/**
	\brief 이 물체가 수면 상태인지 여부를 반환합니다.

	물체가 일정 시간 동안 움직이지 않으면 시뮬레이션에서 제외되어 시간을 절약합니다. 이 상태를 수면 상태라고 합니다.
	그러나 물체가 깨어있는 물체에 의해 접촉되거나 사용자가 속성 중 하나를 변경하면 자동으로 깨어나기 때문에,
	전체 수면 메커니즘은 사용자에게 투명해야 합니다.

	일반적으로, 동적 강체 물체는 다음 조건 중 하나 이상이 충족되면 깨어있는 상태임이 보장됩니다:

	\li 깨우기 카운터가 양수입니다 (참조 #setWakeCounter()).
	\li 선형 또는 각속도가 0이 아닙니다.
	\li 0이 아닌 힘이나 토크가 적용되었습니다.

	동적 강체 물체가 수면 상태인 경우, 다음 상태가 보장됩니다:

	\li 깨우기 카운터가 0입니다.
	\li 선형 및 각속도가 0입니다.
	\li 대기 중인 힘 업데이트가 없습니다.

	물체가 장면에 삽입되면 위의 모든 조건이 충족되면 수면 상태로 간주되고, 그렇지 않으면 깨어있는 상태로 처리됩니다.

	PxScene::fetchResults() 호출 후 물체가 수면 상태인 경우, 물체의 자세가 변경되지 않았음을 보장합니다.
	이 정보를 사용하여 관련 객체의 변환을 업데이트하지 않을 수 있습니다.

	\note 목표 자세가 설정되지 않은 경우, 키네마틱 물체는 수면 상태입니다 
	(목표 자세가 설정되지 않은 경우 두 번의 연속 시뮬레이션 단계가 지나갈 때까지 깨어있는 상태로 유지됩니다). 
	목표 자세가 설정된 경우, 깨우기 카운터는 0 또는 재설정 값 #PxSceneDesc::wakeCounterResetValue로 설정되어
	위의 정의와 일관성을 유지합니다.

	\note 물체가 아직 장면에 추가되지 않은 경우 이 메서드를 사용하는 것은 유효하지 않습니다.

	\note 시뮬레이션이 실행 중인 동안 이 메서드를 사용하는 것은 허용되지 않습니다.

	\return 물체가 수면 상태인 경우 True를 반환합니다.

	@see isSleeping() wakeUp() putToSleep() getSleepThreshold()
	*/
	virtual		bool				isSleeping() const = 0;

    /**
	\brief Sets the mass-normalized kinetic energy threshold below which an actor may go to sleep.

	Actors whose kinetic energy divided by their mass is below this threshold will be candidates for sleeping.

	<b>Default:</b> 5e-5f * PxTolerancesScale::speed * PxTolerancesScale::speed

	\param[in] threshold Energy below which an actor may go to sleep. <b>Range:</b> [0, PX_MAX_F32)

	@see isSleeping() getSleepThreshold() wakeUp() putToSleep() PxTolerancesScale
	*/
	virtual		void				setSleepThreshold(PxReal threshold) = 0;

	/**
	\brief Returns the mass-normalized kinetic energy below which an actor may go to sleep.

	\return The energy threshold for sleeping.

	@see isSleeping() wakeUp() putToSleep() setSleepThreshold()
	*/
	virtual		PxReal				getSleepThreshold() const = 0;

	 /**
	\brief Sets the mass-normalized kinetic energy threshold below which an actor may participate in stabilization.

	Actors whose kinetic energy divided by their mass is above this threshold will not participate in stabilization.

	This value has no effect if PxSceneFlag::eENABLE_STABILIZATION was not enabled on the PxSceneDesc.

	<b>Default:</b> 1e-5f * PxTolerancesScale::speed * PxTolerancesScale::speed

	\param[in] threshold Energy below which an actor may participate in stabilization. <b>Range:</b> [0,inf)

	@see  getStabilizationThreshold() PxSceneFlag::eENABLE_STABILIZATION
	*/
	virtual		void				setStabilizationThreshold(PxReal threshold) = 0;

	/**
	\brief Returns the mass-normalized kinetic energy below which an actor may participate in stabilization.

	Actors whose kinetic energy divided by their mass is above this threshold will not participate in stabilization. 

	\return The energy threshold for participating in stabilization.

	@see setStabilizationThreshold() PxSceneFlag::eENABLE_STABILIZATION
	*/
	virtual		PxReal				getStabilizationThreshold() const = 0;


	/**
	\brief Reads the PxRigidDynamic lock flags.

	See the list of flags #PxRigidDynamicLockFlag

	\return The values of the PxRigidDynamicLock flags.

	@see PxRigidDynamicLockFlag setRigidDynamicLockFlag()
	*/
	virtual		PxRigidDynamicLockFlags getRigidDynamicLockFlags() const = 0;

	/**
	\brief Raises or clears a particular rigid dynamic lock flag.

	See the list of flags #PxRigidDynamicLockFlag

	<b>Default:</b> no flags are set


	\param[in] flag		The PxRigidDynamicLockBody flag to raise(set) or clear. See #PxRigidBodyFlag.
	\param[in] value	The new boolean value for the flag.

	@see PxRigidDynamicLockFlag getRigidDynamicLockFlags()
	*/
	virtual		void				setRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum flag, bool value) = 0;
	virtual		void				setRigidDynamicLockFlags(PxRigidDynamicLockFlags flags) = 0;
	
	/**
	\brief Retrieves the linear velocity of an actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\note The linear velocity is reported with respect to the rigid dynamic's center of mass and not the actor frame origin.

	\return The linear velocity of the actor.

	@see PxRigidDynamic.setLinearVelocity() getAngularVelocity()
	*/
	virtual		PxVec3			getLinearVelocity()		const = 0;

	/**
	\brief 물체의 선형 속도를 설정합니다.

	물체의 속도를 계속해서 직접 설정할 경우, 중력이나 마찰과 같은 힘이 나타나지 않을 수 있습니다.
	이는 힘이 물체의 속도/운동량에 직접적으로 영향을 미치기 때문입니다.

	<b>기본값:</b> (0.0, 0.0, 0.0)

	<b>수면 상태:</b>	이 호출은 물체가 수면 상태인 경우, autowake 매개변수가 true(기본값)이거나
						새로운 속도가 0이 아닌 경우 물체를 깨웁니다.

	\note PxActorFlag::eDISABLE_SIMULATION이 설정된 경우 이 메서드를 사용하는 것은 유효하지 않습니다.

	\note 선형 속도는 물체 프레임 원점이 아닌 강체 동역학의 질량 중심에 대해 적용됩니다.

	\param[in] linVel	새로운 물체의 선형 속도입니다. <b>범위:</b> 속도 벡터
	\param[in] autowake 물체가 수면 상태인 경우 깨울지 여부를 설정합니다. true인 경우,
						현재 깨우기 카운터 값이 #PxSceneDesc::wakeCounterResetValue보다 작으면
						해당 값을 재설정 값으로 증가시킵니다.

	@see getLinearVelocity() setAngularVelocity()
	*/
	virtual		void			setLinearVelocity(const PxVec3& linVel, bool autowake = true) = 0;

	/**
	\brief Retrieves the angular velocity of the actor.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\return The angular velocity of the actor.

	@see PxRigidDynamic.setAngularVelocity() getLinearVelocity()
	*/
	virtual		PxVec3			getAngularVelocity()	const = 0;

	/**
	\brief 물체의 각속도를 설정합니다.

	물체의 각속도를 계속해서 직접 설정할 경우, 마찰과 같은 힘이 물체를 회전시키지 못할 수 있습니다.
	이는 힘이 물체의 속도/운동량에만 직접적으로 영향을 미치기 때문입니다.

	<b>기본값:</b> (0.0, 0.0, 0.0)

	<b>수면 상태:</b> 이 호출은 물체가 수면 상태인 경우,
	autowake 매개변수가 true(기본값)이거나 새로운 속도가 0이 아닌 경우 물체를 깨웁니다.

	\note PxActorFlag::eDISABLE_SIMULATION이 설정된 경우 이 메서드를 사용하는 것은 유효하지 않습니다.

	\param[in] angVel	새로운 물체의 각속도입니다. <b>범위:</b> 각속도 벡터
	\param[in] autowake 물체가 수면 상태인 경우 깨울지 여부를 설정합니다.
						true인 경우, 현재 깨우기 카운터 값이 #PxSceneDesc::wakeCounterResetValue보다 작으면
						해당 값을 재설정 값으로 증가시킵니다.

	@see getAngularVelocity() setLinearVelocity()
	*/
	virtual		void			setAngularVelocity(const PxVec3& angVel, bool autowake = true) = 0;
	/**
	\brief Sets the wake counter for the actor.

	The wake counter value determines the minimum amount of time until the body can be put to sleep. Please note
	that a body will not be put to sleep if the energy is above the specified threshold (see #setSleepThreshold())
	or if other awake bodies are touching it.

	\note Passing in a positive value will wake the actor up automatically.

	\note It is invalid to use this method for kinematic actors since the wake counter for kinematics is defined
	based on whether a target pose has been set (see the comment in #isSleeping()).

	\note It is invalid to use this method if PxActorFlag::eDISABLE_SIMULATION is set.

	<b>Default:</b> 0.4 (which corresponds to 20 frames for a time step of 0.02)

	\param[in] wakeCounterValue Wake counter value. <b>Range:</b> [0, PX_MAX_F32)

	@see isSleeping() getWakeCounter()
	*/
	virtual		void				setWakeCounter(PxReal wakeCounterValue) = 0;

	/**
	\brief Returns the wake counter of the actor.

	\note It is not allowed to use this method while the simulation is running.

	\return The wake counter of the actor.

	@see isSleeping() setWakeCounter()
	*/
	virtual		PxReal				getWakeCounter() const = 0;

	/**
	\brief Wakes up the actor if it is sleeping.

	The actor will get woken up and might cause other touching actors to wake up as well during the next simulation step.

	\note This will set the wake counter of the actor to the value specified in #PxSceneDesc::wakeCounterResetValue.

	\note It is invalid to use this method if the actor has not been added to a scene already or if PxActorFlag::eDISABLE_SIMULATION is set.

	\note It is invalid to use this method for kinematic actors since the sleep state for kinematics is defined
	based on whether a target pose has been set (see the comment in #isSleeping()).

	@see isSleeping() putToSleep()
	*/
	virtual		void				wakeUp() = 0;

	/**
	\brief 액터를 슬립 상태로 만듭니다.

	만약 다른 슬립하지 않는 액터에 의해 터치되지 않는다면, 
	다음 시뮬레이션 단계에서도 액터는 슬립 상태로 유지됩니다.

	참고: 적용된 힘은 지워지고, 액터의 속도와 깨어 있는 카운터(wake counter)는 0으로 설정됩니다.

	참고: 이 메서드는 액터가 이미 시뮬레이션에 추가되지 않았거나 PxActorFlag::eDISABLE_SIMULATION이 설정된 경우에 사용할 수 없습니다.

	참고: 키네매틱 액터의 경우 이 메서드를 사용하는 것은 무효입니다. 
	키네매틱 액터의 슬립 상태는 대상 포즈가 설정되었는지 여부에 따라 정의됩니다(참조: #isSleeping()의 주석).

	@see isSleeping() wakeUp()
	*/
	virtual		void				putToSleep() = 0;

/************************************************************************************************/

	/**
	\brief 물체에 대한 솔버 반복 횟수를 설정합니다.

	솔버 반복 횟수는 조인트와 접촉이 얼마나 정확하게 해결되는지를 결정합니다.
	조인트가 연결된 물체가 진동하거나 불규칙하게 동작하는 경우, 위치 반복 횟수를 높이면 안정성을 개선할 수 있습니다.

	교차하는 물체가 너무 강하게 분리되는 경우, 속도 반복 횟수를 늘리십시오.
	속도 반복 횟수가 많을수록 교차하는 물체의 상대적 이탈 속도가 복원력에 따라 올바른 값에 더 가까워집니다.

	<b>기본값:</b> 위치 반복 4회, 속도 반복 1회

	\param[in] minPositionIters 이 물체에 대해 솔버가 수행해야 하는 위치 반복 횟수입니다. <b>범위:</b> [1,255]
	\param[in] minVelocityIters 이 물체에 대해 솔버가 수행해야 하는 속도 반복 횟수입니다. <b>범위:</b> [0,255]

	@see getSolverIterationCounts()
	*/
	virtual		void				setSolverIterationCounts(PxU32 minPositionIters, PxU32 minVelocityIters = 1) = 0;

	/**
	\brief Retrieves the solver iteration counts.

	@see setSolverIterationCounts()
	*/
	virtual		void				getSolverIterationCounts(PxU32& minPositionIters, PxU32& minVelocityIters) const = 0;

	/**
	\brief Retrieves the force threshold for contact reports.

	The contact report threshold is a force threshold. If the force between 
	two actors exceeds this threshold for either of the two actors, a contact report 
	will be generated according to the contact report threshold flags provided by
	the filter shader/callback.
	See #PxPairFlag.

	The threshold used for a collision between a dynamic actor and the static environment is 
    the threshold of the dynamic actor, and all contacts with static actors are summed to find 
    the total normal force.

	<b>Default:</b> PX_MAX_F32

	\return Force threshold for contact reports.

	@see setContactReportThreshold PxPairFlag PxSimulationFilterShader PxSimulationFilterCallback
	*/
	virtual     PxReal				getContactReportThreshold() const = 0;

	/**
	\brief Sets the force threshold for contact reports.

	See #getContactReportThreshold().

	\param[in] threshold Force threshold for contact reports. <b>Range:</b> [0, PX_MAX_F32)

	@see getContactReportThreshold PxPairFlag
	*/
	virtual     void				setContactReportThreshold(PxReal threshold) = 0;

	virtual		const char*			getConcreteTypeName() const { return "PxRigidDynamic"; }

protected:
	PX_INLINE						PxRigidDynamic(PxType concreteType, PxBaseFlags baseFlags) : PxRigidBody(concreteType, baseFlags) { }
	PX_INLINE						PxRigidDynamic(PxBaseFlags baseFlags) : PxRigidBody(baseFlags) {}
	virtual							~PxRigidDynamic() {}
	virtual		bool				isKindOf(const char* name) const { PX_IS_KIND_OF(name, "PxRigidDynamic", PxRigidBody); }

};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
