#pragma once
#include <functional>

namespace flt
{
	namespace test
	{
		class TesterHashTable;
	}

	template<typename Key, typename Value>
	class HashTable
	{
#pragma region Nasted
	private:
		class iterator
		{

		};
#pragma endregion
	public:
		HashTable();
		~HashTable();

	private:
		void Rehash();

	private:
		std::function<int(const Key&)> _hash;
		int _capacity;
		int _size;

		friend class test::TesterHashTable;
	};
}
