#include<iostream>
#include<vector>
#include<cmath>
#include <bits/stdc++.h>

using namespace std;

struct Point
{
	double x = 0, y = 0, z = 0;
	// constructor
	Point(double x = 0, double y = 0, double z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// copy constructor
	Point(const Point &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}
	// assignment operator
	Point &operator=(const Point &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		return *this;
	}
	// destructor
	~Point() {}
	// addition operator
	Point operator+(const Point &p)
	{
		return Point(this->x + p.x, this->y + p.y, this->z + p.z);
	}
	// subtraction operator
	Point operator-(const Point &p)
	{
		return Point(this->x - p.x, this->y - p.y, this->z - p.z);
	}
	// scalar multiplication operator
	Point operator*(const double &d)
	{
		return Point(this->x * d, this->y * d, this->z * d);
	}
	// division operator
	Point operator/(const double &d)
	{
		return Point(this->x / d, this->y / d, this->z / d);
	}
	// dot product
	double operator*(const Point &p)
	{
		return this->x * p.x + this->y * p.y + this->z * p.z;
	}
	// cross product
	Point operator^(const Point &p)
	{
		return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
	}
};



vector<Point> generatePointsofSphere(double radius, int sectorCount, int stackCount)
{

	// clear memory of prev arrays
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	vector<Point> points;
	double PI = acos(-1.0);
	float x, y, z, xy;							 // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
	float s, t;									 // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);		 // r * cos(u)
		z = radius * sinf(stackAngle);		 // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep; // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			points.push_back(Point(x, y, z));

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	return points;
}

int main()
{
    vector<Point> points = generatePointsofSphere(1/sqrt(2.0), 3, 2);
    for (int i = 0; i < points.size(); i++)
    {
        cout << points[i].x << " " << points[i].y << " " << points[i].z << endl;
    }
    return 0;
}