
#pragma once
#include <bits/stdc++.h>

class Point
{
public:
    double x, y, z;

    // Constructors
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Point(const Point &p) : x(p.x), y(p.y), z(p.z) {}

    // Destructor
    ~Point() {}

    // Assignment operator
    Point &operator=(const Point &p)
    {
        if (this != &p)
        {
            this->x = p.x;
            this->y = p.y;
            this->z = p.z;
        }
        return *this;
    }

    // Arithmetic operators
    Point operator+(const Point &p)
    {
        return Point(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Point operator-(const Point &p)
    {
        return Point(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Point operator*(const double &d)
    {
        return Point(this->x * d, this->y * d, this->z * d);
    }

    Point operator/(const double &d)
    {
        return Point(this->x / d, this->y / d, this->z / d);
    }

    // Dot product
    double operator*(const Point &p)
    {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }

    // Cross product
    Point operator^(const Point &p)
    {
        return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
    }
};
// end class Point
