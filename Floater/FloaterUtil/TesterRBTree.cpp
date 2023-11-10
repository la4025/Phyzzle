#include "TesterRBTree.h"

flt::test::TesterRBTree::TesterRBTree() : _tree(64)
{
	_tree._dataList[1] = { 7, 7, (char)1, flt::RBTree<int, int>::Color::BLACK };
	_tree._dataList[2] = { 3, 3, (char)1, flt::RBTree<int, int>::Color::BLACK };
	_tree._dataList[3] = { 18, 18, (char)1, flt::RBTree<int, int>::Color::RED };
	_tree._dataList[6] = { 10, 10, (char)1, flt::RBTree<int, int>::Color::BLACK };
	_tree._dataList[7] = { 22, 22, (char)1, flt::RBTree<int, int>::Color::BLACK };
	_tree._dataList[12] = { 8, 8, (char)1, flt::RBTree<int, int>::Color::RED };
	_tree._dataList[13] = { 11, 11, (char)1, flt::RBTree<int, int>::Color::RED };
	_tree._dataList[15] = { 26, 26, (char)1, flt::RBTree<int, int>::Color::RED };
}

bool flt::test::TesterRBTree::Test()
{
	// 단순 생성 삭제 테스트
	{
		flt::RBTree<int, int> tree;
	}

	// 들어간 값을 제대로 찾는 지 테스트.
	int value = _tree[7];
	value = _tree[3];
	value = _tree[18];
	value = _tree[10];
	value = _tree[22];
	value = _tree[8];
	value = _tree[11];
	value = _tree[26];
	value = _tree[27];
	value = _tree[7];
	value = _tree[3];
	value = _tree[18];
	value = _tree[10];
	value = _tree[22];
	value = _tree[8];
	value = _tree[11];
	value = _tree[26];
	value = _tree[27];



	return false;
}
