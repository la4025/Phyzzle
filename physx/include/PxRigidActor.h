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

#ifndef PX_RIGID_ACTOR_H
#define PX_RIGID_ACTOR_H
/** \addtogroup physics
@{
*/

#include "PxActor.h"
#include "PxShape.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxConstraint;

/**
\brief PxRigidActor represents a base class shared between dynamic and static rigid bodies in the physics SDK.

PxRigidActor objects specify the geometry of the object by defining a set of attached shapes (see #PxShape).

@see PxActor
*/
class PxRigidActor : public PxActor
{
public:
	/**
	*\brief 강체 액터 객체를 삭제합니다.

	또한 액터와 관련된 모든 형상을 해제합니다.

	액터를 해제하면 액터에 연결된 모든 객체(조인트 등과 같은 제약 쉐이더)에 영향을 미칩니다.
	이러한 연결된 객체는 씬 삭제 시 또는 사용자가 명시적으로 이러한 객체들에 대해 release()를 호출하여 삭제됩니다.
	액터 자체를 제거하기 전에 항상 액터를 참조하는 모든 객체를 제거하는 것이 좋습니다.
	죽은 연결된 객체의 목록을 검색하는 것은 불가능합니다.

	<b>슬리핑:</b> 이 호출은 삭제된 액터와 직접 또는 간접적으로 연락하는 모든 슬리핑 액터를 깨웁니다.

	#PxActor::release()를 호출하므로 해당 메서드의 설명도 확인하는 것이 좋습니다.

	@see PxActor::release()
	*/
	virtual		void			release() = 0;

	/**
	\brief Returns the internal actor index.

	\warning	This is only defined for actors that have been added to a scene.

	\return		The internal actor index, or 0xffffffff if the actor is not part of a scene.
	*/
	virtual PxU32				getInternalActorIndex() const = 0;

/************************************************************************************************/
/** @name Global Pose Manipulation
*/

	/**
	\brief Retrieves the actors world space transform.

	The getGlobalPose() method retrieves the actor's current actor space to world space transformation.

	\note It is not allowed to use this method while the simulation is running (except during PxScene::collide(),
	in PxContactModifyCallback or in contact report callbacks).

	\return Global pose of object.

	@see PxRigidDynamic.setGlobalPose() PxRigidStatic.setGlobalPose()
	*/
	virtual		PxTransform 	getGlobalPose()		const = 0;

	/**
	\brief 물체의 자세를 월드 공간에 설정하는 메서드입니다.

	이 메서드는 물체 공간에서 월드 공간으로의 변환을 즉시 변경합니다.

	이 메서드는 주로 동적 강체(참조 #PxRigidDynamic)를 위해 사용됩니다. 정적 물체에 이 메서드를 호출하면
	내부 최적화 구조를 재계산해야 하므로 성능 저하가 발생할 수 있습니다.
	또한, 정적 물체를 이동시키면 동적 물체나 조인트와 올바르게 상호작용하지 않을 수 있습니다.

	물체의 위치를 직접 제어하고 동적 물체 및 조인트와 올바르게 상호작용하도록 하려면,
	PxRigidBodyFlag::eKINEMATIC 플래그가 설정된 동적 물체를 생성한 후 setKinematicTarget() 명령을 사용하여 경로를 정의하십시오.

	동적 물체를 이동할 때도 이 메서드를 사용하는 것을 자제하십시오. 가능한 한 다음을 피하십시오:

	\li 다른 물체와 겹치도록 물체를 이동시키는 것 (유효하지 않은 물리 상태)

	\li 조인트로 연결된 물체를 다른 물체로부터 멀리 이동시키는 것 (조인트 오류 발생)

	\note 물체가 아직 장면에 추가되지 않은 #PxPruningStructure의 일부인 경우 이 메서드를 사용하는 것은 허용되지 않습니다.

	<b>수면 상태:</b> 이 호출은 동적 물체가 수면 상태인 경우 autowake 매개변수가 true(기본값)일 때 깨어나게 합니다.

	\param[in] pose		물체의 로컬 프레임에서 글로벌 프레임으로의 변환입니다. <b>범위:</b> 강체 변환.
	\param[in] autowake 물체가 동적일 경우 깨어나게 할지 여부를 설정합니다.
						이 매개변수는 정적 또는 키네마틱 물체에는 영향을 주지 않습니다.
						true이고 현재 깨우기 카운터 값이 #PxSceneDesc::wakeCounterResetValue보다
						작으면 해당 값을 재설정 값으로 증가시킵니다.

	@see getGlobalPose()
	*/
	virtual		void			setGlobalPose(const PxTransform& pose, bool autowake = true) = 0;

/************************************************************************************************/
/** @name Shapes
*/

	/**
	\brief 모양을 액터에 부착합니다.

	이 호출은 모양의 참조 횟수를 증가시킵니다.

	\note 동적 강체 액터의 질량 속성은 자동으로 재계산되지 않아 새로운 모양이 암시하는 새 질량 분포를 반영하지 않습니다. 이 호출 이후에는 PhysX 확장 메서드 #PxRigidBodyExt::updateMassAndInertia()를 호출하여 이를 수행하세요.

	eSIMULATION_SHAPE로 구성된 삼각형 메시, 높낮이 필드 또는 평면 지오메트리 모양은 비-키네마틱 PxRigidDynamic 인스턴스에 대해 지원되지 않습니다.

	<b>휴면 상태:</b> 액터를 자동으로 깨우지 <b>않습니다.</b>

	\param[in] shape 부착할 모양입니다.

	\return 성공하면 true.
	*/
	virtual bool				attachShape(PxShape& shape) = 0;


	/**
	\brief 액터에서 모양을 분리합니다.

	이는 또한 PxShape의 참조 횟수를 감소시키며, 참조 횟수가 0이되면 해당 모양이 삭제됩니다.

	<b>휴면 상태:</b> 액터를 자동으로 깨우지 <b>않습니다.</b>

	\param[in] shape 분리할 모양입니다.
	\param[in] wakeOnLostTouch 이전 프레임에서 접촉했던 객체가 다음 프레임에서 깨어날지 여부를 지정합니다.
	PxArticulationReducedCoordinate 및 PxRigidActor 유형에만 적용됩니다.
	*/
	virtual void				detachShape(PxShape& shape, bool wakeOnLostTouch = true) = 0;

	/**
	\brief Returns the number of shapes assigned to the actor.

	You can use #getShapes() to retrieve the shape pointers.

	\return Number of shapes associated with this actor.

	@see PxShape getShapes()
	*/
	virtual		PxU32			getNbShapes()		const	= 0;

	/**
	\brief 해당 액터에 속한 모든 모양 포인터를 검색합니다.

	이러한 모양은 액터가 충돌 감지에 사용하는 모양입니다.

	모양 포인터의 수는 #getNbShapes()를 호출하여 검색할 수 있습니다.

	참고: #PxShape::release()를 사용하여 모양을 제거하면 제거된 모양의 포인터가 무효화됩니다.

	\param[out]	userBuffer	모양 포인터를 저장할 버퍼입니다.
	\param[in]	bufferSize	제공된 사용자 버퍼의 크기입니다.
	\param[in]	startIndex	검색할 첫 번째 모양 포인터의 인덱스입니다.
	\return 버퍼에 쓰여진 모양 포인터의 수입니다.

	@see PxShape getNbShapes() PxShape::release()
	*/
	virtual		PxU32			getShapes(PxShape** userBuffer, PxU32 bufferSize, PxU32 startIndex=0)			const	= 0;

/************************************************************************************************/
/** @name Constraints
*/

	/**
	\brief Returns the number of constraint shaders attached to the actor.

	You can use #getConstraints() to retrieve the constraint shader pointers.

	\return Number of constraint shaders attached to this actor.

	@see PxConstraint getConstraints()
	*/
	virtual		PxU32			getNbConstraints()		const	= 0;

	/**
	\brief Retrieve all the constraint shader pointers belonging to the actor.

	You can retrieve the number of constraint shader pointers by calling #getNbConstraints()

	Note: Removing constraint shaders with #PxConstraint::release() will invalidate the pointer of the released constraint.

	\param[out] userBuffer The buffer to store the constraint shader pointers.
	\param[in] bufferSize Size of provided user buffer.
	\param[in] startIndex Index of first constraint pointer to be retrieved
	\return Number of constraint shader pointers written to the buffer.

	@see PxConstraint getNbConstraints() PxConstraint::release()
	*/
	virtual		PxU32			getConstraints(PxConstraint** userBuffer, PxU32 bufferSize, PxU32 startIndex=0)		const	= 0;

protected:
	PX_INLINE					PxRigidActor(PxType concreteType, PxBaseFlags baseFlags) : PxActor(concreteType, baseFlags) {}
	PX_INLINE					PxRigidActor(PxBaseFlags baseFlags) : PxActor(baseFlags) {}
	virtual						~PxRigidActor()	{}
	virtual		bool			isKindOf(const char* name)	const	{	PX_IS_KIND_OF(name, "PxRigidActor", PxActor); }
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
