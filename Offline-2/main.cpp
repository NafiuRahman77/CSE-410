// main.cpp
#include <bits/stdc++.h>
#include "Point.hpp"
#include "Triangle.hpp"
#include "Transform.hpp"
#include "bitmap_image.hpp"

using namespace std;

int main()
{

    // stage 1

    ifstream in("scene.txt");
    ofstream out("stage1.txt");

    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    double x, y, z;
    in >> x >> y >> z;
    eye = Point(x, y, z);
    in >> x >> y >> z;
    look = Point(x, y, z);
    in >> x >> y >> z;
    up = Point(x, y, z);

    in >> fovY;
    in >> aspectRatio;
    in >> near;
    in >> far;

    stack<Transform> st;
    Transform tt;
    st.push(tt);

    string command;

    out << fixed << setprecision(7);

    while (in >> command)
    {

        if (command == "triangle")
        {
            Point p1, p2, p3;
            in >> x >> y >> z;
            p1 = Point(x, y, z);
            in >> x >> y >> z;
            p2 = Point(x, y, z);
            in >> x >> y >> z;
            p3 = Point(x, y, z);

            Point r1, r2, r3;

            r1 = st.top().transform(p1);
            r2 = st.top().transform(p2);
            r3 = st.top().transform(p3);

            out << r1.getX() << " " << r1.getY() << " " << r1.getZ() << endl;
            out << r2.getX() << " " << r2.getY() << " " << r2.getZ() << endl;
            out << r3.getX() << " " << r3.getY() << " " << r3.getZ() << endl;
            out << endl;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            in >> tx >> ty >> tz;
            Transform t;
            t.translate(tx, ty, tz);
            st.top() = st.top() * t;
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            in >> sx >> sy >> sz;
            Transform t;
            t.scale(sx, sy, sz);
            st.top() = st.top() * t;
        }
        else if (command == "rotate")
        {
            double angle, rx, ry, rz;
            in >> angle >> rx >> ry >> rz;
            Transform t;
            t.rotate(angle, rx, ry, rz);
            st.top() = st.top() * t;
        }
        else if (command == "push")
        {
            st.push(st.top());
        }
        else if (command == "pop")
        {
            st.pop();
        }
        else if (command == "end")
        {
            break;
        }
    }

    in.close();
    out.close();

    // stage 2

    in.open("stage1.txt");
    out.open("stage2.txt");

    Point l = (look - eye) / (look - eye).getMagnitude();
    Point r = (l ^ up) / (l ^ up).getMagnitude();
    Point u = (r ^ l) / (r ^ l).getMagnitude();

    Transform T;
    T.translate(-eye.getX(), -eye.getY(), -eye.getZ());

    Transform R;
    R.setMatrix({{r.getX(), r.getY(), r.getZ(), 0},
                 {u.getX(), u.getY(), u.getZ(), 0},
                 {-l.getX(), -l.getY(), -l.getZ(), 0},
                 {0, 0, 0, 1}});

    Transform V = R * T;
    while (in >> x >> y >> z)
    {
        Point p1(x, y, z);
        in >> x >> y >> z;
        Point p2(x, y, z);
        in >> x >> y >> z;
        Point p3(x, y, z);

        Triangle t(p1, p2, p3);
        Point r1, r2, r3;
        r1 = V.transform(t.getA());
        r2 = V.transform(t.getB());
        r3 = V.transform(t.getC());

        out << r1.getX() << " " << r1.getY() << " " << r1.getZ() << endl;
        out << r2.getX() << " " << r2.getY() << " " << r2.getZ() << endl;
        out << r3.getX() << " " << r3.getY() << " " << r3.getZ() << endl;
        out << endl;

        if (in.eof())
        {
            break;
        }
    }

    in.close();
    out.close();

    // stage 3

    in.open("stage2.txt");
    out.open("stage3.txt");

    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY * acos(-1) / 360.0);
    double rr = near * tan(fovX * acos(-1) / 360.0);

    Transform P;

    P.setMatrix({{near / rr, 0, 0, 0},
                 {0, near / t, 0, 0},
                 {0, 0, -(far + near) / (far - near), -(2 * far * near) / (far - near)},
                 {0, 0, -1, 0}});

    Point p1, p2, p3;
    while (in >> p1)
    {
        in >> p2;
        in >> p3;

        Triangle t(p1, p2, p3);
        Point r1, r2, r3;
        r1 = P.transform(t.getA());
        r2 = P.transform(t.getB());
        r3 = P.transform(t.getC());

        out << r1;
        out << r2;
        out << r3;
        out << endl;

        if (in.eof())
        {
            break;
        }
    }

    in.close();
    out.close();

    // stage 4

    in.open("config.txt");

    int width, height;
    in >> width >> height;

    in.close();

    in.open("stage3.txt");
    out.open("z_buffer.txt");

    vector<Triangle> triangles;

    while (in >> p1)
    {

        in >> p2;
        in >> p3;

        Triangle t(p1, p2, p3);
        triangles.push_back(t);

        if (in.eof())
        {
            break;
        }
    }

    in.close();

    double boxtop = 1, boxbottom = -1, boxleft = -1, boxright = 1;
    double dx = (boxright - boxleft) / width;
    double dy = (boxtop - boxbottom) / height;
    double topY = boxtop - dy / 2, leftX = boxleft + dx / 2, rightX = boxright - dx / 2, bottomY = boxbottom + dy / 2;
    double z_max = 1.0;

    vector<vector<double>> z_buffer(height, vector<double>(width, z_max));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            z_buffer[i][j] = z_max;
        }
    }

    bitmap_image image(width, height);
    image.set_all_channels(0, 0, 0);

    for (vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); it++)
    {
        Triangle t = *it;
        t.setColor();
        Point p1 = t.getA();
        Point p2 = t.getB();
        Point p3 = t.getC();

        double maxY = max(p1.getY(), max(p2.getY(), p3.getY()));
        double minY = min(p1.getY(), min(p2.getY(), p3.getY()));

        if (maxY > topY)
        {
            maxY = topY;
        }

        if (minY < bottomY)
        {
            minY = bottomY;
        }

        // find top scanline and bottom scanline
        int top_scanline = round((topY - maxY) / dy);
        int bottom_scanline = round((topY - minY) / dy);

        for (int i = top_scanline; i <= bottom_scanline; i++)
        {
            double y = topY - i * dy;

            // create an array using Points of the triangle
            vector<Point> points(3);
            points[0] = t.getA();
            points[1] = t.getB();
            points[2] = t.getC();
            double maxX = INT_MIN, minX = INT_MAX;

            for (int t = 0; t < 3; t++)
            {
                int next = (t + 1) % 3;

                if (abs(points[t].getY() - points[next].getY()) <= abs(1e-9))
                {
                    continue;
                }

                double smaller = min(points[t].getY(), points[next].getY());
                double bigger = max(points[t].getY(), points[next].getY());
                if (y >= smaller && y <= bigger)
                {

                    double x = points[t].getX() + (y - points[t].getY()) * (points[next].getX() - points[t].getX()) / (points[next].getY() - points[t].getY());
                    maxX = max(maxX, x);
                    minX = min(minX, x);
                }
            }

            if (maxX > rightX)
            {
                maxX = rightX;
            }

            if (minX < leftX)
            {
                minX = leftX;
            }

            int left_col = round((minX - leftX) / dx);
            int right_col = round((maxX - leftX) / dx);

            for (int j = left_col; j <= right_col; j++)
            {
                double x = leftX + j * dx;
                Point s = p1, pp0 = p2 - p1, pp1 = p3 - p2;

                double k0 = ((pp1.getY() * (x - s.getX()) - pp1.getX() * (y - s.getY())) / (pp0.getX() * pp1.getY() - pp1.getX() * pp0.getY()));
                double k1 = ((pp0.getY() * (x - s.getX()) - pp0.getX() * (y - s.getY())) / (pp1.getX() * pp0.getY() - pp0.getX() * pp1.getY()));

                Point tp = s + pp0 * k0 + pp1 * k1;

                double depth = s.getZ() + pp0.getZ() * k0 + pp1.getZ() * k1;

                if (i >= 0 && i < height && j >= 0 && j < width)
                {

                    if (depth < z_buffer[i][j] && depth < z_max && depth > -z_max)
                    {
                        z_buffer[i][j] = depth;
                        image.set_pixel(j, i, t.getColor()[0], t.getColor()[1], t.getColor()[2]);
                    }
                }
            }
        }
    }

    out << fixed << setprecision(6);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            if (abs(z_buffer[i][j] - z_max) <= abs(1e-9))
            {

                out << "";
            }

            else

                out << z_buffer[i][j] << "\t";
        }
        out << endl;
    }

    out.close();

    image.save_image("out.bmp");

    return 0;
}