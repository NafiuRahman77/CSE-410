
#pragma once
#include <bits/stdc++.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;
class Color
{
public:
    double r, g, b;
    Color(double r = 0, double g = 0, double b = 0)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color(const Color &c)
    {
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
    }
    ~Color() {}
    Color &operator=(const Color &c)
    {
        if (this != &c)
        {
            this->r = c.r;
            this->g = c.g;
            this->b = c.b;
        }
        return *this;
    }

    Color operator*(const double d)
    {
        return Color(this->r * d, this->g * d, this->b * d);
    }

    Color operator+(const Color &c)
    {
        return Color(this->r + c.r, this->g + c.g, this->b + c.b);
    }
};
class Vector3D
{
public:
    double x, y, z;

    // Constructors
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vector3D(const Vector3D &p) : x(p.x), y(p.y), z(p.z) {}

    // Destructor
    ~Vector3D() {}

    // Assignment operator
    Vector3D &operator=(const Vector3D &p)
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
    Vector3D operator+(const Vector3D &p)
    {
        return Vector3D(this->x + p.x, this->y + p.y, this->z + p.z);
    }

    Vector3D operator-(const Vector3D &p)
    {
        return Vector3D(this->x - p.x, this->y - p.y, this->z - p.z);
    }

    Vector3D operator*(const double &d)
    {
        return Vector3D(this->x * d, this->y * d, this->z * d);
    }

    Vector3D operator/(const double &d)
    {
        return Vector3D(this->x / d, this->y / d, this->z / d);
    }

    // Dot product
    double operator*(const Vector3D &p)
    {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }

    // Cross product
    Vector3D operator^(const Vector3D &p)
    {
        return Vector3D(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
    }
};
// end class Vector3D

class PointLight
{
public:
    Vector3D light_pos;
    Color color;

    PointLight()
    {
    }

    PointLight(Vector3D light_pos, Color color)
    {
        this->light_pos = light_pos;
        this->color = color;
    }

    void draw()
    {
        glColor3f(color.r, color.g, color.b);
        // draw a point light
        glPushMatrix();
        {
            glTranslatef(light_pos.x, light_pos.y, light_pos.z);
            glutSolidSphere(1, 100, 100);
        }
        glPopMatrix();
    }
};

class SpotLight
{
public:
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;

    SpotLight()
    {
    }

    SpotLight(PointLight point_light, Vector3D light_direction, double cutoff_angle)
    {
        this->point_light = point_light;
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }

    void draw()
    {
        point_light.draw();
    }
};

class Ray
{
public:
    Vector3D start, dir;

    Ray(Vector3D start, Vector3D dir)
    {
        this->start = start;
        this->dir = dir;
        if(dir*dir!=0)
            this->dir = dir / sqrt(dir * dir);
    }

    Vector3D getPoint(double t)
    {
        return start + (dir * t);
    }
};

class Object
{
public:
    Vector3D reference_point; // should have x, y, z
    double height, width, length;
    Color color;
    int id = -1;
    std::vector<double> coefficients; // ambient, diffuse, specular, reflection coefficients
    int shine;                        // exponent term of specular component
    Object()
    {
        color = Color(0, 0, 0);
        coefficients = {0, 0, 0, 0};
    }
    virtual void draw() = 0;
    void setColor(Color c)
    {
        // set color vector
        color = c;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setCoefficients(double ambient, double diffuse, double specular, double reflection)
    {
        coefficients = {ambient, diffuse, specular, reflection};
    }

    std::vector<double> getCoefficients()
    {
        return coefficients;
    }

    Color getColor()
    {
        return color;
    }

    virtual double intersect_2(std::vector<PointLight> pointlights, std::vector<SpotLight> spotlights, std::vector<Object *> objects, Ray r, Color &c, int level)
    {
        const double t = intersect(r, c, level);

        if (level == 0)
            return t;
        
        if(level>=4)
            return t;

        if (t <= 0)

            return -1;
        // cout << "t: " << t << endl;

        Vector3D intersectionPoint = r.start + (r.dir * t);
        Color intersectionPointColor = getColorAt(intersectionPoint);
        c = intersectionPointColor * coefficients[0]; // ambient component
        // calculate normal at intersection point
        Vector3D normal = getNormal(intersectionPoint); 
    
        normal = normal / sqrt(normal * normal);

        for (int i = 0; i < pointlights.size(); i++)
        {
            Vector3D lightDir = intersectionPoint - pointlights[i].light_pos;

            double lightDistance = sqrt(lightDir * lightDir);
            lightDir = lightDir / lightDistance;

            Ray shadowRay(pointlights[i].light_pos, lightDir);

            double dist = intersect(shadowRay, c, 0);
            
            bool inShadow = false;
            for (int j = 0; j < objects.size(); j++)
            {
                double temp = objects[j]->intersect(shadowRay, c, 0);
                if (temp > 0.00001 && temp < dist)
                {
                    inShadow = true;
                    break;
                }
            }

            if (!inShadow)
            {
                Vector3D reflectionDir = lightDir - normal * 2 * (lightDir * normal);
                double lambert = -(lightDir * normal);
                //cout<<lambert<<endl;
                if (lambert >= 0)
                {
                    c = c + (Color(intersectionPointColor.r * pointlights[i].color.r, intersectionPointColor.g * pointlights[i].color.g, intersectionPointColor.b * pointlights[i].color.b) * coefficients[1] * lambert); // diffuse component
                }

                double phong =  -(r.dir * lightDir);
                //cout<<phong<<endl;
                if (phong >= 0)
                {
                    phong = pow(phong, shine);
                    c = c + (Color(intersectionPointColor.r * pointlights[i].color.r, intersectionPointColor.g * pointlights[i].color.g, intersectionPointColor.b * pointlights[i].color.b) * coefficients[2] * phong); // specular component
                }
            }
        }

        // for (int i = 0; i < spotlights.size(); i++)
        // {
        //     Vector3D lightDir = intersectionPoint - spotlights[i].point_light.light_pos;

        //     double lightDistance = sqrt(lightDir * lightDir);
        //     lightDir = lightDir / lightDistance;

        //     double angle = acos(lightDir * spotlights[i].light_direction);
        //     if (angle <= spotlights[i].cutoff_angle)
        //     {
        //         Ray shadowRay(spotlights[i].point_light.light_pos, lightDir);
        //         // if intersectionPoint is in shadow, the diffuse
        //         // and specular components need not be calculated

        //         double dist = intersect(shadowRay, c, 0);

        //         bool inShadow = false;
        //         for (int j = 0; j < objects.size(); j++)
        //         {
        //             double temp = objects[j]->intersect(shadowRay, c, 0);
        //             if (temp > 0.00001 && temp < dist)
        //             {
        //                 inShadow = true;
        //                 break;
        //             }
        //         }
        //         if (!inShadow)
        //         {
        //             Vector3D reflectionDir = lightDir - normal * 2 * (lightDir * normal);
        //             double lambert = -(lightDir * normal);
        //             if (lambert > 0)
        //             {
        //                 c = c + (Color(intersectionPointColor.r * spotlights[i].point_light.color.r, intersectionPointColor.g * spotlights[i].point_light.color.g, intersectionPointColor.b * spotlights[i].point_light.color.b) * coefficients[1] * lambert); // diffuse component
        //             }

        //             double phong = -(r.dir * lightDir);
        //             if (phong > 0)
        //             {
        //                 phong = pow(phong, shine);
        //                 c = c + (Color(intersectionPointColor.r * spotlights[i].point_light.color.r, intersectionPointColor.g * spotlights[i].point_light.color.g, intersectionPointColor.b * spotlights[i].point_light.color.b) * coefficients[2] * phong); // specular component
        //             }
        //         }
        //     }
        // }

        // // find nearest intersection object and do recursive call

        // // construct reflected ray from intersection point
        // //  actually slightly forward from the point (by moving the
        // //  start a little bit towards the reflection direction)
        // //  to avoid self intersection

        //   Vector3D reflectionDir = r.dir - normal * 2 * (r.dir * normal);

        //     reflectionDir = reflectionDir / sqrt(reflectionDir * reflectionDir);

        //     Ray reflectedRay(intersectionPoint + reflectionDir * 0.0001, reflectionDir);
        //     double t_reflected = -1;
        //     int nearest = -1;
        //     for (int i = 0; i < objects.size(); i++)
        //     {
        //         double temp = objects[i]->intersect(reflectedRay, c, 0);

        //         if (temp > 0.00001 && (t_reflected < 0 || temp < t_reflected))
        //         {

        //             t_reflected = temp;
        //             nearest = i;
        //         }
        //     }
        //     if (nearest != -1)
        //     {
        //         // cout << "inside reflected ray" << endl;
        //         if (level == 1 && nearest == 1)
        //         {

        //             // cout<<normal.x<<" "<<normal.y<<" "<<normal.z<<endl;
        //             // cout<<r.dir.x<<" "<<r.dir.y<<" "<<r.dir.z<<endl;
        //             // cout<<"reflected"<<reflectedRay.dir.x<<" "<<reflectedRay.dir.y<<" "<<reflectedRay.dir.z<<endl;
        //             // cout<<endl;
        //         }

        //         Color reflectedColor;
        //         objects[nearest]->intersect_2(pointlights, spotlights, objects, reflectedRay, reflectedColor, level + 1);
        //         c = c + reflectedColor * coefficients[3];
        //     }
        
        return t;
    }
    virtual double intersect(Ray r, Color &c, int level)
    {
        return -1;
    }

    virtual Color getColorAt(Vector3D pt)
    {
        return color;
    }
    virtual Vector3D getNormal(Vector3D pt)
    {
        return Vector3D(0, 0, 0);
    }
};

class Sphere : public Object
{
public:
    Sphere(Vector3D C, double R)
    {
        reference_point = C;
        length = R;
        id = 0;
    }
    void draw()
    {
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
        {
            glTranslatef(reference_point.x, reference_point.y, reference_point.z);
            glColor3f(color.r, color.g, color.b);
            glutSolidSphere(length, 100, 100);
        }
        glPopMatrix();
    }

    virtual double intersect(Ray r, Color &c, int level)
    {
        double A = r.dir * r.dir;
        double B = ((r.start - reference_point) * r.dir) * 2;
        double C = (r.start - reference_point) * (r.start - reference_point) - length * length;
        double D = B * B - 4 * A * C;
        if (D < 0)
            return -1;
        if(A<0.00001)
            return -C/B;

        double t1 = (-B + sqrt(D)) / (2 * A);
        double t2 = (-B - sqrt(D)) / (2 * A);

        if (t1 < 0 && t2 < 0)
            return -1;
        
        if(t1<0.0f)
        {
            return -1;
        }
        if(t2>=0.0f)
        {
            return t2;
        }
        
    }

    virtual Vector3D getNormal(Vector3D pt)
    {
        Vector3D normal = pt - reference_point;
        normal = normal / sqrt(normal * normal);
        return normal;
    }

    virtual Color getColorAt(Vector3D pt)
    {
        // return color at point pt
        return color;
    }
};

class Triangle : public Object
{
public:
    Vector3D A, B, C;
    Triangle(Vector3D A, Vector3D B, Vector3D C)
    {
        this->A = A;
        this->B = B;
        this->C = C;
        id = 1;
    }
    void draw()
    {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(A.x, A.y, A.z);
            glVertex3f(B.x, B.y, B.z);
            glVertex3f(C.x, C.y, C.z);
        }
        glEnd();
    }

    virtual double intersect(Ray r, Color &c, int level)
    {
        // use barycentric coordinates

        std::vector<std::vector<double>> matrixA = {{A.x - B.x, A.x - C.x, r.dir.x},
                                                    {A.y - B.y, A.y - C.y, r.dir.y},
                                                    {A.z - B.z, A.z - C.z, r.dir.z}};
        std::vector<std::vector<double>> matrixBeta = {{A.x - r.start.x, A.x - C.x, r.dir.x},
                                                       {A.y - r.start.y, A.y - C.y, r.dir.y},
                                                       {A.z - r.start.z, A.z - C.z, r.dir.z}};
        std::vector<std::vector<double>> matrixGamma = {{A.x - B.x, A.x - r.start.x, r.dir.x},
                                                        {A.y - B.y, A.y - r.start.y, r.dir.y},
                                                        {A.z - B.z, A.z - r.start.z, r.dir.z}};
        std::vector<std::vector<double>> matrixT = {{A.x - B.x, A.x - C.x, A.x - r.start.x},
                                                    {A.y - B.y, A.y - C.y, A.y - r.start.y},
                                                    {A.z - B.z, A.z - C.z, A.z - r.start.z}};
        double detA = matrixA[0][0] * (matrixA[1][1] * matrixA[2][2] - matrixA[1][2] * matrixA[2][1]) - matrixA[0][1] * (matrixA[1][0] * matrixA[2][2] - matrixA[1][2] * matrixA[2][0]) + matrixA[0][2] * (matrixA[1][0] * matrixA[2][1] - matrixA[1][1] * matrixA[2][0]);
        // beta = det(matrixBeta) / detA
        double detBeta = matrixBeta[0][0] * (matrixBeta[1][1] * matrixBeta[2][2] - matrixBeta[1][2] * matrixBeta[2][1]) - matrixBeta[0][1] * (matrixBeta[1][0] * matrixBeta[2][2] - matrixBeta[1][2] * matrixBeta[2][0]) + matrixBeta[0][2] * (matrixBeta[1][0] * matrixBeta[2][1] - matrixBeta[1][1] * matrixBeta[2][0]);
        double beta = detBeta / detA;
        // gamma = det(matrixGamma) / detA
        double detGamma = matrixGamma[0][0] * (matrixGamma[1][1] * matrixGamma[2][2] - matrixGamma[1][2] * matrixGamma[2][1]) - matrixGamma[0][1] * (matrixGamma[1][0] * matrixGamma[2][2] - matrixGamma[1][2] * matrixGamma[2][0]) + matrixGamma[0][2] * (matrixGamma[1][0] * matrixGamma[2][1] - matrixGamma[1][1] * matrixGamma[2][0]);
        double gamma = detGamma / detA;
        // t = det(matrixT) / detA
        double detT = matrixT[0][0] * (matrixT[1][1] * matrixT[2][2] - matrixT[1][2] * matrixT[2][1]) - matrixT[0][1] * (matrixT[1][0] * matrixT[2][2] - matrixT[1][2] * matrixT[2][0]) + matrixT[0][2] * (matrixT[1][0] * matrixT[2][1] - matrixT[1][1] * matrixT[2][0]);
        if (detA == 0)
            return -1;
        double t = detT / detA;
        // std::cout<<"Triangle "<<t<<std::endl;
        if (beta > 0 && gamma > 0 && beta + gamma <= 1 && t > 0)
        {
            return t;
        }

        return -1;
        // normalize r
        // Vector3D n_rdir = r.dir / sqrt(r.dir * r.dir);

        // Vector3D normal = (B - A) ^ (C - A);
        // normal = normal / sqrt(normal * normal);

        // double D = -(normal * A);
        // double denominator = normal * n_rdir;
        // if (fabs(denominator) < 0.00001)
        //     return -1;
        // double t = -1 * (normal * r.start + D) / denominator;

        // if (t < 0)
        //     return -1;

        // Vector3D e1 = B - A;
        // Vector3D e2 = C - A;
        // Vector3D P = r.start + (n_rdir * t) - A;
        // float a = -1.0f;
        // float b = -1.0f;

        // if (abs(e1.x * e2.y - e1.y * e2.x) >= 0.000001f && abs(e2.x * e1.y - e1.x * e2.y) >= 0.000001f)
        // {
        //     a = (P.x * e2.y - P.y * e2.x) / (e1.x * e2.y - e1.y * e2.x);
        //     b = (P.x * e1.y - P.y * e1.x) / (e2.x * e1.y - e1.x * e2.y);
        // }
        // else if (abs(e1.y * e2.z - e1.z * e2.y) >= 0.000001f && abs(e2.y * e1.z - e1.y * e2.z) >= 0.000001f)
        // {
        //     a = (P.y * e2.z - P.z * e2.y) / (e1.y * e2.z - e1.z * e2.y);
        //     b = (P.y * e1.z - P.z * e1.y) / (e2.y * e1.z - e1.y * e2.z);
        // }
        // else if (abs(e1.z * e2.x - e1.x * e2.z) >= 0.000001f && abs(e2.x * e1.x - e1.z * e2.x) >= 0.000001f)
        // {
        //     a = (P.z * e2.x - P.x * e2.z) / (e1.z * e2.x - e1.x * e2.z);
        //     b = (P.z * e1.x - P.x * e1.z) / (e2.z * e1.x - e1.z * e2.x);
        // }
        // else
        // {
        //     return -3.0f;
        // }

        // if (a < 0.0f)
        // {
        //     return -4.0f;
        // }

        // if (a > 1.0f)
        // {
        //     return -5.0f;
        // }

        // if (b < 0.0f)
        // {
        //     return -6.0f;
        // }

        // if (b > 1.0f)
        // {
        //     return -7.0f;
        // }

        // if (!(a + b <= 1.0f))
        // {
        //     return -8.0f;
        // }

        // return t;
    }

    // find normal of the triangle
    // virtual Vector3D getNormal()
    // {
    //     Vector3D normal = (B - A) ^ (C - A);
    //     normal = normal / sqrt(normal * normal);
    //     return normal;
    // }

    virtual Vector3D getNormal(Vector3D pt)
    {
        Vector3D normal = (B - A) ^ (C - A);
        normal = normal / sqrt(normal * normal);
        return normal;
    }

    virtual Color getColorAt(Vector3D pt)
    {
        // return color at point pt
        return color;
    }
};

class Floor : public Object
{
public:
    int tileCount;
    Floor(int floorWidth, int tileWidth)
    {
        reference_point = (-floorWidth / 2, -floorWidth / 2, 0);
        length = tileWidth;
        tileCount = floorWidth / tileWidth;
        id = 2;
    }
    void draw()
    {
        int gridSize = 20;

        for (int i = -tileCount / 2; i < tileCount / 2; i++)
        {
            for (int j = -tileCount / 2; j < tileCount / 2; j++)
            {
                if ((i + j) % 2 == 0)
                    glColor3f(1, 1, 1);
                else
                    glColor3f(0, 0, 0);

                glBegin(GL_QUADS);
                glVertex3f(i * length, j * length, 0);
                glVertex3f((i + 1) * length, j * length, 0);
                glVertex3f((i + 1) * length, (j + 1) * length, 0);
                glVertex3f(i * length, (j + 1) * length, 0);

                glEnd();
            }
        }
    }

    virtual Color getColorAt(Vector3D pt)
    {
        // return color at point pt
        int i = floor((pt.x) / length);
        int j = floor((pt.y) / length);
        if ((i + j) % 2 == 0)
            color = {1.0, 1.0, 1.0}; // White square
        else
            color = {0.0, 0.0, 0.0}; // Black square

        return color;
    }

    virtual double intersect(Ray r, Color &c, int level)
    {
        // find if the ray intersects the floor
        Vector3D normal = Vector3D(0, 0, 1);
        if (fabs(normal * r.dir) <= 0.00001)
            return -1;

        double t = (0 - r.start.z) / r.dir.z;
        if (t < -0.00001)
            return -1;

        Vector3D intersection = r.start + (r.dir * t);
        if (intersection.x >= -tileCount / 2 * length && intersection.x <= tileCount / 2 * length &&
            intersection.y >= -tileCount / 2 * length && intersection.y <= tileCount / 2 * length)
            return t;

        return -1;
    }

    virtual Vector3D getNormal(Vector3D pt)
    {
        return Vector3D(0, 0, 1);
    }
};

class General : public Object
{
public:
    double A, B, C, D, E, F, G, H, I, J;
    double length, width, height;
    General()
    {
    }

    General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J)
    {
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;

        id = 3;
    }

    void draw()
    {
        // draw a general quadratic surface using OpenGL

        // Equation: F(x,y,z) = Ax2+By2+Cz2+Dxy+Exz+Fyz+Gx+Hy+Iz+J = 0
    }

    double intersect(Ray r, Color c, int level)
    {
        // find if the ray intersects the general quadratic surface
        // Equation: F(x,y,z) = Ax2+By2+Cz2+Dxy+Exz+Fyz+Gx+Hy+Iz+J = 0
        // ○ Ray-quadric surface intersection (by plugging in Px = R0x + t*Rdx and similarly Py
        // and Pz from the ray, into the general 3D quadratic equation)
        // ○ If two values of t are obtained, check which one (or none or both) falls within the
        // reference cube i.e. the bounding box within which the general quadric surface
        // needs to be drawn. If any dimension of the bounding box is 0, no clipping along
        // that dimension is required.
        // ○ Normal = (𝜕F/𝜕x, 𝜕F/𝜕y, 𝜕F/𝜕z) [Substitute x, y, z values with that of the
        // intersection point to obtain normals at different points]

        double a = A * r.dir.x * r.dir.x + B * r.dir.y * r.dir.y + C * r.dir.z * r.dir.z + D * r.dir.x * r.dir.y + E * r.dir.x * r.dir.z + F * r.dir.y * r.dir.z;
        double b = 2 * (A * r.start.x * r.dir.x + B * r.start.y * r.dir.y + C * r.start.z * r.dir.z) + D * (r.start.x * r.dir.y + r.start.y * r.dir.x) + E * (r.start.x * r.dir.z + r.start.z * r.dir.x) + F * (r.start.y * r.dir.z + r.start.z * r.dir.y) + G * r.dir.x + H * r.dir.y + I * r.dir.z;
        double c_ = A * r.start.x * r.start.x + B * r.start.y * r.start.y + C * r.start.z * r.start.z + D * r.start.x * r.start.y + E * r.start.x * r.start.z + F * r.start.y * r.start.z + G * r.start.x + H * r.start.y + I * r.start.z + J;

        double D = b * b - 4 * a * c_;

        if (D < 0)
            return -1;

        double t1 = (-b + sqrt(D)) / (2 * a);
        double t2 = (-b - sqrt(D)) / (2 * a);
    }
};