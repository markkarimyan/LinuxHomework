#include "ComplexNumber.h"

ComplexNumber::ComplexNumber(double real, double imag) 
    : real_part(real), imag_part(imag) {}

double ComplexNumber::real() const {
    return real_part;
}

double ComplexNumber::imag() const {
    return imag_part;
}

double ComplexNumber::abs() const {
    return std::sqrt(real_part * real_part + imag_part * imag_part);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(real_part + other.real_part, imag_part + other.imag_part);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(real_part - other.real_part, imag_part - other.imag_part);
}

ComplexNumber ComplexNumber::operator*(double scalar) const {
    return ComplexNumber(real_part * scalar, imag_part * scalar);
}

void ComplexNumber::print() const {
    std::cout << "(" << real_part << " + " << imag_part << "i)" << std::endl;
}
