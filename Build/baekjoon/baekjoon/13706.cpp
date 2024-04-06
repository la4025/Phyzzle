#include <iostream>
#include <cmath>

int main()
{
	std::cin.tie();
	std::cout.tie();
	std::ios_base::sync_with_stdio(false);

	unsigned long long N = 0;

	std::cin >> N;

	unsigned long long low = 1;
	unsigned long long mid = 0;
	unsigned long long high = N;

	while (low <= high)
	{
		mid = (low + high) / 2;

		unsigned long long check = mid * mid;

		if (check == N)
		{
			break;
		}
		else if (check > N)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	std::cout << low;

	return 0;
}