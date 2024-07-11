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

#ifndef PX_CONTROLLER_H
#define PX_CONTROLLER_H
/** \addtogroup character
  @{
*/

#include "characterkinematic/PxExtended.h"
#include "characterkinematic/PxControllerObstacles.h"
#include "PxQueryFiltering.h"
#include "foundation/PxErrorCallback.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief The type of controller, eg box, sphere or capsule.
*/
struct PxControllerShapeType
{
	enum Enum
	{
		/**
		\brief A box controller.

		@see PxBoxController PxBoxControllerDesc
		*/
		eBOX,

		/**
		\brief A capsule controller

		@see PxCapsuleController PxCapsuleControllerDesc
		*/
		eCAPSULE,

		eFORCE_DWORD = 0x7fffffff
	};
};

class PxShape;
class PxScene;
class PxController;
class PxRigidDynamic;
class PxMaterial;
struct PxFilterData;
class PxQueryFilterCallback;
class PxControllerBehaviorCallback;
class PxObstacleContext;
class PxObstacle;

/**
\brief CCT가 비보행 부분과 상호작용하는 방식을 지정합니다.

이것은 slopeLimit이 0이 아닐 때만 사용됩니다. 현재 정적 액터에만 사용 가능하며, 구형(sphere) 또는 캡슐형(capsule)에는 지원되지 않습니다.
*/
struct PxControllerNonWalkableMode
{
	enum Enum
	{
		ePREVENT_CLIMBING,						//!< Stops character from climbing up non-walkable slopes, but doesn't move it otherwise
		ePREVENT_CLIMBING_AND_FORCE_SLIDING		//!< Stops character from climbing up non-walkable slopes, and forces it to slide down those slopes
	};
};

/**
\brief 캐릭터가 어느 쪽과 충돌하고 있는지 지정합니다.
*/
struct PxControllerCollisionFlag
{
	enum Enum
	{
		eCOLLISION_SIDES = (1 << 0),	//!< 캐릭터가 옆면과 충돌하고 있습니다.
		eCOLLISION_UP = (1 << 1),		//!< 캐릭터가 위쪽과 충돌하고 있습니다.
		eCOLLISION_DOWN = (1 << 2)		//!< 캐릭터가 아래쪽과 충돌하고 있습니다.
	};
};

/**
\brief Bitfield that contains a set of raised flags defined in PxControllerCollisionFlag.

@see PxControllerCollisionFlag
*/
typedef PxFlags<PxControllerCollisionFlag::Enum, PxU8> PxControllerCollisionFlags;
PX_FLAGS_OPERATORS(PxControllerCollisionFlag::Enum, PxU8)

/**
\brief 컨트롤러의 내부 상태를 설명합니다.
*/
struct PxControllerState
{
	PxVec3				deltaXP;				//!< CCT가 서 있는/타고 있는 객체의 위치 변경 벡터입니다. 가변 시간 간격을 사용할 때 항상 CCT의 위치 변경과 일치하지는 않습니다.
	PxShape*			touchedShape;			//!< CCT가 서 있는 모양입니다.
	PxRigidActor*		touchedActor;			//!< 'touchedShape'을 소유하는 액터입니다.
	PxObstacleHandle	touchedObstacleHandle;	// CCT가 서 있는 장애물입니다.
	PxU32				collisionFlags;			//!< 마지막으로 알려진 충돌 플래그입니다 (PxControllerCollisionFlag).
	bool				standOnAnotherCCT;		//!< 다른 CCT 위에 서 있습니까?
	bool				standOnObstacle;		//!< 사용자 정의 장애물 위에 서 있습니까?
	bool				isMovingUp;				//!< CCT가 위로 이동 중인가요? (즉, 명시적인 점프)
};

/**
\brief Describes a controller's internal statistics.
*/
struct PxControllerStats
{
	PxU16			nbIterations;
	PxU16			nbFullUpdates;
	PxU16			nbPartialUpdates;
	PxU16			nbTessellation;
};

/**
\brief Describes a generic CCT hit.
*/
struct PxControllerHit
{
	PxController*	controller;		//!< Current controller
	PxExtendedVec3	worldPos;		//!< Contact position in world space
	PxVec3			worldNormal;	//!< Contact normal in world space
	PxVec3			dir;			//!< Motion direction
	PxF32			length;			//!< Motion length
};

/**
\brief Describes a hit between a CCT and a shape. Passed to onShapeHit()

@see PxUserControllerHitReport.onShapeHit()
*/
struct PxControllerShapeHit : public PxControllerHit
{
	PxShape*		shape;			//!< Touched shape
	PxRigidActor*	actor;			//!< Touched actor
	PxU32			triangleIndex;	//!< touched triangle index (only for meshes/heightfields)
};

/**
\brief Describes a hit between a CCT and another CCT. Passed to onControllerHit().

@see PxUserControllerHitReport.onControllerHit()
*/
struct PxControllersHit : public PxControllerHit
{
	PxController*	other;			//!< Touched controller
};

/**
\brief Describes a hit between a CCT and a user-defined obstacle. Passed to onObstacleHit().

@see PxUserControllerHitReport.onObstacleHit() PxObstacleContext
*/
struct PxControllerObstacleHit : public PxControllerHit
{
	const void*		userData;
};

/**
\brief User callback class for character controller events.

\note Character controller hit reports are only generated when move is called.

@see PxControllerDesc.callback
*/
class PxUserControllerHitReport
{
public:

	/**
	\brief Called when current controller hits a shape.

	This is called when the CCT moves and hits a shape. This will not be called when a moving shape hits a non-moving CCT.

	\param[in] hit Provides information about the hit.

	@see PxControllerShapeHit
	*/
	virtual void onShapeHit(const PxControllerShapeHit& hit) = 0;

	/**
	\brief Called when current controller hits another controller.

	\param[in] hit Provides information about the hit.

	@see PxControllersHit
	*/
	virtual void onControllerHit(const PxControllersHit& hit) = 0;

	/**
	\brief Called when current controller hits a user-defined obstacle.

	\param[in] hit Provides information about the hit.

	@see PxControllerObstacleHit PxObstacleContext
	*/
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) = 0;

protected:
	virtual ~PxUserControllerHitReport(){}
};


/**
\brief Dedicated filtering callback for CCT vs CCT.

This controls collisions between CCTs (one CCT vs anoter CCT).

To make each CCT collide against all other CCTs, just return true - or simply avoid defining a callback.
To make each CCT freely go through all other CCTs, just return false.
Otherwise create a custom filtering logic in this callback.

@see PxControllerFilters
*/
class PxControllerFilterCallback
{
public:
	virtual ~PxControllerFilterCallback(){}

	/**
	\brief Filtering method for CCT-vs-CCT.

	\param[in] a	First CCT
	\param[in] b	Second CCT
	\return true to keep the pair, false to filter it out
	*/
	virtual bool filter(const PxController& a, const PxController& b) = 0;
};

/**
\brief Filtering data for "move" call.

This class contains all filtering-related parameters for the PxController::move() call.

Collisions between a CCT and the world are filtered using the mFilterData, mFilterCallback and mFilterFlags
members. These parameters are internally passed to PxScene::overlap() to find objects touched by the CCT.
Please refer to the PxScene::overlap() documentation for details.

Collisions between a CCT and another CCT are filtered using the mCCTFilterCallback member. If this filter
callback is not defined, none of the CCT-vs-CCT collisions are filtered, and each CCT will collide against
all other CCTs.

\note PxQueryFlag::eANY_HIT and PxQueryFlag::eNO_BLOCK are ignored in mFilterFlags.

@see PxController.move() PxControllerFilterCallback
*/
class PxControllerFilters
{
	public:

	PX_INLINE					PxControllerFilters(const PxFilterData* filterData=NULL, PxQueryFilterCallback* cb=NULL, PxControllerFilterCallback* cctFilterCb=NULL) :
									mFilterData			(filterData),
									mFilterCallback		(cb),
									mFilterFlags		(PxQueryFlag::eSTATIC|PxQueryFlag::eDYNAMIC|PxQueryFlag::ePREFILTER),
									mCCTFilterCallback	(cctFilterCb)
								{}

	// CCT-vs-shapes:
	const PxFilterData* mFilterData;					//!< 내부 PxQueryFilterData 구조체를 위한 데이터입니다. PxScene::overlap() 호출에 전달됩니다.
														//!< 이 값이 NULL인 경우, 기본 PxFilterData가 사용됩니다.
	PxQueryFilterCallback* mFilterCallback;				//!< 사용자 정의 필터 논리 (NULL일 수 있음). PxScene::overlap() 호출에 전달됩니다.
	PxQueryFlags				mFilterFlags;			//!< 내부 PxQueryFilterData 구조체를 위한 플래그입니다. PxScene::overlap() 호출에 전달됩니다.
	// CCT-vs-CCT:
	PxControllerFilterCallback* mCCTFilterCallback;		//!< CCT-vs-CCT 필터 콜백입니다. NULL인 경우, 모든 CCT-vs-CCT 충돌이 유지됩니다.
};

/**
\brief Descriptor class for a character controller.

@see PxBoxController PxCapsuleController
*/
class PxControllerDesc
{
public:

	/**
	\brief returns true if the current settings are valid

	\return True if the descriptor is valid.
	*/
	PX_INLINE virtual	bool			isValid()		const;

	/**
	\brief Returns the character controller type

	\return The controllers type.

	@see PxControllerType PxCapsuleControllerDesc PxBoxControllerDesc
	*/
	PX_INLINE	PxControllerShapeType::Enum		getType()		const	{ return mType;		}

	/**
	\brief The position of the character

	\note The character's initial position must be such that it does not overlap the static geometry.

	<b>Default:</b> Zero
	*/
	PxExtendedVec3						position;

	/**
	\brief Specifies the 'up' direction

	In order to provide stepping functionality the SDK must be informed about the up direction.

	<b>Default:</b> (0, 1, 0)

	*/
	PxVec3								upDirection;

	/**
	\brief The maximum slope which the character can walk up.

	In general it is desirable to limit where the character can walk, in particular it is unrealistic
	for the character to be able to climb arbitary slopes.

	The limit is expressed as the cosine of desired limit angle. A value of 0 disables this feature.

	\warning It is currently enabled for static actors only (not for dynamic/kinematic actors), and not supported for spheres or capsules.

	<b>Default:</b> 0.707

	@see upDirection invisibleWallHeight maxJumpHeight
	*/
	PxF32								slopeLimit;

	/**
	\brief Height of invisible walls created around non-walkable triangles

	The library can automatically create invisible walls around non-walkable triangles defined
	by the 'slopeLimit' parameter. This defines the height of those walls. If it is 0.0, then
	no extra triangles are created.

	<b>Default:</b> 0.0

	@see upDirection slopeLimit maxJumpHeight
	*/
	PxF32								invisibleWallHeight;

	/**
	\brief Maximum height a jumping character can reach

	This is only used if invisible walls are created ('invisibleWallHeight' is non zero).

	When a character jumps, the non-walkable triangles he might fly over are not found
	by the collision queries (since the character's bounding volume does not touch them).
	Thus those non-walkable triangles do not create invisible walls, and it is possible
	for a jumping character to land on a non-walkable triangle, while he wouldn't have
	reached that place by just walking.

	The 'maxJumpHeight' variable is used to extend the size of the collision volume
	downward. This way, all the non-walkable triangles are properly found by the collision
	queries and it becomes impossible to 'jump over' invisible walls.

	If the character in your game can not jump, it is safe to use 0.0 here. Otherwise it
	is best to keep this value as small as possible, since a larger collision volume
	means more triangles to process.

	<b>Default:</b> 0.0

	@see upDirection slopeLimit invisibleWallHeight
	*/
	PxF32								maxJumpHeight;

	/**
	\brief The contact offset used by the controller.

	Specifies a skin around the object within which contacts will be generated.
	Use it to avoid numerical precision issues.

	This is dependant on the scale of the users world, but should be a small, positive 
	non zero value.

	<b>Default:</b> 0.1
	*/
	PxF32								contactOffset;

	/**
	\brief Defines the maximum height of an obstacle which the character can climb.

	A small value will mean that the character gets stuck and cannot walk up stairs etc, 
	a value which is too large will mean that the character can climb over unrealistically 
	high obstacles.

	<b>Default:</b> 0.5

	@see upDirection 
	*/
	PxF32								stepOffset;

	/**
	\brief Density of underlying kinematic actor

	The CCT creates a PhysX's kinematic actor under the hood. This controls its density.

	<b>Default:</b> 10.0
	*/
	PxF32								density;

	/**
	\brief Scale coefficient for underlying kinematic actor

	The CCT creates a PhysX's kinematic actor under the hood. This controls its scale factor.
	This should be a number a bit smaller than 1.0.

	<b>Default:</b> 0.8
	*/
	PxF32								scaleCoeff;

	/**
	\brief Cached volume growth

	Amount of space around the controller we cache to improve performance. This is a scale factor
	that should be higher than 1.0f but not too big, ideally lower than 2.0f.

	<b>Default:</b> 1.5
	*/
	PxF32								volumeGrowth;

	/**
	\brief Specifies a user report callback.

	This report callback is called when the character collides with shapes and other characters.

	Setting this to NULL disables the callback.

	<b>Default:</b> NULL

	@see PxUserControllerHitReport
	*/
	PxUserControllerHitReport*			reportCallback;

	/**
	\brief Specifies a user behavior callback.

	This behavior callback is called to customize the controller's behavior w.r.t. touched shapes.

	Setting this to NULL disables the callback.

	<b>Default:</b> NULL

	@see PxControllerBehaviorCallback
	*/
	PxControllerBehaviorCallback*		behaviorCallback;

	/**
	\brief The non-walkable mode controls if a character controller slides or not on a non-walkable part.

	This is only used when slopeLimit is non zero.

	<b>Default:</b> PxControllerNonWalkableMode::ePREVENT_CLIMBING

	@see PxControllerNonWalkableMode
	*/
	PxControllerNonWalkableMode::Enum	nonWalkableMode;

	/**
	\brief The material for the actor associated with the controller.
	
	The controller internally creates a rigid body actor. This parameter specifies the material of the actor.

	<b>Default:</b> NULL

	@see PxMaterial
	*/
	PxMaterial*							material;

	/**
	\brief Use a deletion listener to get informed about released objects and clear internal caches if needed.

	If a character controller registers a deletion listener, it will get informed about released objects. That allows the
	controller to invalidate cached data that connects to a released object. If a deletion listener is not
	registered, PxController::invalidateCache has to be called manually after objects have been released.

	@see PxController::invalidateCache

	<b>Default:</b> true
	*/
	bool								registerDeletionListener;

	/**
	\brief Client ID for associated actor.

	@see PxClientID PxActor::setOwnerClient

	<b>Default:</b> PX_DEFAULT_CLIENT
	*/
	PxClientID							clientID;

	/**
	\brief User specified data associated with the controller.

	<b>Default:</b> NULL
	*/
	void*								userData;

protected:
	const PxControllerShapeType::Enum	mType;		//!< The type of the controller. This gets set by the derived class' ctor, the user should not have to change it.

	/**
	\brief constructor sets to default.
	*/
	PX_INLINE							PxControllerDesc(PxControllerShapeType::Enum);
	PX_INLINE virtual					~PxControllerDesc();

	/**
	\brief copy constructor.
	*/
	PX_INLINE							PxControllerDesc(const PxControllerDesc&);

	/**
	\brief assignment operator.
	*/
	PX_INLINE PxControllerDesc&			operator=(const PxControllerDesc&);

	PX_INLINE void						copy(const PxControllerDesc&);
};

PX_INLINE PxControllerDesc::PxControllerDesc(PxControllerShapeType::Enum t) :
	position					(PxExtended(0.0), PxExtended(0.0), PxExtended(0.0)),
	upDirection					(0.0f, 1.0f, 0.0f),
	slopeLimit					(0.707f),
	invisibleWallHeight			(0.0f),
	maxJumpHeight				(0.0f),
	contactOffset				(0.1f),
	stepOffset					(0.5f),
	density						(10.0f),
	scaleCoeff					(0.8f),
	volumeGrowth				(1.5f),
	reportCallback				(NULL),
	behaviorCallback			(NULL),
	nonWalkableMode				(PxControllerNonWalkableMode::ePREVENT_CLIMBING),
	material					(NULL),
	registerDeletionListener	(true),
	clientID					(PX_DEFAULT_CLIENT),
	userData					(NULL),
	mType						(t)
{
}

PX_INLINE PxControllerDesc::PxControllerDesc(const PxControllerDesc& other) : mType(other.mType)
{
	copy(other);
}

PX_INLINE PxControllerDesc& PxControllerDesc::operator=(const PxControllerDesc& other)
{
	copy(other);
	return *this;
}

PX_INLINE void PxControllerDesc::copy(const PxControllerDesc& other)
{
	upDirection					= other.upDirection;
	slopeLimit					= other.slopeLimit;
	contactOffset				= other.contactOffset;
	stepOffset					= other.stepOffset;
	density						= other.density;
	scaleCoeff					= other.scaleCoeff;
	volumeGrowth				= other.volumeGrowth;
	reportCallback				= other.reportCallback;
	behaviorCallback			= other.behaviorCallback;
	userData					= other.userData;
	nonWalkableMode				= other.nonWalkableMode;
	position.x					= other.position.x;
	position.y					= other.position.y;
	position.z					= other.position.z;
	material					= other.material;
	invisibleWallHeight			= other.invisibleWallHeight;
	maxJumpHeight				= other.maxJumpHeight;
	registerDeletionListener	= other.registerDeletionListener;
	clientID					= other.clientID;
}

PX_INLINE PxControllerDesc::~PxControllerDesc()
{
}

PX_INLINE bool PxControllerDesc::isValid() const
{
	if(		mType!=PxControllerShapeType::eBOX
		&&	mType!=PxControllerShapeType::eCAPSULE)
		return false;
	if(scaleCoeff<0.0f)
		return false;
	if(volumeGrowth<1.0f)
		return false;
	if(density<0.0f)
		return false;
	if(slopeLimit<0.0f)
		return false;
	if(stepOffset<0.0f)
		return false;
	if(contactOffset<=0.0f)
		return false;
	if(!material)
		return false;
	if(!toVec3(position).isFinite())
		return false;   //the float version needs to be finite otherwise actor creation will fail.
	
	return true;
}


/**
\brief 캐릭터 컨트롤러의 기본 클래스입니다.

@see PxCapsuleController PxBoxController
*/
class PxController
{
public:
	/**
	\brief Return the type of controller

	@see PxControllerType
	*/
	virtual		PxControllerShapeType::Enum	getType()		const			= 0;

	/**
	\brief Releases the controller.
	*/
	virtual		void					release() = 0;

	/**
	\brief "충돌하고 슬라이드" 알고리즘을 사용하여 캐릭터를 이동시킵니다.

	\param[in]	disp		이동 벡터
	\param[in]	minDist		고려할 최소 이동 거리. 이동 거리가 더 작으면 캐릭터는 이동하지 않습니다.
							이는 잔여 이동 거리가 작을 때 재귀적인 이동 알고리즘을 중지하는 데 사용됩니다.
	\param[in]	elapsedTime 마지막 호출 이후 경과한 시간
	\param[in]	filters		이동에 대한 사용자 정의 필터
	\param[in]	obstacles	CCT가 충돌해야 할 가능한 추가 장애물
	\return		충돌 플래그, ::PxControllerCollisionFlags의 집합
	*/
	virtual		PxControllerCollisionFlags	move(const PxVec3& disp, PxF32 minDist, PxF32 elapsedTime, const PxControllerFilters& filters, const PxObstacleContext* obstacles=NULL) = 0;

	/**
	\brief 컨트롤러의 위치를 설정합니다.

	이 함수로 제어되는 위치는 충돌 모양의 중심입니다.

	\warning 이 함수는 '텔레포트' 기능으로 충돌을 확인하지 않습니다.
	\warning 캐릭터의 위치는 정적 지오메트리와 겹치지 않아야 합니다.

	일반적인 조건에서 캐릭터를 이동시키려면 #move() 함수를 사용하세요.

	\param[in] position 컨트롤러의 새로운 (중심) 위치입니다.
	\return 현재 항상 true를 반환합니다.

	@see PxControllerDesc.position getPosition() getFootPosition() setFootPosition() move()
	*/
	virtual		bool					setPosition(const PxExtendedVec3& position) = 0;

	/**
	\brief 컨트롤러의 실제 위치를 가져옵니다.

	이 함수로 가져오는 위치는 충돌 모양의 중심입니다. 모양의 하단 위치, 즉 발 위치를 가져오려면 getFootPosition() 함수를 사용하세요.

	이 위치는 move() 호출로 업데이트됩니다. move()를 호출하지 않고 이 메서드를 호출하면 마지막 위치나 컨트롤러의 초기 위치가 반환됩니다.

	\return 컨트롤러의 중심 위치

	@see PxControllerDesc.position setPosition() getFootPosition() setFootPosition() move()
	*/
	virtual		const PxExtendedVec3&	getPosition()			const	= 0;

	/**
	\brief 컨트롤러의 발 위치를 설정합니다.

	이 함수로 제어되는 위치는 충돌 모양의 하단, 즉 발 위치입니다.

	\note 발 위치는 접촉 오프셋을 고려합니다.

	\warning 이 함수는 '텔레포트' 기능으로 충돌을 확인하지 않습니다.

	일반적인 조건에서 캐릭터를 이동시키려면 #move() 함수를 사용하세요.

	\param[in] position 컨트롤러의 새로운 (하단) 위치입니다.
	\return 현재 항상 true를 반환합니다.

	@see PxControllerDesc.position setPosition() getPosition() getFootPosition() move()
	*/
	virtual		bool					setFootPosition(const PxExtendedVec3& position) = 0;

	/**
	\brief Retrieve the "foot" position of the controller, i.e. the position of the bottom of the CCT's shape.

	\note The foot position takes the contact offset into account

	\return The controller's foot position

	@see PxControllerDesc.position setPosition() getPosition() setFootPosition() move()
	*/
	virtual		PxExtendedVec3			getFootPosition()		const	= 0;

	/**
	\brief 이 컨트롤러와 연관된 강체 액터를 가져옵니다 (PhysX 문서를 참조하십시오).
	이 액터를 수동으로 변경할 때의 동작은 정의되지 않으므로, 주로 읽기 전용 속성에 사용하는 것이 좋습니다.

	\return 컨트롤러와 연관된 액터를 반환합니다.
	*/
	virtual		PxRigidDynamic*			getActor()				const	= 0;

	/**
	\brief 스텝 높이.

	\param[in] offset 컨트롤러의 새로운 스텝 오프셋입니다.

	@see PxControllerDesc.stepOffset
	*/
	virtual	    void					setStepOffset(const PxF32 offset) =0;

	/**
	\brief Retrieve the step height.

	\return The step offset for the controller.

	@see setStepOffset()
	*/
	virtual	    PxF32					getStepOffset()						const		=0;

	/**
	\brief CCT의 비보행 모드를 설정합니다.

	\param[in] flag 비보행 모드의 새로운 값입니다.

	@see PxControllerNonWalkableMode
	*/
	virtual		void						setNonWalkableMode(PxControllerNonWalkableMode::Enum flag)	= 0;

	/**
	\brief Retrieves the non-walkable mode for the CCT.

	\return The current non-walkable mode.

	@see PxControllerNonWalkableMode
	*/
	virtual		PxControllerNonWalkableMode::Enum	getNonWalkableMode()				const		= 0;

	/**
	\brief 접촉 오프셋을 가져옵니다.

	\return 컨트롤러의 접촉 오프셋.

	@see PxControllerDesc.contactOffset
	*/
	virtual	    PxF32					getContactOffset()					const		=0;

	/**
	\brief 접촉 오프셋을 설정합니다.

	\param[in] offset 컨트롤러의 접촉 오프셋.

	@see PxControllerDesc.contactOffset
	*/
	virtual	    void					setContactOffset(PxF32 offset)					=0;

	/**
	\brief Retrieve the 'up' direction.

	\return The up direction for the controller.

	@see PxControllerDesc.upDirection
	*/
	virtual		PxVec3					getUpDirection()					const		=0;

	/**
	\brief Sets the 'up' direction.

	\param[in] up The up direction for the controller.

	@see PxControllerDesc.upDirection
	*/
	virtual		void					setUpDirection(const PxVec3& up)				=0;

	/**
	\brief 경사 한계를 가져옵니다.

	\return 컨트롤러의 경사 한계.

	@see PxControllerDesc.slopeLimit
	*/
	virtual	    PxF32					getSlopeLimit()						const		=0;

	/**
	\brief 경사 제한을 설정합니다.

	\note	이 기능은 런타임에서 활성화할 수 없습니다.
			즉, CCT를 생성할 때 경사 제한이 0인 경우(이 기능이 비활성화됨)
			런타임에서 경사 제한을 변경해도 효과가 없으며 호출은 무시됩니다.

	\param[in] slopeLimit 컨트롤러의 경사 제한입니다.

	@see PxControllerDesc.slopeLimit
	*/
	virtual	    void					setSlopeLimit(PxF32 slopeLimit)					=0;

	/**
	\brief 내부 지오메트리 캐시를 플러시합니다.

	캐릭터 컨트롤러는 충돌 테스트를 가속화하기 위해 캐싱을 사용합니다.
	캐시는 장면에서 정적 객체에 변경이 감지될 때 자동으로 플러시됩니다.
	예를 들어, 정적 모양이 장면에 추가,
	업데이트 또는 제거될 때 캐시가 자동으로 무효화됩니다.

	그러나 자동으로 감지할 수 없는 상황이 있을 수 있으며,
	이러한 경우 캐시를 수동으로 무효화해야 합니다.
	현재 사용자는 필터링 동작이 변경될 때
	(즉, PxController::move 호출의 PxControllerFilters 매개변수)
	이 기능을 호출해야 합니다.
	이론적으로 컨트롤러는 이러한 매개변수의 변경을 감지할 수 있지만,
	필터링 함수의 동작 변경은 감지할 수 없습니다.

	@see PxController.move
	*/
	virtual		void					invalidateCache()			= 0;

	/**
	\brief Retrieve the scene associated with the controller.

	\return The physics scene
	*/
	virtual		PxScene*				getScene()						= 0;

	/**
	\brief Returns the user data associated with this controller.

	\return The user pointer associated with the controller.

	@see PxControllerDesc.userData
	*/
	virtual		void*					getUserData()		const		= 0;

	/**
	\brief Sets the user data associated with this controller.

	\param[in] userData The user pointer associated with the controller.

	@see PxControllerDesc.userData
	*/
	virtual		void					setUserData(void* userData)		= 0;

	/**
	\brief 컨트롤러의 내부 상태에 대한 정보를 반환합니다.

	\param[out] state 컨트롤러의 내부 상태

	@see PxControllerState
	*/
	virtual		void					getState(PxControllerState& state)	const		= 0;

	/**
	\brief 컨트롤러의 내부 통계를 반환합니다.

	\param[out] stats 컨트롤러의 내부 통계

	@see PxControllerStats
	*/
	virtual		void					getStats(PxControllerStats& stats)	const		= 0;

	/**
	\brief Resizes the controller.

	This function attempts to resize the controller to a given size, while making sure the bottom
	position of the controller remains constant. In other words the function modifies both the
	height and the (center) position of the controller. This is a helper function that can be used
	to implement a 'crouch' functionality for example.

	\param[in] height Desired controller's height
	*/
	virtual		void					resize(PxReal height)	= 0;

protected:
	PX_INLINE							PxController()					{}
	virtual								~PxController()					{}
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
