#include <iostream>

bool CanJump(unsigned long long bridge, unsigned long long count)
{
	// count 만큼 뛸수 있는 칸
	unsigned long long d = count * (count + 1) / 2;

	// d보다 크면 충분히 뛸 수 있음
	return d <= bridge;
}

void Step()
{
	unsigned long long minCount = 1;
	unsigned long long midCount = 0;
	unsigned long long maxCount = 1'000'000'000;

	unsigned long long result = 0;
	unsigned long long bridge = 0;
	std::cin >> bridge;

	while (minCount <= maxCount)
	{
		midCount = (minCount + maxCount) / 2;

		// midCount 횟수로 bridge를 넘어갈 수 있나?
		const bool can = CanJump(bridge, midCount);

		if (can)
		{
			// min ~ mid 부분은 전부 가능
			result = midCount;
			minCount = midCount + 1;
		}
		else
		{
			// mid ~ max 부분은 전부 불가능
			result = midCount - 1;
			maxCount = midCount - 1;
		}
	}

	std::cout << result << std::endl;
}

int main()
{
	std::cin.tie();
	std::cout.tie();
	std::ios_base::sync_with_stdio(false);

	long long testCase = 1;
	std::cin >> testCase;

	while (testCase--)
	{
		Step();
	}

	return  0;
}