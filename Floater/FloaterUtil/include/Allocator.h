#pragma once


namespace flt
{
	template<typename T>
	class Allocator
	{
		T* Allocate(size_t elementNum)
		{
			return (T*)malloc(sizeof(T) * elementNum);
		}

		void DeAllocate(T* target)
		{
			free(target);
		}

		void Construct(T* target)
		{
			new ((void*)target) T();
		}

		void Construct(T* target, const T& value)
		{
			new ((void*)target) T(value);
		}

		void Construct(T* target, T&& value) noexcept
		{
			new ((void*)target) T(std::move(value));
		}

		void Destroy(T* target)
		{
			target->~T();
		}
	};
}
