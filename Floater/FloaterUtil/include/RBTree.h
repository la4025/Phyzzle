#pragma once
#include <functional>
#include "FixedSizeMemoryPool.h"
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
			Node() : Node(Key{}, Value{}, nullptr, Color::RED, & s_nil, & s_nil) {}
			Node(Key key, Value value, Color color, Node* pParent, Node* pLeft, Node* pRight) :
				key(key), value(value), color(color),
				pParent(pParent), pLeft(pLeft), pRight(pRight) {}

			Node* GetGrandParent()
			{
				if (pParent == nullptr)
				{
					return nullptr;
				}

				return pParent->pParent;
			}

			Node* GetUncle()
			{
				Node* pGrandParent = GetGrandParent();
				if (pGrandParent == nullptr)
				{
					return nullptr;
				}

				if (pGrandParent->pLeft == pParent)
				{
					return pGrandParent->pRight;
				}
				else
				{
					return pGrandParent->pLeft;
				}
			}

			Node* GetSibling()
			{
				if (pParent == nullptr)
				{
					return nullptr;
				}

				if (pParent->pLeft == this)
				{
					return pParent->pRight;
				}
				else
				{
					return pParent->pLeft;
				}
			}

			Key key;
			Value value;
			Color color;

			Node* pParent;
			Node* pLeft;
			Node* pRight;
		};

		class iterator
		{
		public:
			iterator() : iterator(nullptr) {}
			iterator(Node* pNode) : _pNode(pNode) {}
			//iterator(const iterator& other);
			//iterator(iterator&& other) noexcept;
			//~iterator();

			bool operator==(const iterator& other)
			{
				return _pNode == other._pNode;
			}

			bool operator!=(const iterator& other)
			{
				return _pNode != other._pNode;
			}

			Node& operator*()
			{
				return *_pNode;
			}
			Node* operator->()
			{
				return _pNode;
			}

		private:
			Node* _pNode;
		};

#pragma endregion
	public:
		RBTree();
		~RBTree();

		bool Insert(Key key, Value value);
		void erase(Key key);
		void erase(iterator it);
		iterator Find(const Key& key);

		Value& operator[](const Key& key);

		bool Clear();

		// iterator 관련 함수들
		iterator begin();
		iterator end();

	private:
		Node* FindRecursive(const Key& key, Node* pNode);

		Node* BSTInsert(const Key& key, const Value& value);
		bool BSTInsertRecursive(Node* pCmpNode, Node* pNode, int* outDepth);

		void InsertCase1(Node* pNode);
		void InsertCase2(Node* pNode);
		void InsertCase3(Node* pNode);
		void InsertCase4(Node* pNode);
		void InsertCase5(Node* pNode);

		void RotateRight(Node* pNode);
		void RotateLeft(Node* pNode);

		void delete_case1(Node* pNode);
		void delete_case2(Node* pNode);
		void delete_case3(Node* pNode);
		void delete_case4(Node* pNode);
		void delete_case5(Node* pNode);
		void delete_case6(Node* pNode);

		void 

	private:
		std::function<bool(const Key&, const Key&)> _compareFunc;
		int _blackHigh;
		Node* _root;
		FixedSizeMemoryPool _memoryPool;

		static Node s_nil;

		friend  class ::flt::test::TesterRBTree;
		friend class iterator;
	};

	template<typename Key, typename Value>
	void flt::RBTree<Key, Value>::erase(Key key)
	{

	}

	template<typename Key, typename Value>
	void flt::RBTree<Key, Value>::erase(iterator it)
	{
		erase(it->key);
	}

	// static 멤버 변수 정의
	template<typename Key, typename Value>
	typename RBTree<Key, Value>::Node RBTree<Key, Value>::s_nil = Node{ Key{}, Value{}, Color::BLACK, nullptr, nullptr, nullptr };
}

//----------------------------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------------------------

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::RotateRight(Node* pNode)
{
	Node* pLeft = pNode->pLeft;
	Node* pParent = pNode->pParent;

	if (pLeft->pRight != &s_nil)
	{
		pLeft->pRight->pParent = pNode;
	}

	pNode->pLeft = pLeft->pRight;
	pNode->pParent = pLeft;
	pLeft->pRight = pNode;
	pLeft->pParent = pParent;

	if (pParent != nullptr)
	{
		if (pParent->pLeft == pNode)
		{
			pParent->pLeft = pLeft;
		}
		else
		{
			pParent->pRight = pLeft;
		}
	}
	else
	{
		_root = pLeft;
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::RotateLeft(Node* pNode)
{
	Node* pRight = pNode->pRight;
	Node* pParent = pNode->pParent;

	if (pRight->pLeft != &s_nil)
	{
		pRight->pLeft->pParent = pNode;
	}

	pNode->pRight = pRight->pLeft;
	pNode->pParent = pRight;
	pRight->pLeft = pNode;
	pRight->pParent = pParent;

	if (pParent != nullptr)
	{
		if (pParent->pLeft == pNode)
		{
			pParent->pLeft = pRight;
		}
		else
		{
			pParent->pRight = pRight;
		}
	}
	else
	{
		_root = pRight;
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase1(Node* pNode)
{
	// 1. Node가 루트노드일 경우 자신을 블랙으로 바꾸고 종료.
	if (pNode == _root)
	{
		pNode->color = Color::BLACK;
	}
	else
	{
		InsertCase2(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase2(Node* pNode)
{
	// 2. 부모가 블랙일 경우 종료.
	if (pNode->pParent->color == Color::BLACK)
	{
		return;
	}
	else
	{
		InsertCase3(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase3(Node* pNode)
{
	ASSERT(pNode->pParent->color == Color::RED, "부모의 색은 빨강 이어야합니다.");

	Node* pUncle = pNode->GetUncle();

	if (pUncle != nullptr && pUncle->color == Color::RED)
	{
		pNode->pParent->color = Color::BLACK;
		pUncle->color = Color::BLACK;

		Node* pGrandParent = pNode->GetGrandParent();
		pGrandParent->color = Color::RED;

		InsertCase1(pGrandParent);
	}
	else
	{
		InsertCase4(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase4(Node* pNode)
{
	Node* pGrandParent = pNode->GetGrandParent();

	// 내가 오른쪽 자식이고, 부모는 왼쪽 자식일 경우
	if (pNode == pNode->pParent->pRight && pNode->pParent == pGrandParent->pLeft)
	{
		RotateLeft(pNode->pParent);
		pNode = pNode->pLeft;
	}
	// 내가 왼쪽 자식이고 부모는 오른쪽 자식일 경우
	else if (pNode == pNode->pParent->pLeft && pNode->pParent == pGrandParent->pRight)
	{
		RotateRight(pNode->pParent);
		pNode = pNode->pRight;
	}

	InsertCase5(pNode);
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase5(Node* pNode)
{
	Node* pGrandParent = pNode->GetGrandParent();

	pNode->pParent->color = Color::BLACK;
	pGrandParent->color = Color::RED;

	if (pNode == pNode->pParent->pLeft)
	{
		RotateRight(pGrandParent);
	}
	else
	{
		RotateLeft(pGrandParent);
	}
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::Insert(Key key, Value value)
{
	Node* node = BSTInsert(key, value);

	if (node == nullptr)
	{
		return false;
	}

	InsertCase1(node);

	return true;
}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::begin()
{
	return iterator{ _root };
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::end()
{
	return iterator{ &s_nil };
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::Node* flt::RBTree<Key, Value>::BSTInsert(const Key& key, const Value& value)
{
	//Node* pNode = new(std::nothrow) Node{ key, value, Color::RED, nullptr, &s_nil, &s_nil };

	void* pMemory = _memoryPool.Alloc();
	if (pMemory == nullptr)
	{
		return nullptr;
	}

	Node* pNode = new (pMemory) Node{ key, value, Color::RED, nullptr, &s_nil, &s_nil };

	int depth = 1;

	if (_root == &s_nil)
	{
		_root = pNode;
		return pNode;
	}

	if (BSTInsertRecursive(_root, pNode, &depth))
	{
		return pNode;
	}

	_memoryPool.Free(pNode);
	return nullptr;
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::BSTInsertRecursive(Node* pCmpNode, Node* pNode, int* outDepth)
{
	(*outDepth)++;

	// key중복은 허용 안함.
	if (pCmpNode->key == pNode->key)
	{
		*outDepth = 0;
		return false;
	}

	Node** ppNextCmpNode = nullptr;
	if (_compareFunc(pCmpNode->key, pNode->key))
	{
		ppNextCmpNode = &pCmpNode->pRight;
	}
	else
	{
		ppNextCmpNode = &pCmpNode->pLeft;
	}

	if (*ppNextCmpNode == &s_nil)
	{
		*ppNextCmpNode = pNode;
		pNode->pParent = pCmpNode;
		return true;
	}

	return BSTInsertRecursive(*ppNextCmpNode, pNode, outDepth);
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::Node* flt::RBTree<Key, Value>::FindRecursive(const Key& key, Node* pNode)
{
	// NIL 노드를 만난다면 종료
	if (pNode == &s_nil)
	{
		return nullptr;
	}

	const Key& cmpKey = pNode->key;

	// key를 찾았다면 종료
	if (cmpKey == key)
	{
		return pNode;
	}

	// 기본적으로 cmpKey < key 일 경우 왼쪽 노드.
	// 반대의 경우 오른쪽 노드.
	if (_compareFunc(cmpKey, key))
	{
		return FindRecursive(key, pNode->pRight);
	}
	else
	{
		return FindRecursive(key, pNode->pLeft);
	}
}

template<typename Key, typename Value>
Value& flt::RBTree<Key, Value>::operator[](const Key& key)
{
	Insert(key, Value{});

	return (Find(key))->value;
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::Find(const Key& key)
{
	return iterator{ FindRecursive(key, _root) };
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::Clear()
{
	_root = nullptr;
	_memoryPool.FreeAll();
}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::RBTree() :
	_compareFunc([](const Key& a, const Key& b) {return a < b; }),
	_blackHigh(0),
	_root(&s_nil),
	_memoryPool(sizeof(Node), 1000)
{

}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::~RBTree()
{

}
