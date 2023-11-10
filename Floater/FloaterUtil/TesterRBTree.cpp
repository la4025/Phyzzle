#include "TesterRBTree.h"
#include <iostream>
#include <random>

flt::test::TesterRBTree::TesterRBTree() : _tree(), _inputData()
{
	int inputDataCount = 1000;

	std::mt19937_64 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<int> dist(0, 1000);

	for (int i = 0; i < 1000; ++i)
	{
		_inputData.push_back(dist(rng));
	}

	for (auto& e : _inputData)
	{
		_tree.Insert(e, e);
	}
}

bool flt::test::TesterRBTree::Test()
{
	// 단순 생성 삭제 테스트
	{
		flt::RBTree<int, int> tree;
	}


	std::vector<int> v;
	v.reserve(64);
	// 전위 순회
	Preorder(_tree._root, &v);
	for (auto& e : v)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
	v.clear();

	// 중위 순회, 오름차순 정렬
	Inorder(_tree._root, &v);
	for (auto& e : v)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
	v.clear();

	// 후위 순회, 루트 노드가 마지막에 출력된다.
	Postorder(_tree._root, &v);
	for (auto& e : v)
	{
		std::cout << e << " ";
	}
	std::cout << std::endl;
	v.clear();

	// 들어간 값을 제대로 찾는 지 테스트.
	for (auto& e : _inputData)
	{
		auto it = _tree.Find(e);
		if (it == _tree.end())
		{
			return false;
		}
	}

	return true;
}

void flt::test::TesterRBTree::Preorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	outVector->push_back(pNode->key);

	Preorder(pNode->pLeft, outVector);
	Preorder(pNode->pRight, outVector);
}

void flt::test::TesterRBTree::Inorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	Inorder(pNode->pLeft, outVector);
	outVector->push_back(pNode->key);
	Inorder(pNode->pRight, outVector);
}

void flt::test::TesterRBTree::Postorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	Postorder(pNode->pLeft, outVector);
	Postorder(pNode->pRight, outVector);
	outVector->push_back(pNode->key);
}
