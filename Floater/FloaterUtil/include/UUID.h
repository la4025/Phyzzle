#pragma once
#include "copyrt.h"
#include "FloaterType.h"

#undef uuid_t
typedef struct {
	uint32  time_low;
	uint16  time_mid;
	uint16  time_hi_and_version;
	uint8   clock_seq_hi_and_reserved;
	uint8   clock_seq_low;
	byte        node[6];
} uuid_t;
