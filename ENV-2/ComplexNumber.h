#pragma once

#include <iostream>
#include <cmath>

class ComplexNumber {
public:
    ComplexNumber(double real = 0, double imag = 0);
    double real() const;
    double imag() const;
    
    double abs() const;
    
    ComplexNumber operator+(const ComplexNumber& other) const;
    ComplexNumber operator-(const ComplexNumber& other) const;
    ComplexNumber operator*(double scalar) const;

    void print() const;

private:
    double real_part;
    double imag_part;
};

