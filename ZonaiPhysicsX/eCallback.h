#pragma once

namespace ZonaiPhysics
{
	enum class eCallback : int
	{
		eError,
		eTriggerEnter,
		eTriggerExit,
		eCollisionEnter,
		eCollisionStay,
		eCollisionExit,
	};
}