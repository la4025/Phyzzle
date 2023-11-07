#pragma once
#include <map>
#include "Allocator.h"


namespace flt
{
	/// <summary>
	/// 레드블랙트리.
	/// 노드를 개별 동적할당 하지 않고 배열을 이용한다.
	/// 
	/// RBTree 규칙
	/// 1. 노드는 레드 혹은 블랙이다.
	/// 2. 루트 노드는 반드시 블랙이다.
	/// 3. 모든 NIL은 블랙이다.
	/// 4. 루트 노드부터 리프 노트까지 블랙의 갯수는 항상 같다.
	/// 5. 레드 노드의 자식은 모두 블랙이다.
	/// </summary>
	template<typename Key, typename Value, typename KeyCompare = std::less<Key>, Allocator_<typename Key> KeyAllocator, Allocator_<typename Value> ValueAllocator>
	class RBTree
	{
	private:
		struct Node
		{
			Node() : key(), value(), useFlag(0), color(Color::BLACK) {}
			Key key;
			Value value;
			char useFlag;
			Color color;
		};

		class iterator
		{
		public:
			iterator() : _dataList(nullptr), _index(-1);
			iterator(const iterator& other);
			iterator(iterator&& other) noexcept;
			~iterator();

			iterator& operator=(const iterator& other)
			{

			}
			iterator& operator=(iterator&& other) noexcept
			{

			}

			bool operator==(const iterator& other)
			{

			}

			bool operator!=(const iterator& other)
			{

			}



			iterator& operaotr++()
			{

			}

			iterator& operator--()
			{

			}

			iterator operator++(int)
			{

			}

			iterator operator--(int)
			{

			}

		private:
			Node* _dataList;
			int _index;
		};

	public:
		RBTree();
		RBTree(int capacity);
		~RBTree();

		bool Reserve(size_t size);
		bool Insert(Key key, Value value);
		bool Delete(Key key);
		Value& Find(Key key);

		Value& operator[](Key key);

		bool Clear();

	private:
		enum class Color : char
		{
			RED,
			BLACK
		};



		char _currUseFlag;
		int _capacity;
		Node* _dataList;
	};
}


template<typename Key, typename Value, typename KeyCompare, typename Allocator>
Value& flt::RBTree<Key, Value, KeyCompare, Allocator>::Find(Key key)
{

}

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
bool flt::RBTree<Key, Value, KeyCompare, Allocator>::Clear()
{
	_currUseFlag ^= 1;
}

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
bool flt::RBTree<Key, Value, KeyCompare, Allocator>::Reserve(size_t size)
{
	auto dataList = new Node[size];
	if (dataList != nullptr)
	{
		memcpy(dataList, _dataList, sizeof(Node) * cpacity);
		delete[] _dataList;
	}

	_capacity = size;
}

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
flt::RBTree<Key, Value, KeyCompare, Allocator>::RBTree() :
	_currUseFlag(0),
	_capacity(0),
	_dataList(nullptr)
{

}


template<typename Key, typename Value, typename KeyCompare, typename Allocator>
flt::RBTree<Key, Value, KeyCompare, Allocator>::RBTree(int capacity) :
	_currUseFlag(0),
	capacity(capacity),
	_dataList(new Node[capacity])
{

}


template<typename Key, typename Value, typename KeyCompare, typename Allocator>
flt::RBTree<Key, Value, KeyCompare, Allocator>::~RBTree()
{
	if (_dataList != nullptr)
		delete[] _dataList;
}
