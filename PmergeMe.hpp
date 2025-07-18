#pragma once

#include <iostream>
#include <vector>

class PmergeMe {
public:
  ~PmergeMe();
  PmergeMe(std::vector<unsigned int> &toSort);
  const std::vector<unsigned int> &getToSort() const;

private:
  std::vector<unsigned int> _toSort;
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &PmergeMe);