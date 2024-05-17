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

#ifndef PX_QUERY_FILTERING_H
#define PX_QUERY_FILTERING_H
/** \addtogroup scenequery
@{
*/

#include "PxPhysXConfig.h"
#include "PxFiltering.h"
#include "PxQueryReport.h"
#include "PxClient.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxShape;
class PxRigidActor;
struct PxQueryHit;

/**
\brief Filtering flags for scene queries.

@see PxQueryFilterData.flags
*/
struct PxQueryFlag
{
	enum Enum
	{
		eSTATIC							= (1<<0),	//!< 정적 형태를 탐색합니다.

		eDYNAMIC						= (1<<1),	//!< 동적 형태를 탐색합니다.

		ePREFILTER						= (1<<2),	//!< 사전 교차 검사 필터(pre-intersection-test filter)를 실행합니다.
													//!< (PxQueryFilterCallback::preFilter() 참조)

		ePOSTFILTER						= (1<<3),	//!< 사후 교차 검사 필터(post-intersection-test filter)를 실행합니다.
													//!< (PxQueryFilterCallback::postFilter() 참조)

		eANY_HIT						= (1<<4),	//!< 발견된 즉시 모든 탐색를 중단하고 콜백을 통해 해당 히트를 반환합니다.
													//!< 쿼리 성능을 향상시킵니다. 이 플래그는 eTOUCH와 eBLOCK hitTypes가 모두 해당 플래그로 인해 히트로 간주됩니다.

		eNO_BLOCK						= (1<<5),	//!< 모든 히트를 접촉(touching)으로 보고합니다. 사용자 필터에서 eBLOCK을 eTOUCH로 변경하여 eBLOCK이 반환되는 것을 재정의합니다.
													//!< 이것은 쿼리 성능을 향상시킬 수 있는 최적화 힌트입니다.
		
		eBATCH_QUERY_LEGACY_BEHAVIOUR	= (1<<6),	//!< 레거시 배치 쿼리 필터 동작을 사용합니다.
													//!< 이 플래그를 올리면 하드코딩된 필터 방정식이 무시됩니다.
													//!< 이로써 ePREFILTER와 ePOSTFILTER 플래그에 따라 제공된 PxQueryFilterCallback가 사용됨이 보장됩니다.

		eDISABLE_HARDCODED_FILTER		= (1<<6),	//!< eBATCH_QUERY_LEGACY_BEHAVIOUR와 동일하지만, 보다 명확한 이름입니다.
													//!< 일반/비배치 쿼리에서도 사용할 수 있음을 명시적으로 나타냅니다.

		eRESERVED						= (1<<15)	//!< 내부 사용을 위해 예약된 플래그입니다.
	};
};
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eSTATIC==(1<<0));
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eDYNAMIC==(1<<1));
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eBATCH_QUERY_LEGACY_BEHAVIOUR==PxQueryFlag::eDISABLE_HARDCODED_FILTER);

/**
\brief Flags typedef for the set of bits defined in PxQueryFlag.

*/
typedef PxFlags<PxQueryFlag::Enum,PxU16> PxQueryFlags;
PX_FLAGS_OPERATORS(PxQueryFlag::Enum,PxU16)

/**
\brief 씬 쿼리 히트(교차)의 분류.

	- eNONE: 이 히트 유형을 반환하면 해당 히트를 보고하지 않아야 함을 의미합니다.
	- eBLOCK: 모든 레이캐스트, 스윕 및 오버랩 쿼리에 대해 가장 가까운 eBLOCK 유형 히트가 항상 PxHitCallback::block 멤버에 반환됩니다.
	- eTOUCH: 레이캐스트, 스윕 또는 오버랩 쿼리가 PxHitCallback::nbTouches 및 PxHitCallback::touches 매개변수로 호출된 경우, 전역적으로 가장 가까운 eBLOCK 유형 히트와 거리가 같거나 더 가까운(eTOUCH hits that are closer or same distance) eTOUCH 유형 히트가 보고됩니다. (touchDistance <= blockDistance 조건)
	- 예를 들어, 레이캐스트 쿼리의 모든 히트를 기록하려면 항상 eTOUCH를 반환하세요.

오버랩() 쿼리의 모든 히트는 교차 거리가 0으로 처리됩니다.
이는 히트가 정렬되지 않으며 모든 eTOUCH 히트가 콜백에 기록되며 eBLOCK 오버랩 히트가 만날지라도 모든 eTOUCH 히트가 기록됩니다.
모든 오버랩() 차단 히트는 길이가 0이지만, 하나 (임의의)의 eBLOCK 오버랩 히트만 PxHitCallback::block에 기록됩니다.
모든 오버랩() eTOUCH 유형 히트가 보고됩니다. (zero touchDistance <= zero blockDistance condition)

PxHitCallback::nbTouches 멤버 또는 제로 터치 버퍼로 레이캐스트/스윕/오버랩 호출의 경우,
eBLOCK 유형의 가장 가까운 히트만 반환됩니다. 모든 eTOUCH 히트는 삭제됩니다.

@see PxQueryFilterCallback.preFilter PxQueryFilterCallback.postFilter PxScene.raycast PxScene.sweep PxScene.overlap
*/
struct PxQueryHitType
{
	enum Enum
	{
		eNONE	= 0,	//!< 쿼리는 이 형태를 무시해야 합니다.
		eTOUCH	= 1,	//!< 형태에 대한 히트는 쿼리의 교차 지오메트리를 접촉하지만 차단하지 않습니다.
		eBLOCK	= 2		//!< 형태에 대한 히트는 쿼리를 차단합니다. (겹침 쿼리를 차단하지는 않음)
	};
};

/**
\brief 씬 쿼리 필터링 데이터.

씬 쿼리가 형태와 교차되는 경우, 필터링은 다음과 같은 순서로 수행됩니다:

\li 배치 쿼리가 아닌 경우만: <br>만약 데이터 필드가 0이 아니고,
	데이터와 형태의 queryFilterData의 비트 AND 값이 0이면, 해당 형태는 건너뜁니다.

\li 플래그 필드에서 필터 콜백이 활성화된 경우 (PxQueryFlags 참조), 해당되는 콜백이 호출됩니다.

\li #PxQueryFlag::ePREFILTER 또는 #PxQueryFlag::ePOSTFILTER 중 하나도 설정되어 있지 않으면,
	PxHitCallback::nbTouches의 값이 0이고 PxHitCallback::nbTouches로 제공된 쿼리가 있으면,
	히트는 기본적으로 #PxQueryHitType::eBLOCK 유형으로 설정됩니다.
	그렇지 않으면 #PxQueryHitType::eTOUCH 유형으로 설정됩니다.

@see PxScene.raycast PxScene.sweep PxScene.overlap PxQueryFlag::eANY_HIT
*/
struct PxQueryFilterData
{
	/** \brief default constructor */
	explicit PX_INLINE PxQueryFilterData() : flags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC)		{}

	/** \brief constructor to set both filter data and filter flags */
	explicit PX_INLINE PxQueryFilterData(const PxFilterData& fd, PxQueryFlags f) : data(fd), flags(f)	{}

	/** \brief constructor to set filter flags only */
	explicit PX_INLINE PxQueryFilterData(PxQueryFlags f) : flags(f)										{}

	PxFilterData	data;		//!< Filter data associated with the scene query
	PxQueryFlags	flags;		//!< Filter flags (see #PxQueryFlags)
};

/**
\brief 씬 쿼리 필터링 콜백입니다.

씬 쿼리 교차 후보에 대한 사용자 정의 필터링 로직입니다. 교차 후보 객체가 데이터 기반 필터(#PxQueryFilterData 참조)를 통과하면,
필터링 콜백이 요청된 경우(#PxQueryFilterData.flags 참조),

\li #PxQueryFlag::ePREFILTER가 설정된 경우,
	preFilter 함수가 정확한 교차 테스트 이전에 실행됩니다.
	이 함수가 #PxQueryHitType::eTOUCH 또는 #PxQueryHitType::eBLOCK을 반환하면
	정확한 테스트가 수행되어 교차 위치가 결정됩니다.

preFilter 함수는 인수로 전달받은 queryFlags의 사본을 덮어쓸 수 있어서,
#PxHitFlag::eMODIFIABLE_FLAGS 중 일부를 개별 모양 기준으로 지정할 수 있습니다.
변경 사항은 필터링된 모양에만 적용되며, 다른 플래그에 대한 변경은 무시됩니다.

\li #PxQueryFlag::ePREFILTER가 설정되지 않은 경우,
	원래 쿼리의 filterData.flags를 사용하여 정확한 교차 테스트가 수행됩니다.

\li #PxQueryFlag::ePOSTFILTER가 설정된 경우,
	각 교차에 대해 postFilter 함수가 호출되어 터치/블록 상태를 결정합니다.
	이는 이 모양에 대해 이전에 preFilter 함수에서 반환된 터치/블록 상태를 재정의합니다.

필터링 호출은 광선이나 스위핑 방향을 따라 정렬되지 않습니다.

@see PxScene.raycast PxScene.sweep PxScene.overlap PxQueryFlags PxHitFlags
*/
class PxQueryFilterCallback
{
public:

	/**
	\brief This filter callback is executed before the exact intersection test if PxQueryFlag::ePREFILTER flag was set.

	\param[in] filterData		custom filter data specified as the query's filterData.data parameter.
	\param[in] shape			A shape that has not yet passed the exact intersection test.
	\param[in] actor			The shape's actor.
	\param[in,out] queryFlags	scene query flags from the query's function call (only flags from PxHitFlag::eMODIFIABLE_FLAGS bitmask can be modified)
	\return the updated type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) = 0;

	/**
	\brief This filter callback is executed if the exact intersection test returned true and PxQueryFlag::ePOSTFILTER flag was set.

	\param[in] filterData	custom filter data of the query
	\param[in] hit			Scene query hit information. faceIndex member is not valid for overlap queries. For sweep and raycast queries the hit information can be cast to #PxSweepHit and #PxRaycastHit respectively.
	\param[in] shape		Hit shape
	\param[in] actor		Hit actor
	\return the updated hit type for this hit  (see #PxQueryHitType)
	*/
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor) = 0;

	/**
	\brief virtual destructor
	*/
	virtual ~PxQueryFilterCallback() {}
};


#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
