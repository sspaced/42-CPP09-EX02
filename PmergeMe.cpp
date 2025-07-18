#include "PmergeMe.hpp"

PmergeMe::~PmergeMe() {};

PmergeMe::PmergeMe(std::vector<unsigned int> &toSort) : _toSort(toSort) {};

const std::vector<unsigned int> &PmergeMe::getToSort() const {
  return (_toSort);
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &data) {
  const std::vector<unsigned int> &toSort = data.getToSort();
  std::vector<unsigned int>::const_iterator it;
  for (it = toSort.begin(); it != toSort.end(); it++) {
    os << *it << " ";
  }
  return os;
};

std::vector<std::vector<unsigned int > >
PmergeMe::batch(const std::vector<unsigned int> &toSort) const{
  std::vector<std::vector<unsigned int > > result;
  std::vector<unsigned int> pair;

  for (std::vector<unsigned int>::const_iterator it = toSort.begin();
       it != toSort.end(); ++it) {
    pair.push_back(*it);
    if (pair.size() == 2) {
      result.push_back(pair);
      pair.clear();
    }
  }

  if (!pair.empty()) {
    result.push_back(pair);
  }

  return result;
}

void PmergeMe::displayBatch(
    const std::vector<std::vector<unsigned int > > &toDisplay) const{
  for (std::vector<std::vector<unsigned int > >::const_iterator it =
           toDisplay.begin();
       it != toDisplay.end(); ++it) {
		for (std::vector<unsigned int>::const_iterator itPair = (*it).begin(); itPair != (*it).end(); ++itPair) {
			std::cout << *itPair;
		}
		std::cout << std::endl;
  }
};

void PmergeMe::sort() const{
	std::vector<std::vector<unsigned int> > batchedData = batch(this->getToSort());
    displayBatch(batchedData);
};