#pragma once

#include <iostream>
#include <vector>

class PmergeMe {
public:
  ~PmergeMe();
  PmergeMe(std::vector<unsigned int> &toSort);
  const std::vector<unsigned int> &getToSort() const;
  std::vector<std::vector<unsigned int > > batch(const std::vector<unsigned int>& toSort) const;
  void displaySplit(const std::vector<std::vector<unsigned int > > &toDisplay) const;
  void sort() const;

private:
  std::vector<unsigned int> _toSort;
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &PmergeMe);