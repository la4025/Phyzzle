#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>


struct A
{
	constexpr A() : left(), right(), total() {}
	A(int a, int b) :
		total(a * a + b * b),
		left(a),
		right(b)
	{}

	A(const A& _other) :
		total(_other.total),
		left(_other.left),
		right(_other.right)
	{}

	int total = 0;
	int left = 0;
	int right = 0;

	bool operator<(const A& _other) const
	{
		if (total < _other.total)
			return true;
			
		if (left < _other.left)
			return true;

		if (right < _other.right)
			return true;

		return false;
	}

	bool operator>(const A& _other) const
	{
		if (total > _other.total)
			return true;

		if (left > _other.left)
			return true;

		if (right > _other.right)
			return true;

		return false;
	}

	bool operator==(const A& _other) const
	{
		if (total != _other.total)
			return false;

		if (left != _other.left)
			return false;

		if (right != _other.right)
			return false;

		return true;
	}
};

std::set<A> mapmap;

template<int H, int W>
constexpr void Set()
{
	for (int i = 1; i < H; i++)
	{
		for (int j = i + 1; j < W; j++)
		{
			mapmap.insert(A(i, j));
		}
	}
}


int main()
{
	std::cin.tie();
	std::cout.tie();
	std::ios_base::sync_with_stdio(false);

	freopen("input.txt", "rt", stdin);

	Set<151, 151>();

	int h = 0;
	int w = 1;

	int H = 0;
	int W = 0;

	while (true)
	{
		std::cin >> h >> w;

		if (h == 0 && w == 0)
			break;

		int area = std::pow(h, 2) + std::pow(w, 2);

	}





	std::cout << H << " " << W;

	return 0;
}
