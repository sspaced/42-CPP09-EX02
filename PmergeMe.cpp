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
  batch(toSort);
}

void batch(std::vector< unsigned int > toBatch) {
  std::vector< unsigned int > big;
  std::vector< unsigned int > small;

  unsigned int index = 0;
  unsigned int prev;

  for (std::vector< unsigned int >::const_iterator it = toBatch.begin();
       it != toBatch.end(); ++it) {
    if (it == toBatch.begin()) {
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

unsigned int jacobsthalSequence(unsigned int n) {
  if (n == 0)
    return (0);
  unsigned int prev = 0;
  unsigned int current = 1;
  unsigned int buff;

  for (size_t i = 0; i < n - 1; i++) {
    buff = current + 2 * prev;
    prev = current;
    current = buff;
  }
  return current;
}