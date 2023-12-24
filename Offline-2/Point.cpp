// Point class cpp
#include "Point.hpp"
#include <cmath>

Point::Point() : x(0), y(0), z(0), w(1) {}

Point::Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Point::Point(double x, double y, double z) : x(x), y(y), z(z), w(1) {}

Point::Point(const Point &p) : x(p.x), y(p.y), z(p.z), w(p.w) {}

Point::~Point() {}

double Point::getX() const { return x; }

double Point::getY() const { return y; }

double Point::getZ() const { return z; }

double Point::getW() const { return w; }

void Point::setX(double x) { this->x = x; }

void Point::setY(double y) { this->y = y; }

void Point::setZ(double z) { this->z = z; }

void Point::setW(double w) { this->w = w; }

Point &Point::operator=(const Point &p)
{
    x = p.x;
    y = p.y;
    z = p.z;
    w = p.w;
    return *this;
}

Point Point::operator+(const Point &p) const
{
    return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator-(const Point &p) const
{
    return Point(x - p.x, y - p.y, z - p.z);
}

Point Point::operator*(double scalar) const
{
    return Point(x * scalar, y * scalar, z * scalar);
}

Point Point::operator/(double scalar) const
{
    return Point(x / scalar, y / scalar, z / scalar);
}

bool Point::operator==(const Point &p) const
{
    return (x == p.x && y == p.y && z == p.z && w == p.w);
}

bool Point::operator!=(const Point &p) const
{
    return (x != p.x || y != p.y || z != p.z || w != p.w);
}

double Point::operator*(const Point &p) const
{
    return (x * p.x + y * p.y + z * p.z );
}

Point Point::operator^(const Point &p) const
{
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

double Point::getMagnitude() const
{
    return sqrt(x * x + y * y + z * z);
}