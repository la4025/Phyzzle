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

#ifndef PX_FORCE_MODE_H
#define PX_FORCE_MODE_H

#include "foundation/PxPreprocessor.h"

/** \addtogroup physics
@{
*/

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Parameter to addForce() and addTorque() calls, determines the exact operation that is carried out.

@see PxRigidBody.addForce() PxRigidBody.addTorque()
*/
struct PxForceMode
{
	enum Enum
	{
		eFORCE,             //!< 매개변수의 단위는 질량 * 길이 / 시간^2, 즉, 힘의 단위를 가짐
		eIMPULSE,           //!< 매개변수의 단위는 질량 * 길이 / 시간, 즉, 힘 * 시간의 단위를 가짐
		eVELOCITY_CHANGE,   //!< 매개변수의 단위는 길이 / 시간, 즉, 질량과 무관하게 속도 변화를 의미함
		eACCELERATION       //!< 매개변수의 단위는 길이 / 시간^2, 즉, 가속도를 의미함. 이는 힘처럼 취급되지만 통합 전에 질량이 나누어지지 않음
	};
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
