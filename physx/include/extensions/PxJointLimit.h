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

#ifndef PX_JOINT_LIMIT_H
#define PX_JOINT_LIMIT_H
/** \addtogroup extensions
  @{
*/

#include "foundation/PxMath.h"
#include "common/PxTolerancesScale.h"
#include "extensions/PxJoint.h"
#include "PxPhysXConfig.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Describes the parameters for a joint limit. 

Limits are enabled or disabled by setting flags or other configuration parameters joints, see the
documentation for specific joint types for details.
*/
class PxJointLimitParameters
{
public:
	/**
	\brief 조인트가 한계에 도달했을 때 튕김의 양을 제어합니다.

	복원 값이 1.0인 경우 조인트가 한계에 도달할 때의 속도로 다시 튕깁니다.
	값이 0이면 조인트가 즉시 멈춥니다.

	조인트가 많은 잠긴 DOF(예: 5개)을 가지고 있는 상황에서는 복원이 올바르게 적용되지 않을 수 있습니다.
	이는 솔버가 다른 DOF에 대한 제약을 강제하는 동안 복원 속도가 0이 되기 때문인 제한으로 인한 것입니다.

	이 한계는 각도 및 선형 제한 모두에 적용되지만, 일반적으로 각도 DOF가 제한될 때 가장 뚜렷하게 나타납니다.
	조인트 프로젝션을 비활성화하고 솔버 반복 횟수를 늘리면 이 동작을 어느 정도 개선할 수 있습니다.

	또한, 소프트 조인트 제한과 해당 제한을 향해 구동되는 조인트 드라이브를 결합할 경우 안정성에 영향을 줄 수 있습니다.

	<b>범위:</b> [0,1]<br>
	<b>기본값:</b> 0.0
	*/
	PxReal restitution;

	/**
	조인트가 튕기게 할 최소 충격 속도를 결정합니다.
	*/
	PxReal bounceThreshold;

	/**
	\brief 값이 0보다 크면, 제한이 소프트하며, 즉, 스프링이 조인트를 제한으로 다시 끌어당깁니다.

	<b>범위:</b> [0, PX_MAX_F32)<br>
	<b>기본값:</b> 0.0
	*/
	PxReal stiffness;

	/**
	\brief spring이 0보다 크면, 이것은 제한 스프링의 감쇠입니다.

	<b>범위:</b> [0, PX_MAX_F32)<br>
	<b>기본값:</b> 0.0
	*/
	PxReal damping;

	PxJointLimitParameters() :
		restitution		(0.0f),
		bounceThreshold	(0.0f),
		stiffness		(0.0f),
		damping			(0.0f)
	{
	}
	
	PxJointLimitParameters(const PxJointLimitParameters& p) :
		restitution		(p.restitution),
		bounceThreshold	(p.bounceThreshold),
		stiffness		(p.stiffness),
		damping			(p.damping)
	{
	}	

	/**
	\brief Returns true if the current settings are valid.

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return	PxIsFinite(restitution) && restitution >= 0 && restitution <= 1 && 
			    PxIsFinite(stiffness) && stiffness >= 0 && 
			    PxIsFinite(damping) && damping >= 0 &&
				PxIsFinite(bounceThreshold) && bounceThreshold >= 0;
	}

	PX_INLINE bool isSoft() const
	{
		return damping>0 || stiffness>0;
	}

protected:
	~PxJointLimitParameters() {}
};


/**
\brief Describes a one-sided linear limit.
*/
class PxJointLinearLimit : public PxJointLimitParameters
{
public:
	/**
	\brief the extent of the limit. 

	<b>Range:</b> (0, PX_MAX_F32) <br>
	<b>Default:</b> PX_MAX_F32
	*/
	PxReal value;

	/**
	\brief construct a linear hard limit

	\param[in] extent	The extent of the limit

	@see PxJointLimitParameters
	*/
	PxJointLinearLimit(PxReal extent) : value(extent)
	{
	}

	/**
	\brief construct a linear soft limit 

	\param[in] extent the extent of the limit
	\param[in] spring the stiffness and damping parameters for the limit spring

	@see PxJointLimitParameters
	*/
	PxJointLinearLimit(PxReal extent, const PxSpring& spring) : value(extent)
	{
		stiffness = spring.stiffness;
		damping = spring.damping;
	}

	/**
	\brief Returns true if the limit is valid

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return PxJointLimitParameters::isValid() &&
			   PxIsFinite(value) && 
			   value > 0.0f;
	}
};


/**
\brief Describes a two-sided limit.
*/
class PxJointLinearLimitPair : public PxJointLimitParameters
{
public:
	/**
	\brief 제한의 범위입니다.
	상한값(upper limit)은 하한값(lower limit)보다 낮을 수 없으며,
	두 값이 동일한 경우 제한된 자유도는 잠겨진 것으로 처리됩니다.

	<b>범위:</b> 제한이 사용된 관절에서의 자세한 내용은 참조하십시오.<br>
	<b>기본값:</b> lower = -PX_MAX_F32/3, upper = PX_MAX_F32/3
	*/
	PxReal upper, lower;

	/**
	\brief 선형 하드 제한 쌍을 생성합니다. 하한 거리 값은 상한 거리 값보다 작아야 합니다.

	\param[in]	scale
				PxTolerancesScale 구조체입니다. PxPhysics 객체를 생성할 때 사용한 것과 동일해야 합니다.
				
	\param[in]	lowerLimit
				제한의 하한 거리입니다.
				
	\param[in]	upperLimit
				제한의 상한 거리입니다.

	@see PxJointLimitParameters PxTolerancesScale
	*/
	PxJointLinearLimitPair(const PxTolerancesScale& scale, PxReal lowerLimit = -PX_MAX_F32/3.0f, PxReal upperLimit = PX_MAX_F32/3.0f) :
		upper(upperLimit),
		lower(lowerLimit)
	{
		bounceThreshold = 2.0f * scale.length;
	}

	/**
	\brief 선형 소프트 제한 쌍을 생성합니다.

	\param[in]	lowerLimit
				제한의 하한 거리입니다.
				
	\param[in]	upperLimit
				제한의 상한 거리입니다.
				
	\param[in]	spring
				제한 스프링의 강성과 감쇠 매개변수입니다.

	@see PxJointLimitParameters
	*/
	PxJointLinearLimitPair(PxReal lowerLimit, PxReal upperLimit, const PxSpring& spring) :
		upper(upperLimit),
		lower(lowerLimit)
	{
		stiffness = spring.stiffness;
		damping = spring.damping;
	}

	/**
	\brief Returns true if the limit is valid.

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return PxJointLimitParameters::isValid() &&
			   PxIsFinite(upper) && PxIsFinite(lower) && upper >= lower &&
			   PxIsFinite(upper - lower);
	}
};


class PxJointAngularLimitPair : public PxJointLimitParameters
{
public:
	/**
	\brief 제한의 범위입니다. 상한 값은 하한 값보다 낮아서는 안 됩니다.

	<b>단위:</b> 각도: 라디안
	<b>범위:</b> 해당 제한이 사용되는 조인트에 대한 자세한 내용은 확인하십시오.<br>
	<b>기본값:</b> 하한 = -PI/2, 상한 = PI/2
	*/
	PxReal upper, lower;

	/**
	\brief 각도 하드 제한 쌍을 생성합니다.

	하한 값은 상한 값보다 작아야 합니다.

	\param[in] lowerLimit	제한의 하한 각도
	\param[in] upperLimit	제한의 상한 각도

	@see PxJointLimitParameters
	*/
	PxJointAngularLimitPair(PxReal lowerLimit, PxReal upperLimit) :
		upper(upperLimit),
		lower(lowerLimit)
	{
		bounceThreshold = 0.5f;
	}

	/**
	\brief 각도 소프트 제한 쌍을 생성합니다.

	하한 값은 상한 값보다 작아야 합니다.

	\param[in] lowerLimit	제한의 하한 각도
	\param[in] upperLimit	제한의 상한 각도
	\param[in] spring		제한 스프링의 강성과 감쇠

	@see PxJointLimitParameters
	*/
	PxJointAngularLimitPair(PxReal lowerLimit, PxReal upperLimit, const PxSpring& spring) :
		upper(upperLimit),
		lower(lowerLimit)
	{
		stiffness = spring.stiffness;
		damping = spring.damping;
	}

	/**
	\brief Returns true if the limit is valid.

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return PxJointLimitParameters::isValid() &&
			   PxIsFinite(upper) && PxIsFinite(lower) && upper >= lower;
	}
};

/**
\brief Describes an elliptical conical joint limit. Note that very small or highly elliptical limit cones may 
result in jitter.

@see PxD6Joint PxSphericalJoint
*/
class PxJointLimitCone : public PxJointLimitParameters
{
public:
	/**
	\brief 제약 프레임의 Y 축으로부터의 최대 각도입니다.

	<b>단위:</b> 각도: 라디안
	<b>범위:</b> 각도: (0, PI)<br>
	<b>기본값:</b> PI/2
	*/
	PxReal yAngle;

	/**
	\brief 제약 프레임의 Z 축으로부터의 최대 각도입니다.

	<b>단위:</b> 각도: 라디안
	<b>범위:</b> 각도: (0, PI)<br>
	<b>기본값:</b> PI/2
	*/
	PxReal zAngle;

	/**
	\brief 콘 형식의 제한을 생성합니다.

	\param[in] yLimitAngle	제약 프레임으로부터 Y 축의 제한 각도
	\param[in] zLimitAngle	제약 프레임으로부터 Z 축의 제한 각도

	@see PxJointLimitParameters
	*/
	PxJointLimitCone(PxReal yLimitAngle, PxReal zLimitAngle) :
		yAngle(yLimitAngle),
		zAngle(zLimitAngle)
	{
		bounceThreshold = 0.5f;
	}

	/**
	\brief 콘 형식의 소프트 제한을 생성합니다.

	\param[in] yLimitAngle	제약 프레임으로부터 Y 축의 제한 각도
	\param[in] zLimitAngle	제약 프레임으로부터 Z 축의 제한 각도
	\param[in] spring		제한 스프링의 강성과 감쇠

	@see PxJoint
	*/
	PxJointLimitCone(PxReal yLimitAngle, PxReal zLimitAngle, const PxSpring& spring) :
		yAngle(yLimitAngle),
		zAngle(zLimitAngle)
	{
		stiffness = spring.stiffness;
		damping = spring.damping;
	}

	/**
	\brief Returns true if the limit is valid.

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return PxJointLimitParameters::isValid() &&
			   PxIsFinite(yAngle) && yAngle>0 && yAngle<PxPi && 
			   PxIsFinite(zAngle) && zAngle>0 && zAngle<PxPi;
	}
};

/**
\brief Describes a pyramidal joint limit.

@see PxD6Joint
*/
class PxJointLimitPyramid : public PxJointLimitParameters
{
public:
	/**
	\brief the minimum angle from the Y axis of the constraint frame.

	<b>Unit:</b> Angular: Radians
	<b>Range:</b> Angular: (-PI,PI)<br>
	<b>Default:</b> -PI/2
	*/
	PxReal yAngleMin;

	/**
	\brief the maximum angle from the Y axis of the constraint frame.

	<b>Unit:</b> Angular: Radians
	<b>Range:</b> Angular: (-PI,PI)<br>
	<b>Default:</b> PI/2
	*/
	PxReal yAngleMax;

	/**
	\brief the minimum angle from the Z-axis of the constraint frame.

	<b>Unit:</b> Angular: Radians
	<b>Range:</b> Angular: (-PI,PI)<br>
	<b>Default:</b> -PI/2
	*/
	PxReal zAngleMin;

	/**
	\brief the maximum angle from the Z-axis of the constraint frame.

	<b>Unit:</b> Angular: Radians
	<b>Range:</b> Angular: (-PI,PI)<br>
	<b>Default:</b> PI/2
	*/
	PxReal zAngleMax;

	/**
	\brief Construct a pyramid hard limit. 

	\param[in] yLimitAngleMin	The minimum limit angle from the Y-axis of the constraint frame
	\param[in] yLimitAngleMax	The maximum limit angle from the Y-axis of the constraint frame
	\param[in] zLimitAngleMin	The minimum limit angle from the Z-axis of the constraint frame
	\param[in] zLimitAngleMax	The maximum limit angle from the Z-axis of the constraint frame

	@see PxJointLimitParameters
	*/
	PxJointLimitPyramid(PxReal yLimitAngleMin, PxReal yLimitAngleMax, PxReal zLimitAngleMin, PxReal zLimitAngleMax) :
		yAngleMin(yLimitAngleMin),
		yAngleMax(yLimitAngleMax),
		zAngleMin(zLimitAngleMin),
		zAngleMax(zLimitAngleMax)
	{
		bounceThreshold = 0.5f;
	}

	/**
	\brief Construct a pyramid soft limit. 

	\param[in] yLimitAngleMin	The minimum limit angle from the Y-axis of the constraint frame
	\param[in] yLimitAngleMax	The maximum limit angle from the Y-axis of the constraint frame
	\param[in] zLimitAngleMin	The minimum limit angle from the Z-axis of the constraint frame
	\param[in] zLimitAngleMax	The maximum limit angle from the Z-axis of the constraint frame
	\param[in] spring			The stiffness and damping of the limit spring

	@see PxJointLimitParameters
	*/
	PxJointLimitPyramid(PxReal yLimitAngleMin, PxReal yLimitAngleMax, PxReal zLimitAngleMin, PxReal zLimitAngleMax, const PxSpring& spring) :
		yAngleMin(yLimitAngleMin),
		yAngleMax(yLimitAngleMax),
		zAngleMin(zLimitAngleMin),
		zAngleMax(zLimitAngleMax)
	{
		stiffness = spring.stiffness;
		damping = spring.damping;
	}

	/**
	\brief Returns true if the limit is valid.

	\return true if the current settings are valid
	*/
	PX_INLINE bool isValid() const
	{
		return PxJointLimitParameters::isValid() &&
				PxIsFinite(yAngleMin) && yAngleMin>-PxPi && yAngleMin<PxPi && 
				PxIsFinite(yAngleMax) && yAngleMax>-PxPi && yAngleMax<PxPi && 
				PxIsFinite(zAngleMin) && zAngleMin>-PxPi && zAngleMin<PxPi && 
				PxIsFinite(zAngleMax) && zAngleMax>-PxPi && zAngleMax<PxPi && 
				yAngleMax>=yAngleMin && zAngleMax>=zAngleMin;
	}
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
