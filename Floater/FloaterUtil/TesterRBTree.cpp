#include "TesterRBTree.h"
#include <iostream>
#include <random>

flt::test::TesterRBTree::TesterRBTree() : TesterRBTree(std::random_device()())
{
}

flt::test::TesterRBTree::TesterRBTree(unsigned int seed) : _inputDataCount(1000), _tree(), _inputData()
{
	std::mt19937_64 rng;
	std::cout << "seed : " << seed << std::endl;
	rng.seed(seed);
	std::uniform_int_distribution<int> dist(0, _inputDataCount * 10);

	_inputData.reserve(_inputDataCount);
	for (int i = 0; i < _inputDataCount; ++i)
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
	v.reserve(_inputDataCount);
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

	// iterator++로 순회 시 중위순회인지?
	{
		std::vector<int> iterVec;
		iterVec.reserve(_inputDataCount);
		
		//auto end = _tree.end();
		//for (auto it = _tree.begin(); it != end; ++it)
		//{
		//	iterVec.push_back(it->key);
		//}

		for (auto& e : _tree)
		{
			iterVec.push_back(e.key);
		}

		std::cout << std::endl;

		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] != iterVec[i])
			{
				ASSERT(false, "중위 순회 오류");
			}
		}
	}

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
			ASSERT(false, "값이 다 안들어감.");
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
