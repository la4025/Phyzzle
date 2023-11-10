#include "./include/FixedSizeMemoryPool.h"
#include <memory>

static constexpr int _chunkBitShift = 6;
static constexpr int _chunkSize = (1 << 6);

flt::FixedSizeMemoryPool::FixedSizeMemoryPool(uint32 pageSize) : FixedSizeMemoryPool(pageSize, 64)
{

}

flt::FixedSizeMemoryPool::FixedSizeMemoryPool(uint32 pageSize, uint32 capacity) :
	_memoryChunks(((static_cast<size_t>(capacity) >> _chunkBitShift) + 1)),
	_contiguousStartIndex(0),
	_singlePageSize(pageSize), 
	_capacity(((capacity >> _chunkBitShift) + 1) << _chunkBitShift),
	_allocatedCount(0),
	_freeStack(((static_cast<size_t>(capacity) >> _chunkBitShift) + 1) << _chunkBitShift)
{
	for (auto& chunk : _memoryChunks)
	{
		chunk = (byte*)malloc(_singlePageSize * _chunkSize);
	}
}

flt::FixedSizeMemoryPool::~FixedSizeMemoryPool()
{
	for (auto& chunk : _memoryChunks)
	{
		if (chunk)
		{
			free(chunk);
		}
	}
}

void* flt::FixedSizeMemoryPool::Alloc()
{
	++_allocatedCount;

	// 먼저 FreeStack을 확인 후 있음 Top을 return;
	if (_freeStack.size() > 0)
	{
		void* ptr = _freeStack.back();
		_freeStack.pop_back();
		return ptr;
	}

	if (_allocatedCount > _capacity)
	{
		_memoryChunks.emplace_back((byte*)malloc(_singlePageSize * _chunkSize));
		_capacity += _chunkSize;
	}

	return _memoryChunks[_allocatedCount >> _chunkBitShift] + (_allocatedCount & (_chunkSize - 1)) * _singlePageSize;

	++_contiguousStartIndex;
}

void flt::FixedSizeMemoryPool::Free(void* ptr)
{
	_freeStack.push_back(ptr);
}

void flt::FixedSizeMemoryPool::FreeAll()
{
	_allocatedCount = 0;
	_contiguousStartIndex = 0;
	_freeStack.clear();
}
