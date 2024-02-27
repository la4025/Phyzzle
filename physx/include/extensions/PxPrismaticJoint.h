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

#ifndef PX_PRISMATIC_JOINT_H
#define PX_PRISMATIC_JOINT_H
/** \addtogroup extensions
  @{
*/

#include "extensions/PxJoint.h"
#include "extensions/PxJointLimit.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxPrismaticJoint;

/**
\brief Create a prismatic joint.

 \param[in] physics		The physics SDK
 \param[in] actor0		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame0	The position and orientation of the joint relative to actor0
 \param[in] actor1		An actor to which the joint is attached. NULL may be used to attach the joint to a specific point in the world frame
 \param[in] localFrame1	The position and orientation of the joint relative to actor1 

@see PxPrismaticJoint
*/
PxPrismaticJoint*	PxPrismaticJointCreate(PxPhysics& physics, PxRigidActor* actor0, const PxTransform& localFrame0, PxRigidActor* actor1, const PxTransform& localFrame1);


/**
\brief Flags specific to the prismatic joint.

@see PxPrismaticJoint
*/
struct PxPrismaticJointFlag
{
	enum Enum
	{
		eLIMIT_ENABLED	= 1<<1
	};
};

typedef PxFlags<PxPrismaticJointFlag::Enum, PxU16> PxPrismaticJointFlags;
PX_FLAGS_OPERATORS(PxPrismaticJointFlag::Enum, PxU16)

/**
 \brief A prismatic joint permits relative translational movement between two bodies along
 an axis, but no relative rotational movement.

 the axis on each body is defined as the line containing the origin of the joint frame and
 extending along the x-axis of that frame

 \image html prismJoint.png

 @see PxPrismaticJointCreate() PxJoint
*/
class PxPrismaticJoint : public PxJoint
{
public:
	
	/** 
	\brief returns the displacement of the joint along its axis.
	*/
	virtual PxReal			getPosition()	const	= 0;

	/** 
	\brief returns the velocity of the joint along its axis
	*/
	virtual PxReal			getVelocity()	const	= 0;

	/**
	\brief 조인트 리미트 매개변수를 설정합니다.

	리미트 범위는 [-PX_MAX_F32, PX_MAX_F32]이지만 제한의 폭 (상한 - 하한)도 유효한 부동 소수점이어야 합니다.

	@see PxJointLinearLimitPair getLimit()
	*/
	virtual void			setLimit(const PxJointLinearLimitPair&)		= 0;

	/**
	\brief 조인트 리미트 매개변수를 가져옵니다.

	@see PxJointLinearLimit getLimit()
	*/
	virtual PxJointLinearLimitPair getLimit()	const	= 0;

	/**
	\brief 프리즘 조인트에 특화된 플래그를 설정합니다.

	<b>기본값</b> PxPrismaticJointFlags(0)

	\param[in] flags 조인트 플래그.

	@see PxPrismaticJointFlag setFlag() getFlags()
	*/
	virtual void					setPrismaticJointFlags(PxPrismaticJointFlags flags) = 0;

	/**
	\brief 프리즘 조인트에 특화된 단일 플래그를 true 또는 false로 설정합니다.

	\param[in] flag		설정 또는 해제할 플래그입니다.
	\param[in] value	플래그를 설정할 값입니다.

	@see PxPrismaticJointFlag, getFlags() setFlags()
	*/
	virtual void					setPrismaticJointFlag(PxPrismaticJointFlag::Enum flag, bool value) = 0;

	/**
	\brief Get the flags specific to the Prismatic Joint.

	\return the joint flags

	@see PxPrismaticJoint::flags, PxPrismaticJointFlag setFlag() setFlags()
	*/
	virtual PxPrismaticJointFlags	getPrismaticJointFlags()	const	= 0;

	/**
	\brief Returns string name of PxPrismaticJoint, used for serialization
	*/
	virtual	const char*		getConcreteTypeName() const { return "PxPrismaticJoint"; }

protected:
	//serialization
	
	/**
	\brief Constructor
	*/
	PX_INLINE				PxPrismaticJoint(PxType concreteType, PxBaseFlags baseFlags) : PxJoint(concreteType, baseFlags) {}

	/**
	\brief Deserialization constructor
	*/		
	PX_INLINE				PxPrismaticJoint(PxBaseFlags baseFlags) : PxJoint(baseFlags) {}
	
	/**
	\brief Returns whether a given type name matches with the type of this instance
	*/
	virtual	bool			isKindOf(const char* name) const {	PX_IS_KIND_OF(name, "PxPrismaticJoint", PxJoint); }
	
	//~serialization
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
