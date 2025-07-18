#include "PmergeMe.hpp"

PmergeMe::~PmergeMe() {};

PmergeMe::PmergeMe(std::vector<unsigned int> &toSort) : _toSort(toSort) {};

const std::vector<unsigned int> &PmergeMe::getToSort() const {
  return (_toSort);
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &PmergeMe) {
  const std::vector<unsigned int> &toSort = PmergeMe.getToSort();
  std::vector<unsigned int>::const_iterator it;
  for (it = toSort.cbegin(); it != toSort.cend(); it++) {
    os << *it << " ";
  }
  return os;
};