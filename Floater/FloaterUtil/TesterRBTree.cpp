#include "TesterRBTree.h"
#include <iostream>
#include <random>

flt::test::TesterRBTree::TesterRBTree() : TesterRBTree(std::random_device()())
{
}

flt::test::TesterRBTree::TesterRBTree(unsigned int seed) :
	_inputDataCount(10000),
	_tree(_inputDataCount), _map(), _unorderedMap(),
	_inputData(),
	_timer(),
	nil(flt::RBTree<int, int>::s_nil)
{
	std::mt19937_64 rng;
	rng.seed(seed);
	std::uniform_int_distribution<int> dist(0, _inputDataCount * 100);

	_inputData.reserve(_inputDataCount);
	for (int i = 0; i < _inputDataCount; ++i)
	{
		_inputData.push_back(dist(rng));
	}

	std::wcout << L"중복 있는 데이터 " << _inputDataCount << L"개 생성 완료." << std::endl;
	_timer.Start();
	for (auto& e : _inputData)
	{
		_tree.Insert(e, e);
	}
	std::wcout << L"트리 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		_map.insert(std::make_pair(e, e));
	}
	std::wcout << L"맵 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		_unorderedMap.insert(std::make_pair(e, e));
	}
	std::wcout << L"해쉬 맵 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;
}

bool flt::test::TesterRBTree::Test()
{
	bool retval = true;

	// 단순 생성 삭제 테스트
	{
		flt::RBTree<int, int> tree;
	}

	std::vector<int> v;

	// 중위 순회, 오름차순 정렬
	Inorder(_tree._root, &v);

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

		//std::cout << std::endl;

		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] != iterVec[i])
			{
				ASSERT(false, "중위 순회 오류");
				retval = retval && false;
			}
		}
	}

	v.clear();

	// 트리의 최대 깊이가 최저 깊이의 2배가 넘지 않는지 체크.
	{
		int minDepth = INT_MAX;
		int maxDepth = 0;

		CheckDepthRecursive(_tree._root, 0, &minDepth, &maxDepth);

		if (minDepth * 2 < maxDepth)
		{
			ASSERT(false, "깊이 차이가 2배 이상임.");
			retval = retval && false;
		}

		int blackDepth = -1;
		retval = retval && CheckBlackDepthRecursive(_tree._root, 0, &blackDepth);

		printf("depth | min | max | black\n");
		printf("      | %03d | %03d |  %03d\n", maxDepth, minDepth, blackDepth);
	}

	// 들어간 값을 제대로 찾는 지 테스트.
	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _tree.Find(e);
		if (it == _tree.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"트리 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _map.find(e);
		if (it == _map.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"맵 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _unorderedMap.find(e);
		if (it == _unorderedMap.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"해쉬 맵 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_tree.Clear();
	std::wcout << L"트리 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_map.clear();
	std::wcout << L"맵 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_unorderedMap.clear();
	std::wcout << L"해쉬 맵 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;
	return retval;
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

void flt::test::TesterRBTree::CheckDepthRecursive(Node* pNode, int depth, int* outMinDepth, int* outMaxDepth)
{
	if (pNode == &nil)
	{
		*outMinDepth > depth ? *outMinDepth = depth : 0;
		*outMaxDepth < depth ? *outMaxDepth = depth : 0;
		return;
	}
	depth++;
	CheckDepthRecursive(pNode->pLeft, depth, outMinDepth, outMaxDepth);
	CheckDepthRecursive(pNode->pRight, depth, outMinDepth, outMaxDepth);
}

bool flt::test::TesterRBTree::CheckBlackDepthRecursive(Node* pNode, int depth, int* outBlackDepth)
{
	if (pNode == &nil)
	{
		if (*outBlackDepth == -1)
		{
			*outBlackDepth = depth;
		}
		else if (depth != *outBlackDepth)
		{
			ASSERT(false, "depth 다름.");
			return false;
		}
		return true;
	}

	if (pNode->color == Color::BLACK)
	{
		depth++;
	}

	bool retval = CheckBlackDepthRecursive(pNode->pLeft, depth, outBlackDepth);
	retval = retval && CheckBlackDepthRecursive(pNode->pRight, depth, outBlackDepth);

	return retval;
}
