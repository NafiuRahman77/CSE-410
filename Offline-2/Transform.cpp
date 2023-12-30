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
    // Initialize matrix as identity matrix
    matrix = {{1, 0, 0, 0},
              {0, 1, 0, 0},
              {0, 0, 1, 0},
              {0, 0, 0, 1}};

    const double theta = angle * M_PI / 180.0;
    const double c = cos(theta), s = sin(theta);

    // Set precision for c and s
    const double epsilon = 1e-15;
    const auto zero_if_near_zero = [epsilon](double val) { return (fabs(val) < epsilon) ? 0.0 : val; };

    const double t = 1 - c;

    Point p = Point(a, b, cc)/sqrt(a*a+b*b+cc*cc);
    const double ax = a/sqrt(a*a+b*b+cc*cc), ay = b/sqrt(a*a+b*b+cc*cc), az = cc/sqrt(a*a+b*b+cc*cc);

    matrix[0][0] = zero_if_near_zero(t * ax * ax + c);
    matrix[0][1] = zero_if_near_zero(t * ax * ay - s * az);
    matrix[0][2] = zero_if_near_zero(t * ax * az + s * ay);

    matrix[1][0] = zero_if_near_zero(t * ax * ay + s * az);
    matrix[1][1] = zero_if_near_zero(t * ay * ay + c);
    matrix[1][2] = zero_if_near_zero(t * ay * az - s * ax);

    matrix[2][0] = zero_if_near_zero(t * ax * az - s * ay);
    matrix[2][1] = zero_if_near_zero(t * ay * az + s * ax);
    matrix[2][2] = zero_if_near_zero(t * az * az + c);
}


Point Transform::transform(Point p)
{
    std::vector<double> temp = {p.getX(), p.getY(), p.getZ(), p.getW()};
    std::vector<double> result = (*this) * temp;
    return Point(result[0], result[1], result[2]) / result[3];
}