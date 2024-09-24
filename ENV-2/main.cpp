#include <iostream>
#include <vector>
#include "ComplexNumber.h"
#include "SortComplex.h"

int main() {
    std::vector<ComplexNumber> numbers = {
        ComplexNumber(3, 4),
        ComplexNumber(1, 1),
        ComplexNumber(0, 2),
        ComplexNumber(5, 5)
    };
    
    std::cout << "Before sorting:" << std::endl;
    for (const auto& num : numbers) {
        num.print();
    }

    sortComplexNumbers(numbers);

    std::cout << "\nAfter sorting by absolute value:" << std::endl;
    for (const auto& num : numbers) {
        num.print();
    }

    return 0;
}
