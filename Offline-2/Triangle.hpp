//Triangle hpp class
#pragma once
#include "Point.hpp"
#include <cmath>

class Triangle
{
private:
    Point a, b, c;

public:
    Triangle();
    Triangle(const Point &a, const Point &b, const Point &c);
    Triangle(const Triangle &t);
    ~Triangle();
    Point getA() const;
    Point getB() const;
    Point getC() const;
    void setA(const Point &a);
    void setB(const Point &b);
    void setC(const Point &c);
    double getPerimeter() const;
    double getArea() const;
    bool isEquilateral() const;
    bool isIsosceles() const;
    bool isScalene() const;
    bool isRight() const;
    bool isObtuse() const;
    bool isAcute() const;
    bool isDegenerate() const;
    bool operator==(const Triangle &t) const;
    bool operator!=(const Triangle &t) const;
    Triangle &operator=(const Triangle &t);
    double getZ(double x, double y) const;
}; // end class Triangle
