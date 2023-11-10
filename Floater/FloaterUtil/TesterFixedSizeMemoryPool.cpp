#include "TesterFixedSizeMemoryPool.h"

bool flt::test::TesterFixedSizeMemoryPool::Test()
{
	if (_pool._capacity != 64)
	{
		return false;
	}

	for (int i = 0; i < 64; ++i)
	{
		int* p = (int*)_pool.Alloc();
		*p = i;
	}
}
