#pragma once
#include<vector>
#include"Point.hpp"
class Transform
{
private:
    std::vector<std::vector<double>> matrix;
public:
    Transform();
    Transform(std::vector<std::vector<double>> matrix);
    Transform operator*(Transform &other);
    //overload assignment operator
    Transform &operator=(const Transform &other);
    std::vector<double> operator*(std::vector<double> &other);
    std::vector<std::vector<double>> getMatrix();
    void setMatrix(std::vector<std::vector<double>> matrix);
    void print();
    // translate matrix by a,b,c
    void translate(double a, double b, double c);
    // scale matrix by a,b,c
    void scale(double a, double b, double c);
    // rotate matrix by a,b,c
    void rotate(double angle, double a, double b, double c);
    //transform a point
    Point transform(Point p);
    
};
