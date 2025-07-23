#pragma once

#include <deque>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

class PmergeMe {
public:
  // Constructor
  ~PmergeMe();
  PmergeMe(const std::vector< unsigned int > &toSort);

  // Getter
  const std::vector< unsigned int > &getToSort() const;

  // Entry function
  void sort() const;

  // Utils function
  void displayBefore() const;
  void displayAfter(const std::vector< unsigned int > &sorted) const;

  // Input handler
  static std::vector< unsigned int > parseArguments(int argc, char **argv);
  static bool isValidPositiveInteger(const std::string &str);
  static std::vector< unsigned int >
  removeDuplicates(const std::vector< unsigned int > &input);
  static void printUsage();

private:
  std::vector< unsigned int > _toSort;

  std::vector< std::pair< unsigned int, unsigned int > >
  createPairs(const std::vector< unsigned int > &arr, unsigned int &unpaired,
              bool &hasUnpaired) const;

  std::vector< unsigned int >
  fordJohnsonSort(std::vector< unsigned int > arr) const;

  std::vector< size_t > sortLargerElements(
      const std::vector< std::pair< unsigned int, unsigned int > > &pairs)
      const;

  std::vector< unsigned int > buildMainSequence(
      const std::vector< std::pair< unsigned int, unsigned int > > &pairs,
      const std::vector< size_t > &sortedIndices) const;

  void insertRemainingElements(
      std::vector< unsigned int > &mainSequence,
      const std::vector< std::pair< unsigned int, unsigned int > > &pairs,
      const std::vector< size_t > &sortedIndices) const;

  void insertIntoSorted(std::vector< unsigned int > &sorted,
                        unsigned int value) const;

  std::deque< unsigned int >
  fordJohnsonSortDeque(std::deque< unsigned int > arr) const;

  std::deque< std::pair< unsigned int, unsigned int > >
  createPairsDeque(const std::deque< unsigned int > &arr,
                   unsigned int &unpaired, bool &hasUnpaired) const;

  std::deque< size_t > sortLargerElementsDeque(
      const std::deque< std::pair< unsigned int, unsigned int > > &pairs) const;

  std::deque< unsigned int > buildMainSequenceDeque(
      const std::deque< std::pair< unsigned int, unsigned int > > &pairs,
      const std::deque< size_t > &sortedIndices) const;

  void insertRemainingElementsDeque(
      std::deque< unsigned int > &mainSequence,
      const std::deque< std::pair< unsigned int, unsigned int > > &pairs,
      const std::deque< size_t > &sortedIndices) const;

  void insertIntoSortedDeque(std::deque< unsigned int > &sorted,
                             unsigned int value) const;
};

std::ostream &operator<<(std::ostream &os, const PmergeMe &data);

unsigned int jacobsthalSequence(unsigned int n);

std::vector< unsigned int > enerateJacobsthalSequence(unsigned int maxIndex);

std::deque< unsigned int >
generateJacobsthalSequenceDeque(unsigned int maxIndex);