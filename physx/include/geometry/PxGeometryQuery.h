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

#ifndef PX_GEOMETRY_QUERY_H
#define PX_GEOMETRY_QUERY_H

/**
\brief Maximum sweep distance for scene sweeps. The distance parameter for sweep functions will be clamped to this value.
The reason for this is GJK support cannot be evaluated near infinity. A viable alternative can be a sweep followed by an infinite raycast.

@see PxScene
*/
#define PX_MAX_SWEEP_DISTANCE 1e8f

/** \addtogroup geomutils
  @{
*/

#include "common/PxPhysXCommonConfig.h"
#include "geometry/PxGeometryHit.h"
#include "geometry/PxGeometryQueryFlags.h"
#include "geometry/PxGeometryQueryContext.h"

#if !PX_DOXYGEN
namespace physx
{
#endif

class PxGeometry;
class PxContactBuffer;

/**
\brief Collection of geometry object queries (sweeps, raycasts, overlaps, ...).
*/
class PxGeometryQuery
{
public:

	/**
	\brief 기하 객체에 대한 레이캐스트 테스트.

	모든 기하 타입이 지원되지만, PxParticleSystemGeometry, PxTetrahedronMeshGeometry 및 PxHairSystemGeometry는 제외됩니다.

	\param[in] origin            테스트할 레이의 시작점
	\param[in] unitDir           테스트할 레이의 정규화된 방향
	\param[in] geom              레이를 테스트할 기하 객체
	\param[in] pose              기하 객체의 포즈
	\param[in] maxDist           레이의 최대 길이, [0, 무한) 범위 내에 있어야 함
	\param[in] hitFlags          충돌 시 가져올 정보의 종류를 지정. #PxHitFlag 플래그의 조합
	\param[in] maxHits           반환되는 최대 충돌 수 = 'rayHits' 버퍼의 크기
	\param[out] rayHits          레이캐스트 충돌 정보
	\param[in] stride            rayHits 배열의 스트라이드 값 (바이트 단위). 일반적으로 패킹된 배열의 경우 sizeof(PxGeomRaycastHit).
	\param[in] queryFlags        쿼리를 제어하는 선택적 플래그
	\param[in] threadContext     선택적 사용자 정의 스레드별 컨텍스트

	\return 레이와 기하 객체 사이의 충돌 수

	@see PxGeomRaycastHit PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static PxU32 raycast(	const PxVec3& origin, const PxVec3& unitDir,
												const PxGeometry& geom, const PxTransform& pose,
												PxReal maxDist, PxHitFlags hitFlags,
												PxU32 maxHits, PxGeomRaycastHit* PX_RESTRICT rayHits, PxU32 stride = sizeof(PxGeomRaycastHit), PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT,
												PxRaycastThreadContext* threadContext = NULL);

	/**
	\brief 두 기하 객체에 대한 겹침 테스트.

	다음 조합은 지원되지 않습니다:
	\li PxPlaneGeometry vs. {PxPlaneGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxTriangleMeshGeometry vs. PxHeightFieldGeometry
	\li PxHeightFieldGeometry vs. PxHeightFieldGeometry
	\li PxParticleSystemGeometry, PxTetrahedronMeshGeometry 또는 PxHairSystemGeometry가 포함된 모든 조합

	\param[in] geom0            첫 번째 기하 객체
	\param[in] pose0            첫 번째 기하 객체의 포즈
	\param[in] geom1            두 번째 기하 객체
	\param[in] pose1            두 번째 기하 객체의 포즈
	\param[in] queryFlags       쿼리를 제어하는 선택적 플래그
	\param[in] threadContext    선택적 사용자 정의 스레드별 컨텍스트

	\return 두 기하 객체가 겹치는 경우 true를 반환합니다.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool overlap(const PxGeometry& geom0, const PxTransform& pose0,
											const PxGeometry& geom1, const PxTransform& pose1,
											PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT, PxOverlapThreadContext* threadContext=NULL);

	/**
	\brief 지정된 지오메트리 객체를 공간에서 스위핑하고 주어진 객체와 충돌을 테스트합니다.

	다음 조합이 지원됩니다.

	\li PxSphereGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxCapsuleGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxBoxGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}
	\li PxConvexMeshGeometry vs. {PxSphereGeometry, PxPlaneGeometry, PxCapsuleGeometry, PxBoxGeometry, PxConvexMeshGeometry, PxTriangleMeshGeometry, PxHeightFieldGeometry}

	\param[in] unitDir 지오메트리 geom0가 스위핑할 정규화된 방향
	\param[in] maxDist 최대 스위핑 거리. [0, inf) 범위여야 합니다.
	\param[in] geom0 스위핑할 지오메트리 객체. 지원되는 지오메트리는 #PxSphereGeometry, #PxCapsuleGeometry, #PxBoxGeometry, #PxConvexMeshGeometry입니다.
	\param[in] pose0 스위핑할 지오메트리 객체의 포즈
	\param[in] geom1 스위핑 대상으로 테스트할 지오메트리 객체
	\param[in] pose1 스위핑 대상 지오메트리 객체의 포즈
	\param[out] sweepHit 스위핑 충돌 정보. 이 메서드가 true를 반환할 때만 유효합니다.
	\param[in] hitFlags 각 충돌에 대해 계산하고 결과 충돌 배열에 작성할 속성을 지정합니다. #PxHitFlag 플래그의 조합
	\param[in] inflation 스위핑된 모양의 표면이 정상 방향으로 추가적으로 확장됩니다. 모서리와 꼭지점을 둥글게 만듭니다.
	\param[in] queryFlags 쿼리를 제어하는 선택적 플래그
	\param[in] threadContext 선택적 사용자 정의 스레드별 컨텍스트

	\return 스위핑된 지오메트리 객체 geom0이 지오메트리 객체 geom1과 충돌하는 경우 true를 반환합니다.

	@see PxGeomSweepHit PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool sweep(	const PxVec3& unitDir, const PxReal maxDist,
											const PxGeometry& geom0, const PxTransform& pose0,
											const PxGeometry& geom1, const PxTransform& pose1,
											PxGeomSweepHit& sweepHit, PxHitFlags hitFlags = PxHitFlag::eDEFAULT,
											const PxReal inflation = 0.0f, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT,
											PxSweepThreadContext* threadContext = NULL);

	/**
	\brief 두 기하 객체 간의 최소 평행 이동 거리(MTD)를 계산합니다.

	다음 기하 객체 조합은 지원되지 않습니다:
	- plane/plane (평면/평면)
	- plane/mesh (평면/메쉬)
	- plane/heightfield (평면/높이 필드)
	- mesh/mesh (메쉬/메쉬)
	- mesh/heightfield (메쉬/높이 필드)
	- heightfield/heightfield (높이 필드/높이 필드)
	- PxParticleSystemGeometry, PxTetrahedronMeshGeometry 또는 PxHairSystemGeometry가 포함된 모든 조합

	이 함수는 단위 벡터('direction')와 관통 깊이('depth')를 반환합니다.

	첫 번째 객체를 두 번째 객체에서 벗어나게 하기 위해, 평행 이동 벡터 D = direction * depth를 첫 번째 객체에 적용해야 합니다.

	반환된 깊이는 항상 양수 또는 0이어야 합니다.

	객체들이 겹치지 않는 경우, 이 함수는 MTD를 계산할 수 없으며 false를 반환합니다.

	\param[out] direction    계산된 MTD 단위 방향
	\param[out] depth        관통 깊이. 항상 양수 또는 0.
	\param[in] geom0         첫 번째 기하 객체
	\param[in] pose0         첫 번째 기하 객체의 포즈
	\param[in] geom1         두 번째 기하 객체
	\param[in] pose1         두 번째 기하 객체의 포즈
	\param[in] queryFlags    쿼리를 제어하는 선택적 플래그.
	\return 객체들이 겹치는 경우, 즉 MTD가 성공적으로 계산된 경우 true를 반환합니다.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static bool	computePenetration(	PxVec3& direction, PxF32& depth,
														const PxGeometry& geom0, const PxTransform& pose0,
														const PxGeometry& geom1, const PxTransform& pose1,
														PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief 점과 기하 객체 간의 거리를 계산합니다.

	현재 지원되는 기하 객체: 상자, 구, 캡슐, 볼록 다각형, 메쉬.

	\note 메쉬의 경우, BVH34 중간 단계 데이터 구조만 지원됩니다.

	\param[in] point            점 P
	\param[in] geom             기하 객체
	\param[in] pose             기하 객체의 포즈
	\param[out] closestPoint    기하 객체에서 점 P에 가장 가까운 점을 선택적으로 반환합니다. 반환된 거리가 엄격히 양수인 경우에만 유효합니다.
	\param[out] closestIndex    선택적으로 가장 가까운 (삼각형) 인덱스를 반환합니다. 삼각형 메쉬에만 유효합니다.
	\param[in] queryFlags       쿼리를 제어하는 선택적 플래그.
	\return 점과 기하 객체 간의 제곱 거리, 점이 객체 내부에 있는 경우 0.0, 오류가 발생한 경우 (지원되지 않는 기하 타입 또는 잘못된 포즈) -1.0을 반환합니다.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static PxReal pointDistance(const PxVec3& point, const PxGeometry& geom, const PxTransform& pose,
													PxVec3* closestPoint=NULL, PxU32* closestIndex=NULL,
													PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief 지오메트리 객체의 경계를 계산합니다.

	\param[out] bounds		계산된 경계를 반환합니다.
	\param[in] geom			지오메트리 객체
	\param[in] pose			지오메트리 객체의 포즈(위치와 회전)
	\param[in] offset		계산된 경계에 추가할 오프셋.
							이 값은 지오메트리의 범위(Extents)에 추가됩니다.
	\param[in] inflation	계산된 경계에 대한 스케일 팩터.
							지오메트리의 범위(Extents)에 이 값을 곱합니다.
	\param[in] queryFlags	선택적인 플래그로 쿼리를 제어합니다.

	@see PxGeometry PxTransform
	*/
	PX_PHYSX_COMMON_API static void	computeGeomBounds(PxBounds3& bounds, const PxGeometry& geom, const PxTransform& pose, float offset=0.0f, float inflation=1.0f, PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT);

	/**
	\brief 볼록 기하 객체와 단일 삼각형 간의 충돌 접촉을 생성합니다.

	\param[in] geom                 기하 객체. 캡슐, 상자 또는 볼록 메쉬일 수 있습니다.
	\param[in] pose                 기하 객체의 포즈
	\param[in] triangleVertices     삼각형의 로컬 공간 내 정점들
	\param[in] triangleIndex        삼각형 인덱스
	\param[in] contactDistance      쌍 간의 접촉이 생성되기 시작하는 거리
	\param[in] meshContactMargin    메쉬 접촉 마진
	\param[in] toleranceLength      toleranceLength. 거리 기반 임계값을 내부적으로 스케일링하여 다양한 단위의 시뮬레이션에 적합한 결과를 생성하는 데 사용됩니다.
	\param[out] contactBuffer       접촉 정보를 기록할 버퍼

	\return 충돌이 있었는지 여부를 반환합니다.
	*/
	PX_PHYSX_COMMON_API static bool generateTriangleContacts(const PxGeometry& geom, const PxTransform& pose, const PxVec3 triangleVertices[3], PxU32 triangleIndex, PxReal contactDistance, PxReal meshContactMargin, PxReal toleranceLength, PxContactBuffer& contactBuffer);

	/**
	\brief Checks if provided geometry is valid.

	\param[in] geom	The geometry object.
	\return True if geometry is valid.

	@see PxGeometry
	*/
	PX_PHYSX_COMMON_API static bool isValid(const PxGeometry& geom);
};

#if !PX_DOXYGEN
}
#endif

/** @} */
#endif
