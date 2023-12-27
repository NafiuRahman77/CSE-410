#include"Transform.hpp"
#include<iostream>
#include<cmath>

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
   //set matrix to identity matrix
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
    //set matrix to identity matrix
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

void Transform::rotate(double a, double b, double c, double angle)
{
    //set matrix to identity matrix
    matrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
    matrix[0][0] = 1;
    matrix[1][1] = 1;
    matrix[2][2] = 1;
    matrix[3][3] = 1;
    
    double rad = angle * 3.14159265358979323846 / 180;
    double cosA = cos(rad);
    double sinA = sin(rad);

    //normalize a,b,c
    double length = sqrt(a * a + b * b + c * c);
    a /= length;
    b /= length;
    c /= length;
    
    matrix[0][0] = cosA + (1 - cosA) * a * a;
    matrix[0][1] = (1 - cosA) * a * b - sinA * c;
    matrix[0][2] = (1 - cosA) * a * c + sinA * b;
    matrix[1][0] = (1 - cosA) * b * a + sinA * c;
    matrix[1][1] = cosA + (1 - cosA) * b * b;
    matrix[1][2] = (1 - cosA) * b * c - sinA * a;
    matrix[2][0] = (1 - cosA) * c * a - sinA * b;
    matrix[2][1] = (1 - cosA) * c * b + sinA * a;
    matrix[2][2] = cosA + (1 - cosA) * c * c;
    matrix[3][3] = 1;
}

Point Transform::transform(Point p)
{
    std::vector<double> temp = {p.getX(), p.getY(), p.getZ(), p.getW()};
    std::vector<double> result = (*this) * temp;
    return Point(result[0], result[1], result[2], result[3]);
}