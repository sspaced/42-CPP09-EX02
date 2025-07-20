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

	std::vector<uint64_t> seq = jacobsthalSequence(data.size());

	for (size_t i = 0; i < 100; i++)
	{
		std::cout << seq[i] << "\n";
	}
	return (0);
};