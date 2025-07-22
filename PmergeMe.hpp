#pragma once

#include <iostream>
#include <string>
#include <vector>

class PmergeMe {
public:
  ~PmergeMe();
  PmergeMe(const std::vector< unsigned int >& toSort);
  const std::vector< unsigned int >& getToSort() const;
  void sort() const;
  void displayBefore() const;
  void displayAfter(const std::vector< unsigned int >& sorted) const;

  static std::vector< unsigned int > parseArguments(int argc, char** argv);
  static bool isValidPositiveInteger(const std::string& str);
  static std::vector< unsigned int >
  removeDuplicates(const std::vector< unsigned int >& input);
  static void printUsage();

private:
  std::vector< unsigned int > _toSort;

  // Main sorting function
  std::vector< unsigned int >
  fordJohnsonSort(std::vector< unsigned int > arr) const;

  // Helper functions for Ford-Johnson algorithm
  std::vector< std::pair< unsigned int, unsigned int > >
  createPairs(const std::vector< unsigned int >& arr, unsigned int& unpaired,
              bool& hasUnpaired) const;
  std::vector< size_t > sortLargerElements(
      const std::vector< std::pair< unsigned int, unsigned int > >& pairs)
      const;
  std::vector< unsigned int > buildMainSequence(
      const std::vector< std::pair< unsigned int, unsigned int > >& pairs,
      const std::vector< size_t >& sortedIndices) const;
  void insertRemainingElements(
      std::vector< unsigned int >& mainSequence,
      const std::vector< std::pair< unsigned int, unsigned int > >& pairs,
      const std::vector< size_t >& sortedIndices) const;

  // Binary insertion
  void insertIntoSorted(std::vector< unsigned int >& sorted,
                        unsigned int value) const;
};

std::ostream& operator<<(std::ostream& os, const PmergeMe& data);
unsigned int jacobsthalSequence(unsigned int n);
std::vector< unsigned int > generateJacobsthalSequence(unsigned int maxIndex);