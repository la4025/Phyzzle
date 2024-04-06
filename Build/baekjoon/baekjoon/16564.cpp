#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::cin.tie();
	std::cout.tie();
	std::ios_base::sync_with_stdio(false);

	int N = 0;
	int K = 0;
	std::cin >> N >> K;

	std::vector<int> X{};
	std::vector<int> X2{};

	// 입력값을 벡터에 넣음
	for (size_t i = 0; i < N; i++)
	{
		int a = 0;
		std::cin >> a;

		X.push_back(a);
	}

	// 순서대로 정렬함
	std::sort(X.begin(), X.end());

	// 평탄화 작업을 위해서 몇 개가 필요한지 계산함.
	for (size_t i = 0; i < N; i++)
	{
		if (i == 0)
		{
			X2.push_back(0);
			continue;
		}

		// 앞에 공구리 치기 위해 필요한 개수
		// + 자기보다 앞에 있는 친구들이 평평하다는 가정 하에 필요한 시멘트 개수
		// (단차 * 인덱스 개수)
		X2.push_back(X2[i - 1] + (X[i] - X[i - 1]) * i);
	}

	size_t min = 0;
	size_t mid = 0;
	size_t max = X2.size() - 1;

	while (min <= max)
	{
		mid = (min + max) / 2;

		// K개로 mid번째까지 공구리 가능한가?
		if (X2[mid] == K)
		{
			break;
		}
		else if (X2[mid] < K)
		{
			// 충분히 가능함
			min = mid + 1;
		}
		else
		{
			// 부족하면 낮추자
			max = mid - 1;
		}
	}

	size_t result = X[min];

	while (K - X2[min] < min)
	{
		result++;
	}

	std::cout << result;

	return 0;
}
