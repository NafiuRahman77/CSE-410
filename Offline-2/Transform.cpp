#include "Transform.hpp"
#include <iostream>
#include <cmath>

Transform::Transform()
{
    matrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[3][3] = 1;
}

Transform::Transform(std::vector<std::vector<double>> matrix)
{
    this->matrix = matrix;
}

Transform Transform::operator*(Transform &other)
{
    std::vector<std::vector<double>> result(4, std::vector<double>(4, 0));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < other.matrix[0].size(); j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    return Transform(result);
}

Transform &Transform::operator=(const Transform &other)
{
    matrix = other.matrix;
    return *this;
}

std::vector<double> Transform::operator*(std::vector<double> &other)
{
    std::vector<double> result(4, 0);
    for (int i = 0; i < 4; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            result[i] += matrix[i][k] * other[k];
        }
    }
    return result;
}

std::vector<std::vector<double>> Transform::getMatrix()
{
    return matrix;
}

void Transform::setMatrix(std::vector<std::vector<double>> matrix)
{
    this->matrix = matrix;
}

void Transform::print()
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Transform::translate(double a, double b, double c)
{
    // set matrix to identity matrix
    matrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[3][3] = 1;

    matrix[0][3] = a;
    matrix[1][3] = b;
    matrix[2][3] = c;
}

void Transform::scale(double a, double b, double c)
{
    // set matrix to identity matrix
    matrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[3][3] = 1;

    matrix[0][0] = a;
    matrix[1][1] = b;
    matrix[2][2] = c;
    matrix[3][3] = 1;
}

void Transform::rotate(double angle, double a, double b, double cc)
{
    // set matrix to identity matrix
    matrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[3][3] = 1;

    double theta = angle * acos(-1.0) / 180.0;
    double c = 0;
    c = cos(theta);
    double s = sin(theta);
    // set precision of c and s
    if (c < 0.0000000000000001 && c > -0.0000000000000001)
        c = 0;
    if (s < 0.0000000000000001 && s > -0.0000000000000001)
        s = 0;
    double t = 1 - c;

    Point p = Point(a, b, cc);
    p = p / p.getMagnitude();
    double ax = p.getX();
    double ay = p.getY();
    double az = p.getZ();

    matrix[0][0] = t * ax * ax + c;

    matrix[0][1] = t * ax * ay - s * az;
    matrix[0][2] = t * ax * az + s * ay;

    matrix[1][0] = t * ax * ay + s * az;
    matrix[1][1] = t * ay * ay + c;
    matrix[1][2] = t * ay * az - s * ax;

    matrix[2][0] = t * ax * az - s * ay;
    matrix[2][1] = t * ay * az + s * ax;
    matrix[2][2] = t * az * az + c;

    matrix[3][3] = 1;
}

Point Transform::transform(Point p)
{
    std::vector<double> temp = {p.getX(), p.getY(), p.getZ(), p.getW()};
    std::vector<double> result = (*this) * temp;
    return Point(result[0], result[1], result[2]) / result[3];
}