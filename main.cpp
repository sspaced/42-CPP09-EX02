#include "PmergeMe.hpp"

int main(void)
{
	std::vector<unsigned  int> data;
	data.push_back(1);
	data.push_back(2);
	data.push_back(3);

	PmergeMe ford(data);

	std::cout << ford << std::endl;
	return (0);
};