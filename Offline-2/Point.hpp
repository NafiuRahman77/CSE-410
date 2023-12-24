
#pragma once

class Point
{
private:
    double x, y, z, w;

public:
    Point();
    Point(double x, double y, double z, double w);
    Point(double x, double y, double z);
    Point(const Point &p);
    ~Point();
    double getX() const;
    double getY() const;
    double getZ() const;
    double getW() const;
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setW(double w);

    Point &operator=(const Point &p);
    Point operator+(const Point &p) const;
    Point operator-(const Point &p) const;
    Point operator*(double scalar) const;
    Point operator/(double scalar) const;
    //dot product
    double operator*(const Point &p) const;
    //cross product
    Point operator^(const Point &p) const;
    bool operator==(const Point &p) const;
    bool operator!=(const Point &p) const;

    double getMagnitude() const;
    
}; // end class Point

