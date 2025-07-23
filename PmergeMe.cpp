#include "PmergeMe.hpp"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <set>
#include <sstream>

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const std::vector< unsigned int > &toSort)
    : _toSort(toSort) {}

const std::vector< unsigned int > &PmergeMe::getToSort() const {
    return (_toSort);
}

std::ostream &operator<<(std::ostream &os, const PmergeMe &data) {
    const std::vector< unsigned int > &toSort = data.getToSort();
    std::vector< unsigned int >::const_iterator it;
    for (it = toSort.begin(); it != toSort.end(); it++) {
        os << *it << " ";
    }
    return os;
}

void PmergeMe::sort() const {
    std::vector< unsigned int > toSort = this->getToSort();

    displayBefore();

    clock_t start = clock();
    std::vector< unsigned int > sortedVector = fordJohnsonSort(toSort);
    clock_t end = clock();
    double timeVectorUs =
        (static_cast< double >(end - start) / CLOCKS_PER_SEC) * 1000000;

    displayAfter(sortedVector);

    std::deque< unsigned int > toSortDeque(toSort.begin(), toSort.end());
    start = clock();
    std::deque< unsigned int > sortedDeque = fordJohnsonSortDeque(toSortDeque);
    end = clock();
    double timeDequeUs =
        (static_cast< double >(end - start) / CLOCKS_PER_SEC) * 1000000;

    std::cout << "Time to process a range of " << toSort.size()
              << " elements with std::vector : " << timeVectorUs << " us"
              << std::endl;
    std::cout << "Time to process a range of " << toSort.size()
              << " elements with std::deque  : " << timeDequeUs << " us"
              << std::endl;
}

void PmergeMe::displayBefore() const {
    const std::vector< unsigned int > &toSort = this->getToSort();
    std::cout << "Before: ";

    if (toSort.size() <= 6) {
        for (size_t i = 0; i < toSort.size(); i++) {
            std::cout << toSort[i] << " ";
        }
    } else {
        for (size_t i = 0; i < 3; i++) {
            std::cout << toSort[i] << " ";
        }

        std::cout << "[...] ";

        for (size_t i = toSort.size() - 3; i < toSort.size(); i++) {
            std::cout << toSort[i] << " ";
        }
    }

    std::cout << std::endl;
}

void PmergeMe::displayAfter(const std::vector< unsigned int > &sorted) const {
    std::cout << "After:  ";

    if (sorted.size() <= 6) {
        for (size_t i = 0; i < sorted.size(); i++) {
            std::cout << sorted[i] << " ";
        }
    } else {
        for (size_t i = 0; i < 3; i++) {
            std::cout << sorted[i] << " ";
        }

        std::cout << "[...] ";

        for (size_t i = sorted.size() - 3; i < sorted.size(); i++) {
            std::cout << sorted[i] << " ";
        }
    }

    std::cout << std::endl;
}

std::vector< unsigned int > PmergeMe::parseArguments(int argc, char **argv) {
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

        unsigned long num = std::strtoul(arg.c_str(), NULL, 10);

        if (num > UINT_MAX) {
            std::stringstream ss;
            ss << "Error: '" << arg << "' is too large (exceeds UINT_MAX)";
            throw std::invalid_argument(ss.str());
        }

        numbers.push_back(static_cast< unsigned int >(num));
    }

    return numbers;
}

bool PmergeMe::isValidPositiveInteger(const std::string &str) {
    if (str.empty()) {
        return false;
    }

    if (str.length() > 1 && str[0] == '0') {
        return false;
    }

    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

std::vector< unsigned int >
PmergeMe::removeDuplicates(const std::vector< unsigned int > &input) {
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
    if (arr.size() <= 1) {
        return arr;
    }

    // No need to perform algo for only 2 values
    if (arr.size() == 2) {
        if (arr[0] > arr[1]) {
            std::swap(arr[0], arr[1]);
        }
        return arr;
    }

    unsigned int unpaired = 0;
    bool hasUnpaired = false;
    // Batch toSort vector into a vector of sorted pairs [[9,8],[7,6],[5,4]]
    std::vector< std::pair< unsigned int, unsigned int > > pairs =
        createPairs(arr, unpaired, hasUnpaired);

    std::vector< size_t > sortedIndices = sortLargerElements(pairs);

    std::vector< unsigned int > mainSequence =
        buildMainSequence(pairs, sortedIndices);

    insertRemainingElements(mainSequence, pairs, sortedIndices);

    // Insert the last value in the toSort.size() odd case
    if (hasUnpaired) {
        insertIntoSorted(mainSequence, unpaired);
    }

    return mainSequence;
}

std::vector< std::pair< unsigned int, unsigned int > >
PmergeMe::createPairs(const std::vector< unsigned int > &arr,
                      unsigned int &unpaired, bool &hasUnpaired) const {
    std::vector< std::pair< unsigned int, unsigned int > > pairs;

    for (size_t i = 0; i < arr.size() - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
        }
    }

    if (arr.size() % 2 == 1) {
        unpaired = arr[arr.size() - 1];
        hasUnpaired = true;
    } else {
        hasUnpaired = false;
    }

    return pairs;
}

std::vector< size_t > PmergeMe::sortLargerElements(
    const std::vector< std::pair< unsigned int, unsigned int > > &pairs) const {
    std::vector< unsigned int > larger;
    // Fill a vector with largest value of each pair, .first() can be use since
    // pair are sorted while created
    for (size_t i = 0; i < pairs.size(); i++) {
        larger.push_back(pairs[i].first);
    }

    // toSort sub-group larger is recursively sorted until small enough to cut -
    // recursion (/2 between each recursion)
    std::vector< unsigned int > sortedLarger = fordJohnsonSort(larger);

    std::vector< size_t > sortedIndices;
    std::vector< bool > used(pairs.size(), false);

    // Build a indices vector to link sortedLarger element to their origin pair.
    // This is important to later insert smaller element from the pair at the -
    // right place.
    for (size_t i = 0; i < sortedLarger.size(); i++) {
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
    const std::vector< std::pair< unsigned int, unsigned int > > &pairs,
    const std::vector< size_t > &sortedIndices) const {
    std::vector< unsigned int > mainSequence;

    if (!sortedIndices.empty()) {
        mainSequence.push_back(pairs[sortedIndices[0]].second);
    }

    // Build main sequence by adding sorted larger number using the indices
    // vector
    for (size_t i = 0; i < sortedIndices.size(); i++) {
        mainSequence.push_back(pairs[sortedIndices[i]].first);
    }

    return mainSequence;
}

void PmergeMe::insertRemainingElements(
    std::vector< unsigned int > &mainSequence,
    const std::vector< std::pair< unsigned int, unsigned int > > &pairs,
    const std::vector< size_t > &sortedIndices) const {
    if (sortedIndices.size() <= 1) {
        return;
    }

    std::vector< bool > inserted(sortedIndices.size(), false);
    inserted[0] = true;

    // Create a Jacobsthal sequence for the curent toSort sub-group size
    std::vector< unsigned int > jacobsthalSeq =
        generateJacobsthalSequence(sortedIndices.size() - 1);

    // Insert smaller pair value according to the Jacobsthal sequence
    for (size_t i = 0; i < jacobsthalSeq.size(); i++) {
        unsigned int idx = jacobsthalSeq[i];
        if (idx < sortedIndices.size() && idx > 0 && !inserted[idx]) {
            insertIntoSorted(mainSequence, pairs[sortedIndices[idx]].second);
            inserted[idx] = true;
        }
    }

    // Insert remaining smaller pair value
    for (size_t i = 1; i < sortedIndices.size(); i++) {
        if (!inserted[i]) {
            insertIntoSorted(mainSequence, pairs[sortedIndices[i]].second);
        }
    }
}

void PmergeMe::insertIntoSorted(std::vector< unsigned int > &sorted,
                                unsigned int value) const {
    size_t left = 0;
    size_t right = sorted.size();

    // Find the best side where the value should be inserted so less useless
    // comparision in the insert algorithm
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // Sort insert algorithm
    sorted.insert(sorted.begin() + left, value);
}

// Return the Jacobsthal value for a specific index
unsigned int PmergeMe::jacobsthalSequence(unsigned int n) const {
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

// Create a Jacobsthal sequence vector with only usable indexes in the correct
// order
std::vector< unsigned int >
PmergeMe::generateJacobsthalSequence(unsigned int maxIndex) const {
    std::vector< unsigned int > sequence;

    // Sstarting at 2 because their is nothing to sort in the size = 1 or 2 case
    unsigned int jacobsthalIndex = 2;
    unsigned int jacobsthalValue = jacobsthalSequence(jacobsthalIndex);

    // Max index is toSort current sub-group .size() - 1
    while (jacobsthalValue <= maxIndex) {
        unsigned int prevJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);

        for (unsigned int i = jacobsthalValue;
             i > prevJacobsthal && i <= maxIndex; i--) {
            sequence.push_back(i);
        }

        jacobsthalIndex++;
        jacobsthalValue = jacobsthalSequence(jacobsthalIndex);
    }

    unsigned int lastJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);
    for (unsigned int i = maxIndex; i > lastJacobsthal; i--) {
        sequence.push_back(i);
    }

    return sequence;
}

std::deque< unsigned int >
PmergeMe::fordJohnsonSortDeque(std::deque< unsigned int > arr) const {
    if (arr.size() <= 1) {
        return arr;
    }

    if (arr.size() == 2) {
        if (arr[0] > arr[1]) {
            std::swap(arr[0], arr[1]);
        }
        return arr;
    }

    unsigned int unpaired = 0;
    bool hasUnpaired = false;
    std::deque< std::pair< unsigned int, unsigned int > > pairs =
        createPairsDeque(arr, unpaired, hasUnpaired);

    std::deque< size_t > sortedIndices = sortLargerElementsDeque(pairs);

    std::deque< unsigned int > mainSequence =
        buildMainSequenceDeque(pairs, sortedIndices);

    insertRemainingElementsDeque(mainSequence, pairs, sortedIndices);

    if (hasUnpaired) {
        insertIntoSortedDeque(mainSequence, unpaired);
    }

    return mainSequence;
}

std::deque< std::pair< unsigned int, unsigned int > >
PmergeMe::createPairsDeque(const std::deque< unsigned int > &arr,
                           unsigned int &unpaired, bool &hasUnpaired) const {
    std::deque< std::pair< unsigned int, unsigned int > > pairs;

    for (size_t i = 0; i < arr.size() - 1; i += 2) {
        if (arr[i] > arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
        }
    }

    if (arr.size() % 2 == 1) {
        unpaired = arr[arr.size() - 1];
        hasUnpaired = true;
    } else {
        hasUnpaired = false;
    }

    return pairs;
}

std::deque< size_t > PmergeMe::sortLargerElementsDeque(
    const std::deque< std::pair< unsigned int, unsigned int > > &pairs) const {
    std::deque< unsigned int > larger;
    for (size_t i = 0; i < pairs.size(); i++) {
        larger.push_back(pairs[i].first);
    }

    std::deque< unsigned int > sortedLarger = fordJohnsonSortDeque(larger);

    std::deque< size_t > sortedIndices;
    std::vector< bool > used(pairs.size(), false);

    for (size_t i = 0; i < sortedLarger.size(); i++) {
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

std::deque< unsigned int > PmergeMe::buildMainSequenceDeque(
    const std::deque< std::pair< unsigned int, unsigned int > > &pairs,
    const std::deque< size_t > &sortedIndices) const {
    std::deque< unsigned int > mainSequence;

    if (!sortedIndices.empty()) {
        mainSequence.push_back(pairs[sortedIndices[0]].second);
    }

    for (size_t i = 0; i < sortedIndices.size(); i++) {
        mainSequence.push_back(pairs[sortedIndices[i]].first);
    }

    return mainSequence;
}

void PmergeMe::insertRemainingElementsDeque(
    std::deque< unsigned int > &mainSequence,
    const std::deque< std::pair< unsigned int, unsigned int > > &pairs,
    const std::deque< size_t > &sortedIndices) const {
    if (sortedIndices.size() <= 1) {
        return;
    }

    std::vector< bool > inserted(sortedIndices.size(), false);
    inserted[0] = true;

    std::deque< unsigned int > jacobsthalSeq =
        generateJacobsthalSequenceDeque(sortedIndices.size() - 1);

    for (size_t i = 0; i < jacobsthalSeq.size(); i++) {
        unsigned int idx = jacobsthalSeq[i];
        if (idx < sortedIndices.size() && idx > 0 && !inserted[idx]) {
            insertIntoSortedDeque(mainSequence,
                                  pairs[sortedIndices[idx]].second);
            inserted[idx] = true;
        }
    }

    for (size_t i = 1; i < sortedIndices.size(); i++) {
        if (!inserted[i]) {
            insertIntoSortedDeque(mainSequence, pairs[sortedIndices[i]].second);
        }
    }
}

void PmergeMe::insertIntoSortedDeque(std::deque< unsigned int > &sorted,
                                     unsigned int value) const {
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

std::deque< unsigned int >
PmergeMe::generateJacobsthalSequenceDeque(unsigned int maxIndex) const {
    std::deque< unsigned int > sequence;

    unsigned int jacobsthalIndex = 2;
    unsigned int jacobsthalValue = jacobsthalSequence(jacobsthalIndex);

    while (jacobsthalValue <= maxIndex) {
        unsigned int prevJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);

        for (unsigned int i = jacobsthalValue;
             i > prevJacobsthal && i <= maxIndex; i--) {
            sequence.push_back(i);
        }

        jacobsthalIndex++;
        jacobsthalValue = jacobsthalSequence(jacobsthalIndex);
    }

    unsigned int lastJacobsthal = jacobsthalSequence(jacobsthalIndex - 1);
    for (unsigned int i = maxIndex; i > lastJacobsthal; i--) {
        sequence.push_back(i);
    }

    return sequence;
}