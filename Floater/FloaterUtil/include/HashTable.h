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
		struct Node
		{
			int64 version;
			Key key;
			Value value;
		};
		class iterator
		{

		};
#pragma endregion
	public:
		HashTable();
		~HashTable();

	private:
		void Rehash();
		float LoadFactor() const;

	private:
		std::function<int(const Key&)> _hash;
		int _capacity;
		int _size;
		Node* _table;
		uint32* _buckets;
		int64 _version;


		friend class test::TesterHashTable;
	};

	template<typename Key, typename Value>
	float flt::HashTable<Key, Value>::LoadFactor() const
	{
		return (float)_size / (_capacity - 1);
	}

}

template<typename Key, typename Value>
flt::HashTable<Key, Value>::HashTable() : 
	_capacity(512), _size(0),
	_table(new Node[_capacity]), _buckets(new uint32[_capacity]),
	_version(0)
{

}

template<typename Key, typename Value>
flt::HashTable<Key, Value>::~HashTable()
{
	delete[] _table;
	delete[] _buckets;
}
