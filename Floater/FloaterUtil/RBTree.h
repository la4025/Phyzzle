#pragma once
#include <map>


template<typename Key, typename Value, typename KeyCompare = (typename)Key.operator<(const Key&), typename Allocator>
class RBTree
{
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

	struct Node
	{
		Node() : key(), value(), useFlag(0), color(Color::BLACK) {}
		Key key;
		Value value;
		char useFlag;
		Color color;
	};

	char _currUseFlag;
	int _capacity;
	Node* _dataList;
};

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
Value& RBTree<Key, Value, KeyCompare, Allocator>::Find(Key key)
{

}

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
bool RBTree<Key, Value, KeyCompare, Allocator>::Clear()
{
	_currUseFlag ^= 1;
}

template<typename Key, typename Value, typename KeyCompare, typename Allocator>
bool RBTree<Key, Value, KeyCompare, Allocator>::Reserve(size_t size)
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
RBTree<Key, Value, KeyCompare, Allocator>::RBTree() :
	_currUseFlag(0),
	_capacity(0),
	_dataList(nullptr)
{

}


template<typename Key, typename Value, typename KeyCompare, typename Allocator>
RBTree<Key, Value, KeyCompare, Allocator>::RBTree(int capacity) :
	_currUseFlag(0),
	capacity(capacity),
	_dataList(new Node[capacity])
{

}


template<typename Key, typename Value, typename KeyCompare, typename Allocator>
RBTree<Key, Value, KeyCompare, Allocator>::~RBTree()
{
	if (_dataList != nullptr)
		delete[] _dataList;
}
