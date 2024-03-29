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
	\brief Returns true if this body is sleeping.

	When an actor does not move for a period of time, it is no longer simulated in order to save time. This state
	is called sleeping. However, because the object automatically wakes up when it is either touched by an awake object,
	or one of its properties is changed by the user, the entire sleep mechanism should be transparent to the user.

	In general, a dynamic rigid actor is guaranteed to be awake if at least one of the following holds:

	\li The wake counter is positive (see #setWakeCounter()).
	\li The linear or angular velocity is non-zero.
	\li A non-zero force or torque has been applied.

	If a dynamic rigid actor is sleeping, the following state is guaranteed:

	\li The wake counter is zero.
	\li The linear and angular velocity is zero.
	\li There is no force update pending.

	When an actor gets inserted into a scene, it will be considered asleep if all the points above hold, else it will be treated as awake.
	
	If an actor is asleep after the call to PxScene::fetchResults() returns, it is guaranteed that the pose of the actor 
	was not changed. You can use this information to avoid updating the transforms of associated objects.

	\note A kinematic actor is asleep unless a target pose has been set (in which case it will stay awake until two consecutive 
	simulation steps without a target pose being set have passed). The wake counter will get set to zero or to the reset value 
	#PxSceneDesc::wakeCounterResetValue in the case where a target pose has been set to be consistent with the definitions above.

	\note It is invalid to use this method if the actor has not been added to a scene already.

	\note It is not allowed to use this method while the simulation is running.

	\return True if the actor is sleeping.

	@see isSleeping() wakeUp() putToSleep()  getSleepThreshold()
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
	\brief Sets the linear velocity of the actor.

	Note that if you continuously set the velocity of an actor yourself,
	forces such as gravity or friction will not be able to manifest themselves, because forces directly
	influence only the velocity/momentum of an actor.

	<b>Default:</b> (0.0, 0.0, 0.0)

	<b>Sleeping:</b> This call wakes the actor if it is sleeping, and the autowake parameter is true (default) or the
	new velocity is non-zero.

	\note It is invalid to use this method if PxActorFlag::eDISABLE_SIMULATION is set.

	\note The linear velocity is applied with respect to the rigid dynamic's center of mass and not the actor frame origin.

	\param[in] linVel New linear velocity of actor. <b>Range:</b> velocity vector
	\param[in] autowake Whether to wake the object up if it is asleep. If true and the current wake counter value is
	smaller than #PxSceneDesc::wakeCounterResetValue it will get increased to the reset value.

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
	\brief Sets the angular velocity of the actor.

	Note that if you continuously set the angular velocity of an actor yourself,
	forces such as friction will not be able to rotate the actor, because forces directly influence only the velocity/momentum.

	<b>Default:</b> (0.0, 0.0, 0.0)

	<b>Sleeping:</b> This call wakes the actor if it is sleeping, and the autowake parameter is true (default) or the
	new velocity is non-zero.

	\note It is invalid to use this method if PxActorFlag::eDISABLE_SIMULATION is set.

	\param[in] angVel New angular velocity of actor. <b>Range:</b> angular velocity vector
	\param[in] autowake Whether to wake the object up if it is asleep. If true and the current wake counter value is
	smaller than #PxSceneDesc::wakeCounterResetValue it will get increased to the reset value.

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
	\brief Sets the solver iteration counts for the body. 
	
	The solver iteration count determines how accurately joints and contacts are resolved. 
	If you are having trouble with jointed bodies oscillating and behaving erratically, then
	setting a higher position iteration count may improve their stability.

	If intersecting bodies are being depenetrated too violently, increase the number of velocity 
	iterations. More velocity iterations will drive the relative exit velocity of the intersecting 
	objects closer to the correct value given the restitution.

	<b>Default:</b> 4 position iterations, 1 velocity iteration

	\param[in] minPositionIters Number of position iterations the solver should perform for this body. <b>Range:</b> [1,255]
	\param[in] minVelocityIters Number of velocity iterations the solver should perform for this body. <b>Range:</b> [0,255]

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
