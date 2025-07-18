#include "PmergeMe.hpp"

int main(void)
{
	std::vector<unsigned  int> data;
	data.push_back(5);
	data.push_back(4);
	data.push_back(3);
	data.push_back(2);
	data.push_back(1);

	PmergeMe ford(data);
	ford.sort();
	return (0);
};