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

#ifndef PX_CONVEX_MESH_DESC_H
#define PX_CONVEX_MESH_DESC_H
/** \addtogroup cooking
@{
*/

#include "foundation/PxVec3.h"
#include "foundation/PxFlags.h"
#include "common/PxCoreUtilityTypes.h"
#include "geometry/PxConvexMesh.h"
#include "PxSDFDesc.h"

#if !PX_DOXYGEN
namespace physx
{
#endif
/**
\brief Flags which describe the format and behavior of a convex mesh.
*/
struct PxConvexFlag
{
	enum Enum
	{
		/**
		PxConvexMeshDesc::triangles 또는 PxConvexMeshDesc::polygons에서 16비트 정점 인덱스의 사용을 나타냅니다.
		(그렇지 않으면 32비트 인덱스가 가정됩니다)
		@see #PxConvexMeshDesc.indices
		*/
		e16_BIT_INDICES		=	(1<<0),

		/**
		정점에서 오목 다각형을 자동으로 다시 계산합니다. 이 플래그가 설정되지 않은 경우 전체 기하 구조를 수동으로 제공해야 합니다.

		\note 오목 볼록 다각형 계산에는 두 가지 다른 알고리즘이 있습니다. PxConvexMeshCookingType를 참조하십시오.

		@see PxConvexMeshCookingType
		*/
		eCOMPUTE_CONVEX		=	(1<<1),	

		/**
		오목 볼록 다각형 계산 중에 거의 제로 면적 삼각형을 확인하고 제거합니다.
		거부된 면적 크기는 PxCookingParams::areaTestEpsilon에서 지정됩니다.

		\note 이 플래그는 eCOMPUTE_CONVEX와 함께만 사용됩니다.

		@see PxCookingParams PxCookingParams::areaTestEpsilon
		*/
		eCHECK_ZERO_AREA_TRIANGLES		=	(1<<2),

		/**
		입력 정점을 k-평균 클러스터링을 사용하여 양자화합니다.

		\note 입력 정점은 PxConvexMeshDesc::quantizedCount로 양자화됩니다.
		http://en.wikipedia.org/wiki/K-means_clustering을 참조하십시오.

		*/
		eQUANTIZE_INPUT = (1 << 3),

		/**
		볼록 메쉬 유효성 검사를 비활성화하여 헐 생성 속도를 높입니다. 
		디버그 빌드에서 별도의 유효성 검사 함수를 사용하십시오.

		이전에 유효성 검사를 수행하지 않고 유효하지 않은 
		입력 데이터로 볼록 메쉬를 생성하면 정의되지 않은 동작이 발생할 수 있습니다.

		@see PxCooking::validateConvexMesh
		*/
		eDISABLE_MESH_VALIDATION = (1 << 4),

		/**
		정점 제한 알고리즘에 플레인 시프팅을 활성화합니다.

		플레인 시프팅은 계산된 헐이 지정된 정점 제한보다 많은 정점을 가지고 있는 경우에 대한 대안 알고리즘입니다.

		기본 알고리즘은 전체 헐과 입력 정점 주위의 OBB를 계산합니다. 
		이 OBB는 그 후 헐 플레인과 슬라이스됩니다
		정점 제한이 도달될 때까지. 
		기본 알고리즘은 정점 제한을 적어도 8로 설정해야 하며 일반적으로 플레인 시프팅보다 훨씬
		품질이 좋은 결과를 생성합니다.

		플레인 시프팅이 활성화되면 헐 계산은 정점 제한에 도달할 때 중지됩니다. 
		그런 다음 헐 플레인은 모든 입력 정점을 포함하도록 이동되며
		새로운 평면 교차점은 지정된 정점 제한으로 최종 헐을 생성하기 위해 사용됩니다. 
		플레인 시프팅은 입력 클라우드에서 매우 먼 정점에 대해
		날카로운 모서리를 생성할 수 있으며 결과적인 헐 내에 모든 입력 정점이 포함되지 않을 수 있습니다. 
		그러나 정점 제한을 4로 설정할 수 있습니다.
		*/
		ePLANE_SHIFTING = (1 << 5),

		/**
		SIMD 코드를 사용하여 관성 텐서를 계산합니다. 
		그러나 정밀도가 낮아 매우 얇은 헐의 경우 잘못된 관성을 초래할 수 있습니다.
		*/
		eFAST_INERTIA_COMPUTATION = (1 << 6),

		/**
		GPU 시뮬레이션 제한을 고려하여 볼록 헐을 생성합니다.
		정점 제한 및 다각형 제한은 64로 설정되고
		각 면의 정점 제한은 내부적으로 32로 설정됩니다.
		\note eCOMPUTE_CONVEX 플래그와 함께만 사용할 수 있습니다.

		@deprecated PhysX 5.2 이후. #PxCookingParams::buildGPUData를 true로 설정하면 항상 GPU 호환 메쉬를 쿡합니다.
		*/
		eGPU_COMPATIBLE PX_DEPRECATED = (1 << 7),

		/**
		입력 정점을 더 나은 계산 안정성을 제공하기 위해 원점 주변으로 이동합니다.
		원점 주변의 입력 정점을 제공하는 것이 좋습니다.
		그렇지 않으면 이 플래그를 사용하여
		수치적 안정성을 향상시킵니다.
		\note eCOMPUTE_CONVEX 플래그와 함께만 사용됩니다.
		*/
		eSHIFT_VERTICES = (1 << 8)
	};
};

/**
\brief collection of set bits defined in PxConvexFlag.

@see PxConvexFlag
*/
typedef PxFlags<PxConvexFlag::Enum,PxU16> PxConvexFlags;
PX_FLAGS_OPERATORS(PxConvexFlag::Enum,PxU16)

/**
\brief Descriptor class for #PxConvexMesh.
\note The number of vertices and the number of convex polygons in a cooked convex mesh is limited to 256.
\note The number of vertices and the number of convex polygons in a GPU compatible convex mesh is limited to 64,
and the number of faces per vertex is limited to 32.

@see PxConvexMesh PxConvexMeshGeometry PxShape PxPhysics.createConvexMesh()

*/
class PxConvexMeshDesc
{
public:

	/**
	\brief Vertex positions data in PxBoundedData format.

	<b>Default:</b> NULL
	*/
	PxBoundedData points;

	/**
	\brief Polygons data in PxBoundedData format.
	<p>Pointer to first polygon. </p>

	<b>Default:</b> NULL	

	@see PxHullPolygon
	*/
	PxBoundedData polygons;

	/**
	\brief Polygon indices data in PxBoundedData format.
	<p>Pointer to first index.</p>

	<b>Default:</b> NULL	

	<p>This is declared as a void pointer because it is actually either an PxU16 or a PxU32 pointer.</p>

	@see PxHullPolygon PxConvexFlag::e16_BIT_INDICES
	*/
	PxBoundedData indices;

	/**
	\brief Flags bits, combined from values of the enum ::PxConvexFlag

	<b>Default:</b> 0
	*/
	PxConvexFlags flags;

	/**
	\brief Limits the number of vertices of the result convex mesh. Hard maximum limit is 255
	and minimum limit is 4 if PxConvexFlag::ePLANE_SHIFTING is used, otherwise the minimum
	limit is 8.

	\note The please see PxConvexFlag::ePLANE_SHIFTING for algorithm explanation
	\note The maximum limit for GPU compatible convex meshes is 64.

	@see PxConvexFlag::ePLANE_SHIFTING

	<b>Range:</b> [4, 255]<br>
	<b>Default:</b> 255
	*/
	PxU16 vertexLimit;

	/**
	\brief Limits the number of polygons of the result convex mesh. Hard maximum limit is 255
	and minimum limit is 4.

	\note The maximum limit for GPU compatible convex meshes is 64.

	<b>Range:</b> [4, 255]<br>
	<b>Default:</b> 255
	 */
	PxU16 polygonLimit;

	/**
	\brief Maximum number of vertices after quantization. The quantization is done during the vertex cleaning phase. 
	The quantization is applied when PxConvexFlag::eQUANTIZE_INPUT is specified.

	@see PxConvexFlag::eQUANTIZE_INPUT

	<b>Range:</b> [4, 65535]<br>
	<b>Default:</b> 255
	*/
	PxU16 quantizedCount;

	/**
	\brief SDF descriptor. When this descriptor is set, signed distance field is calculated for this convex mesh.

	<b>Default:</b> NULL
	*/
	PxSDFDesc* sdfDesc;

	/**
	\brief constructor sets to default.
	*/
	PX_INLINE PxConvexMeshDesc();
	/**
	\brief (re)sets the structure to the default.	
	*/
	PX_INLINE void setToDefault();

	
	/**
	\brief Returns true if the descriptor is valid.

	\return True if the current settings are valid
	*/
	PX_INLINE bool isValid() const;
};

PX_INLINE PxConvexMeshDesc::PxConvexMeshDesc()	//constructor sets to default
: vertexLimit(255), polygonLimit(255), quantizedCount(255), sdfDesc(NULL)
{

}

PX_INLINE void PxConvexMeshDesc::setToDefault()
{
	*this = PxConvexMeshDesc();
}

PX_INLINE bool PxConvexMeshDesc::isValid() const
{
	// Check geometry
	if(points.count < 3 ||	//at least 1 trig's worth of points
		(points.count > 0xffff && flags & PxConvexFlag::e16_BIT_INDICES))
		return false;
	if(!points.data)
		return false;
	if(points.stride < sizeof(PxVec3))	//should be at least one point's worth of data
		return false;
	if (quantizedCount < 4)
		return false;

	// Check topology
	if(polygons.data)
	{
		if(polygons.count < 4) // we require 2 neighbors for each vertex - 4 polygons at least
			return false;

		if(!indices.data) // indices must be provided together with polygons
			return false;

		PxU32 limit = (flags & PxConvexFlag::e16_BIT_INDICES) ? sizeof(PxU16) : sizeof(PxU32);
		if(indices.stride < limit) 
			return false;

		limit = sizeof(PxHullPolygon);
		if(polygons.stride < limit) 
			return false;
	}
	else
	{
		// We can compute the hull from the vertices
		if(!(flags & PxConvexFlag::eCOMPUTE_CONVEX))
			return false;	// If the mesh is convex and we're not allowed to compute the hull,
							// you have to provide it completely (geometry & topology).
	}

	if((flags & PxConvexFlag::ePLANE_SHIFTING) &&  vertexLimit < 4)
	{
		return false;
	}

	if (!(flags & PxConvexFlag::ePLANE_SHIFTING) && vertexLimit < 8)
	{
		return false;
	}

	if(vertexLimit > 255)
	{
		return false;
	}

	if (polygonLimit < 4)
	{
		return false;
	}

	if (polygonLimit > 255)
	{
		return false;
	}

	if (sdfDesc && !sdfDesc->isValid())
	{
		return false;
	}

	return true;
}

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
