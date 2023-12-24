// Triangle cpp class
#include "Triangle.hpp"
#include <cmath>

Triangle::Triangle() : a(Point()), b(Point()), c(Point()) {}

Triangle::Triangle(const Point &a, const Point &b, const Point &c) : a(a), b(b), c(c) {}

Triangle::Triangle(const Triangle &t) : a(t.a), b(t.b), c(t.c) {}

Triangle::~Triangle() {}

Point Triangle::getA() const { return a; }

Point Triangle::getB() const { return b; }

Point Triangle::getC() const { return c; }

void Triangle::setA(const Point &a) { this->a = a; }

void Triangle::setB(const Point &b) { this->b = b; }

void Triangle::setC(const Point &c) { this->c = c; }

double Triangle::getPerimeter() const
{
    return (a - b).getMagnitude() + (b - c).getMagnitude() + (c - a).getMagnitude();
}

double Triangle::getArea() const
{
    double s = getPerimeter() / 2;
    return sqrt(s * (s - (a - b).getMagnitude()) * (s - (b - c).getMagnitude()) * (s - (c - a).getMagnitude()));
}

bool Triangle::isEquilateral() const
{
    return (a - b).getMagnitude() == (b - c).getMagnitude() && (b - c).getMagnitude() == (c - a).getMagnitude();
}

bool Triangle::isIsosceles() const
{
    return (a - b).getMagnitude() == (b - c).getMagnitude() || (b - c).getMagnitude() == (c - a).getMagnitude() || (c - a).getMagnitude() == (a - b).getMagnitude();
}

bool Triangle::isScalene() const
{
    return (a - b).getMagnitude() != (b - c).getMagnitude() && (b - c).getMagnitude() != (c - a).getMagnitude() && (c - a).getMagnitude() != (a - b).getMagnitude();
}

bool Triangle::isRight() const
{
    return (a - b) * (b - c) == 0 || (b - c) * (c - a) == 0 || (c - a) * (a - b) == 0;
}

bool Triangle::isObtuse() const
{
    return (a - b) * (b - c) < 0 || (b - c) * (c - a) < 0 || (c - a) * (a - b) < 0;
}

bool Triangle::isAcute() const
{
    return (a - b) * (b - c) > 0 && (b - c) * (c - a) > 0 && (c - a) * (a - b) > 0;
}

bool Triangle::isDegenerate() const
{
    return (a - b) * (b - c) == 0 || (b - c) * (c - a) == 0 || (c - a) * (a - b) == 0;
}

bool Triangle::operator==(const Triangle &t) const
{
    return (a == t.a && b == t.b && c == t.c) || (a == t.b && b == t.c && c == t.a) || (a == t.c && b == t.a && c == t.b);
}

bool Triangle::operator!=(const Triangle &t) const
{
    return (a != t.a || b != t.b || c != t.c) && (a != t.b || b != t.c || c != t.a) && (a != t.c || b != t.a || c != t.b);
}

Triangle &Triangle::operator=(const Triangle &t)
{
    a = t.a;
    b = t.b;
    c = t.c;
    return *this;
}
