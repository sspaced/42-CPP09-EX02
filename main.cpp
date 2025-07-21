#include "PmergeMe.hpp"

int main(void)
{
	std::vector<unsigned  int> data;
	data.push_back(10);
	data.push_back(3);
	data.push_back(8);
	data.push_back(5);
	data.push_back(6);
	data.push_back(2);

	PmergeMe ford(data);
	ford.sort();

	for (size_t i = 0; i < 20; i++)
	{
		std::cout << i << " : " << jacobsthalSequence(i) << "\n";
	}
	return (0);
};