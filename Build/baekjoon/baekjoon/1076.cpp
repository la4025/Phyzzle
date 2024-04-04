#include <iostream>
#include <map>

int main()
{
	std::map<std::string, std::pair<int, int>> table;

	table.insert(std::make_pair("black", std::make_pair(0, 1)));
	table.insert(std::make_pair("brown", std::make_pair(1, 10)));
	table.insert(std::make_pair("red", std::make_pair(2, 100)));
	table.insert(std::make_pair("orange", std::make_pair(3, 1'000)));
	table.insert(std::make_pair("yellow", std::make_pair(4, 10'000)));
	table.insert(std::make_pair("green", std::make_pair(5, 100'000)));
	table.insert(std::make_pair("blue", std::make_pair(6, 1'000'000)));
	table.insert(std::make_pair("violet", std::make_pair(7, 10'000'000)));
	table.insert(std::make_pair("grey", std::make_pair(8, 100'000'000)));
	table.insert(std::make_pair("white", std::make_pair(9, 1'000'000'000)));

	std::string input;

	long long result = 0L;

	for (int i = 0; i < 3; i++)
	{
		std::cin >> input;

		if (i == 0)
		{
			result += table[input].first * 10;
		}
		if (i == 1)
		{
			result += table[input].first;
		}
		if (i == 2)
		{
			result *= table[input].second;
		}
	}

	std::cout << result;

	return 0;
}
