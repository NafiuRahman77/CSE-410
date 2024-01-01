//Triangle hpp class
#pragma once
#include <bits/stdc++.h>
#include <vector>
#include "Point.hpp"

static unsigned long int g_seed = 1;  
inline int getRandomInt() 
{ 
g_seed = (214013 * g_seed + 2531011);  
return (g_seed >> 16) & 0x7FFF; 
} 

class Triangle
{
private:
    Point a, b, c;
    std::vector<double> color;
    
public:
    Triangle();
    Triangle(const Point &a, const Point &b, const Point &c);
    Triangle(const Triangle &t);
    ~Triangle();
    //set color
    void setColor();
    std::vector<double> getColor() const;
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
