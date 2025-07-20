#include "PmergeMe.hpp"

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const std::vector< unsigned int >& toSort)
    : _toSort(toSort) {}

const std::vector< unsigned int >& PmergeMe::getToSort() const {
  return (_toSort);
}

std::ostream& operator<<(std::ostream& os, const PmergeMe& data) {
  const std::vector< unsigned int >& toSort = data.getToSort();
  std::vector< unsigned int >::const_iterator it;
  for (it = toSort.begin(); it != toSort.end(); it++) {
    os << *it << " ";
  }
  return os;
}

void PmergeMe::sort() const {
  std::vector< unsigned int > toSort = this->getToSort();
  std::vector< unsigned int > big;
  std::vector< unsigned int > small;

  unsigned int index = 0;
  unsigned int prev;

  for (std::vector< unsigned int >::const_iterator it = toSort.begin();
       it != toSort.end(); ++it) {
    if (it == toSort.begin()) {
      prev = *it;
      std::cout << "First prev : " << prev << "\n";
    }
    if (index % 2) {
      std::cout << "==================================" << "\n";
      std::cout << "Index : " << index << "\n";
      std::cout << "New pair sorted : ";
      if (*it > prev) {
        std::cout << "big = " << *it << " small = " << prev << "\n";
        big.push_back(*it);
        small.push_back(prev);
      } else {
        std::cout << "big = " << prev << " small = " << *it << "\n";
        big.push_back(prev);
        small.push_back(*it);
      }
    }
    prev = *it;
    index++;
  }
  std::cout << "big length : " << big.size() << "\n";
  std::cout << "big length : " << small.size() << "\n";
}

// std::vector<unsigned int> subSort(std::vector<unsigned int> &toSort)
// {
// 	std::vector< unsigned int > big;
//   	std::vector< unsigned int > small;
// }

std::vector<uint64_t> jacobsthalSequence(unsigned int n)
{
	std::vector<uint64_t> sequence;
	unsigned int prev = 0;
	unsigned int current = 1;
	unsigned int buff;

	sequence.push_back(prev);
	sequence.push_back(current);

	for (size_t i = 0; i < n; i++) {
		buff = current + 2 * prev;
		prev = current;
		current = buff;
		sequence.push_back(current);
	}
	return sequence;
}