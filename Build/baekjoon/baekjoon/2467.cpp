#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>


//struct A
//{
//	A(int a, int b) : left(a), right(b), total(left + right)
//	{}
//
//	int total = 0;
//	int left = 0;
//	int right = 0;
//
//	bool operator<(const A& _other)
//	{
//		return total < _other.total;
//	}
//	bool operator>(const A& _other)
//	{
//		return total > _other.total;
//	}
//	bool operator==(const A& _other)
//	{
//		return total == _other.total;
//	}
//
//};
//
//std::ostream& operator<<(std::ostream& _other)
//{
//	return _other << _other.left << " " << _other.right;
//}

int main()
{
	std::cin.tie();
	std::cout.tie();
	std::ios_base::sync_with_stdio(false);

	//freopen("input.txt", "rt", stdin);

	std::vector<long long> container{};

	long long N = 0;
	std::cin >> N;

	long long total = 0;

	for (long long i = 0; i < N; i++)
	{
		long long a = 0;
		std::cin >> a;

		if (a == 0)
			continue;

		container.push_back(a);
	}

	long long left = 0LL;
	long long right = container.size() - 1LL;
	long long result = 2'000'000'000LL;

	long long save0 = 0;
	long long save1 = 0;

	while (left < right)
	{
		const long long sum = container[left] + container[right];

		if (result > std::abs(sum))
		{
			result = std::abs(sum);
			save0 = left;
			save1 = right;
		}

		if (sum > 0)
		{
			right--;
		}
		else if (sum < 0)
		{
			left++;
		}
		else if (sum == 0)
		{
			break;
		}
	}

	std::cout << container[save0] << " " << container[save1];

	return 0;
}
