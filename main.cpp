#include "PmergeMe.hpp"
#include <exception>

int main(int argc, char** argv) {
  try {
    // Parse command line arguments
    std::vector< unsigned int > numbers = PmergeMe::parseArguments(argc, argv);

    // Remove duplicates
    std::vector< unsigned int > uniqueNumbers =
        PmergeMe::removeDuplicates(numbers);

    // Check if we have any numbers left after removing duplicates
    if (uniqueNumbers.empty()) {
      std::cerr << "Error: No valid numbers to sort" << std::endl;
      return 1;
    }

    // Check if we have only one number
    if (uniqueNumbers.size() == 1) {
      std::cout << "Before: " << uniqueNumbers[0] << std::endl;
      std::cout << "After:  " << uniqueNumbers[0] << std::endl;
      return 0;
    }

    // Create PmergeMe instance and sort
    PmergeMe sorter(uniqueNumbers);
    sorter.sort();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << std::endl;
    PmergeMe::printUsage();
    return 1;
  }

  return 0;
}