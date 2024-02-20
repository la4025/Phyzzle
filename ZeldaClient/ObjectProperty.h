#pragma once

namespace PurahEngine
{
	enum class ObjectProperty : unsigned int
	{
		eConductor				= 0x01 << 0,	// 전도체
		eEnalbeHandling			= 0x01 << 1,	// 울트라핸드 가능 여부
		e2						= 0x01 << 2,	// 
		e3						= 0x01 << 3,
		e4						= 0x01 << 4,
		e5						= 0x01 << 5,
		e6						= 0x01 << 6,
		e7						= 0x01 << 7,
	};
}

