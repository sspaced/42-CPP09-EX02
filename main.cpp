#include "PmergeMe.hpp"
#include <exception>

int main(int argc, char **argv) {
    try {
        std::vector< unsigned int > numbers =
            PmergeMe::parseArguments(argc, argv);

        std::vector< unsigned int > uniqueNumbers =
            PmergeMe::removeDuplicates(numbers);

        if (uniqueNumbers.empty()) {
            std::cerr << "Error: No valid numbers to sort" << std::endl;
            return 1;
        }

        if (uniqueNumbers.size() == 1) {
            std::cout << "Before: " << uniqueNumbers[0] << std::endl;
            std::cout << "After:  " << uniqueNumbers[0] << std::endl;
            return 0;
        }

        PmergeMe sorter(uniqueNumbers);
        sorter.sort();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << std::endl;
        PmergeMe::printUsage();
        return 1;
    }

    return 0;
}