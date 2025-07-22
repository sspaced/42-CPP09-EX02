#include "PmergeMe.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <set>
#include <sstream>

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

  displayBefore();

  // Measure sorting time
  clock_t start = clock();
  std::vector< unsigned int > sorted = fordJohnsonSort(toSort);
  clock_t end = clock();

  displayAfter(sorted);

  // Calculate and display time in microseconds
  double timeMs =
      (static_cast< double >(end - start) / CLOCKS_PER_SEC) * 1000000;
  std::cout << "Time to process a range of " << toSort.size()
            << " elements with std::vector : " << timeMs << " us" << std::endl;
}

void PmergeMe::displayBefore() const {
  const std::vector< unsigned int >& toSort = this->getToSort();
  std::cout << "Before: ";

  if (toSort.size() <= 6) {
    // Display all elements if 6 or fewer
    for (size_t i = 0; i < toSort.size(); i++) {
      std::cout << toSort[i] << " ";
    }
  } else {
    // Display first 3 elements
    for (size_t i = 0; i < 3; i++) {
      std::cout << toSort[i] << " ";
    }

    std::cout << "[...] ";

    // Display last 3 elements
    for (size_t i = toSort.size() - 3; i < toSort.size(); i++) {
      std::cout << toSort[i] << " ";
    }
  }

  std::cout << std::endl;
}

void PmergeMe::displayAfter(const std::vector< unsigned int >& sorted) const {
  std::cout << "After:  ";

  if (sorted.size() <= 6) {
    // Display all elements if 6 or fewer
    for (size_t i = 0; i < sorted.size(); i++) {
      std::cout << sorted[i] << " ";
    }
  } else {
    // Display first 3 elements
    for (size_t i = 0; i < 3; i++) {
      std::cout << sorted[i] << " ";
    }

    std::cout << "[...] ";

    // Display last 3 elements
    for (size_t i = sorted.size() - 3; i < sorted.size(); i++) {
      std::cout << sorted[i] << " ";
    }
  }

  std::cout << std::endl;
}

std::vector< unsigned int > PmergeMe::parseArguments(int argc, char** argv) {
  std::vector< unsigned int > numbers;

  if (argc < 2) {
    throw std::invalid_argument("Error: No arguments provided");
  }

  for (int i = 1; i < argc; i++) {
    std::string arg(argv[i]);

    if (!isValidPositiveInteger(arg)) {
      std::stringstream ss;
      ss << "Error: '" << arg << "' is not a valid positive integer";
      throw std::invalid_argument(ss.str());
    }

    // Convert to unsigned int
    unsigned long num = std::strtoul(arg.c_str(), NULL, 10);

    // Check for overflow
    if (num > UINT_MAX) {
      std::stringstream ss;
      ss << "Error: '" << arg << "' is too large (exceeds UINT_MAX)";
      throw std::invalid_argument(ss.str());
    }

    numbers.push_back(static_cast< unsigned int >(num));
  }

  return numbers;
}

bool PmergeMe::isValidPositiveInteger(const std::string& str) {
  if (str.empty()) {
    return false;
  }

  // Check for leading zeros (except for "0" itself)
  if (str.length() > 1 && str[0] == '0') {
    return false;
  }

  // Check each character is a digit
  for (size_t i = 0; i < str.length(); i++) {
    if (!std::isdigit(str[i])) {
      return false;
    }
  }

  return true;
}

std::vector< unsigned int >
PmergeMe::removeDuplicates(const std::vector< unsigned int >& input) {
  std::set< unsigned int > uniqueSet;
  std::vector< unsigned int > result;
  size_t originalSize = input.size();

  for (size_t i = 0; i < input.size(); i++) {
    if (uniqueSet.find(input[i]) == uniqueSet.end()) {
      uniqueSet.insert(input[i]);
      result.push_back(input[i]);
    }
  }

  if (result.size() != originalSize) {
    std::cout << "Warning: " << (originalSize - result.size())
              << " duplicate(s) removed from input" << std::endl;
  }

  return result;
}

void PmergeMe::printUsage() {
  std::cout << "Usage: ./PmergeMe [positive integers...]" << std::endl;
  std::cout << "Example: ./PmergeMe 3 5 9 7 4" << std::endl;
  std::cout << "Note: Duplicates will be automatically removed" << std::endl;
}

std::vector< unsigned int >
PmergeMe::fordJohnsonSort(std::vector< unsigned int > arr) const {
  // Base cases
  if (arr.size() <= 1) {
    return arr;
  }

  if (arr.size() == 2) {
    if (arr[0] > arr[1]) {
      std::swap(arr[0], arr[1]);
    }
    return arr;
  }

  // Step 1: Create pairs and handle unpaired element
  unsigned int unpaired = 0;
  bool hasUnpaired = false;
  std::vector< std::pair< unsigned int, unsigned int > > pairs =
      createPairs(arr, unpaired, hasUnpaired);

  // Step 2: Sort larger elements and get sorted indices
  std::vector< size_t > sortedIndices = sortLargerElements(pairs);

  // Step 3: Build main sequence with first smaller element and all larger
  // elements
  std::vector< unsigned int > mainSequence =
      buildMainSequence(pairs, sortedIndices);

  // Step 4: Insert remaining smaller elements using Jacobsthal sequence
  insertRemainingElements(mainSequence, pairs, sortedIndices);

  // Step 5: Insert unpaired element if exists
  if (hasUnpaired) {
    insertIntoSorted(mainSequence, unpaired);
  }

  return mainSequence;
}

std::vector< std::pair< unsigned int, unsigned int > >
PmergeMe::createPairs(const std::vector< unsigned int >& arr,
                      unsigned int& unpaired, bool& hasUnpaired) const {
  std::vector< std::pair< unsigned int, unsigned int > > pairs;

  // Create pairs (larger, smaller)
  for (size_t i = 0; i < arr.size() - 1; i += 2) {
    if (arr[i] > arr[i + 1]) {
      pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
    } else {
      pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
  }

  // Handle unpaired element
  if (arr.size() % 2 == 1) {
    unpaired = arr[arr.size() - 1];
    hasUnpaired = true;
  } else {
    hasUnpaired = false;
  }

  return pairs;
}

std::vector< size_t > PmergeMe::sortLargerElements(
    const std::vector< std::pair< unsigned int, unsigned int > >& pairs) const {
  // Extract larger elements
  std::vector< unsigned int > larger;
  for (size_t i = 0; i < pairs.size(); i++) {
    larger.push_back(pairs[i].first);
  }

  // Sort larger elements recursively
  std::vector< unsigned int > sortedLarger = fordJohnsonSort(larger);

  // Create mapping from sorted larger values to their pair indices
  std::vector< size_t > sortedIndices;
  std::vector< bool > used(pairs.size(), false);

  for (size_t i = 0; i < sortedLarger.size(); i++) {
    // Find first unused pair with this larger value
    for (size_t j = 0; j < pairs.size(); j++) {
      if (!used[j] && pairs[j].first == sortedLarger[i]) {
        sortedIndices.push_back(j);
        used[j] = true;
        break;
      }
    }
  }

  return sortedIndices;
}

std::vector< unsigned int > PmergeMe::buildMainSequence(
    const std::vector< std::pair< unsigned int, unsigned int > >& pairs,
    const std::vector< size_t >& sortedIndices) const {
  std::vector< unsigned int > mainSequence;

  // Add first smaller element (if pairs exist)
  if (!sortedIndices.empty()) {
    mainSequence.push_back(pairs[sortedIndices[0]].second);
  }

  // Add all sorted larger elements
  for (size_t i = 0; i < sortedIndices.size(); i++) {
    mainSequence.push_back(pairs[sortedIndices[i]].first);
  }

  return mainSequence;
}

void PmergeMe::insertRemainingElements(
    std::vector< unsigned int >& mainSequence,
    const std::vector< std::pair< unsigned int, unsigned int > >& pairs,
    const std::vector< size_t >& sortedIndices) const {
  if (sortedIndices.size() <= 1) {
    return; // No remaining elements to insert
  }

  std::vector< bool > inserted(sortedIndices.size(), false);
  inserted[0] = true; // First element already inserted

  // Generate Jacobsthal sequence for optimal insertion order
  std::vector< unsigned int > jacobsthalSeq =
      generateJacobsthalSequence(sortedIndices.size() - 1);

  // Insert elements according to Jacobsthal sequence
  for (size_t i = 0; i < jacobsthalSeq.size(); i++) {
    unsigned int idx = jacobsthalSeq[i];
    if (idx < sortedIndices.size() && idx > 0 && !inserted[idx]) {
      insertIntoSorted(mainSequence, pairs[sortedIndices[idx]].second);
      inserted[idx] = true;
    }
  }

  // Insert any remaining elements (fallback)
  for (size_t i = 1; i < sortedIndices.size(); i++) {
    if (!inserted[i]) {
      insertIntoSorted(mainSequence, pairs[sortedIndices[i]].second);
    }
  }
}

void PmergeMe::insertIntoSorted(std::vector< unsigned int >& sorted,
                                unsigned int value) const {
  // Binary search insertion
  size_t left = 0;
  size_t right = sorted.size();

  while (left < right) {
    size_t mid = left + (right - left) / 2;
    if (sorted[mid] < value) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  sorted.insert(sorted.begin() + left, value);
}

unsigned int jacobsthalSequence(unsigned int n) {
  if (n == 0)
    return (0);
  if (n == 1)
    return (1);

  unsigned int prev = 0;
  unsigned int current = 1;
  unsigned int buff;

  for (size_t i = 1; i < n; i++) {
    buff = current + 2 * prev;
    prev = current;
    current = buff;
  }
  return current;
}

std::vector< unsigned int > generateJacobsthalSequence(unsigned int maxIndex) {
  std::vector< unsigned int > sequence;

  unsigned int jacobsthalIndex = 2; // Start with J(2) = 3
  unsigned int jacobsthalValue = jacobsthalSequence(jacobsthalIndex);

  while (jacobsthalValue <= maxIndex) {
    // Add elements from jacobsthalValue down to previous jacobsthal + 1
    unsigned int prevJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);

    for (unsigned int i = jacobsthalValue; i > prevJacobsthal && i <= maxIndex;
         i--) {
      sequence.push_back(i);
    }

    jacobsthalIndex++;
    jacobsthalValue = jacobsthalSequence(jacobsthalIndex);
  }

  // Add remaining elements in reverse order
  unsigned int lastJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);
  for (unsigned int i = maxIndex; i > lastJacobsthal; i--) {
    sequence.push_back(i);
  }

  return sequence;
}