#pragma once
#include <functional>
//#include "Allocator.h"


namespace flt
{
	namespace test
	{
		class TesterRBTree;
	}
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
	template<typename Key, typename Value>
	class RBTree
	{

#pragma region Nasted
	private:
		enum class Color : char
		{
			RED,
			BLACK
		};
		struct Node
		{
			Node() : key(), value(), useFlag(0), color(Color::BLACK) {}
			Node(Key key, Value value, char flag, Color color) : key(key), value(value), useFlag(flag), color(color) {}
			Key key;
			Value value;
			char useFlag;
			Color color;
		};

		class iterator
		{
		public:
			iterator() : _dataList(nullptr), _index(0) {}
			iterator(RBTree<Key, Value>* tree, int index) : _tree(tree), _index(index) {}
			//iterator(const iterator& other);
			//iterator(iterator&& other) noexcept;
			//~iterator();

			Node& operator*()
			{
				return _tree->_dataList[_index];
			}
			Node* operator->()
			{
				return &(_tree->_dataList[_index]);
			}

		private:
			RBTree<Key, Value>* _tree;
			int _index;
		};

#pragma endregion
	public:
		RBTree();
		RBTree(int capacity);
		~RBTree();

		bool Reserve(size_t size);
		bool Insert(Key key, Value value);
		//bool Delete(Key key);
		iterator Find(const Key& key);

		Value& operator[](const Key& key);

		bool Clear();

	private:
		int FindRecursive(const Key& key, int index);

		void RotateLeft(int index);
		void RotateRight(int index);

	private:
		char _currUseFlag;
		int _capacity;
		Node* _dataList;
		std::function<bool(const Key&, const Key&)> _compareFunc;

		friend  class ::flt::test::TesterRBTree;
		friend class iterator;
	};

	template<typename Key, typename Value>
	void flt::RBTree<Key, Value>::RotateRight(int index)
	{

	}

	template<typename Key, typename Value>
	void flt::RBTree<Key, Value>::RotateLeft(int index)
	{

	}

	template<typename Key, typename Value>
	int flt::RBTree<Key, Value>::FindRecursive(const Key& key, int index)
	{
		// NIL 노드를 만난다면 종료
		if (_dataList[index].useFlag != _currUseFlag)
		{
			return 0;
		}

		const Key& cmpKey = _dataList[index].key;

		if (cmpKey == key)
		{
			return index;
		}

		// 디폴트는 cmpKey < key 일 경우 왼쪽 노드.
		// 반대의 경우 오른쪽 노드.
		if (_compareFunc(cmpKey, key))
		{
			return FindRecursive(key, index * 2 + 1);
		}
		else
		{
			return FindRecursive(key, index * 2);
		}
	}

	template<typename Key, typename Value>
	Value& flt::RBTree<Key, Value>::operator[](const Key& key)
	{
		return (Find(key))->value;
	}
}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::Find(const Key& key)
{
	int index = FindRecursive(key, 1);
	return iterator{this, index};
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::Clear()
{
	_currUseFlag ^= 1;
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::Reserve(size_t size)
{
	auto dataList = new(std::nothrow) Node[size];
	if (dataList != nullptr)
	{
		memcpy(dataList, _dataList, sizeof(Node) * size);
		delete[] _dataList;
	}

	_dataList = dataList;
	_capacity = size;
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::RBTree() : RBTree(0)
{

}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::RBTree(int capacity) :
	_currUseFlag(1),
	_capacity(capacity),
	_dataList(new Node[capacity]),
	_compareFunc([](const Key& a, const Key& b) {return a < b; })
{

}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::~RBTree()
{
	if (_dataList != nullptr)
		delete[] _dataList;
}
