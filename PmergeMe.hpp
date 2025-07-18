#pragma once

#include <iostream>
#include <vector>

class PmergeMe {
public:
  ~PmergeMe();
  PmergeMe(const std::vector<unsigned int> &toSort);
  const std::vector<unsigned int> &getToSort() const;
  void sort() const;

private:
  std::vector<unsigned int> _toSort;
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &data);