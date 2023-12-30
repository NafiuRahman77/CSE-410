// main.cpp
#include <bits/stdc++.h>
#include "Point.hpp"
#include "Triangle.hpp"
#include "Transform.hpp"

using namespace std;

int main()
{

    // stage 1

    ifstream in("scene.txt");
    ofstream out("stage1.txt");

    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    // //set eye.x eye.y eye.z from in
    double x, y, z;
    in >> x >> y >> z;
    eye = Point(x, y, z);
    // set look.x look.y look.z from in
    in >> x >> y >> z;
    look = Point(x, y, z);
    // set up.x up.y up.z from in
    in >> x >> y >> z;
    up = Point(x, y, z);

    // set fovY from in
    in >> fovY;
    // set aspectRatio from in
    in >> aspectRatio;
    // set near from in
    in >> near;
    // set far from in
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
            // set p1.x p1.y p1.z from in
            in >> x >> y >> z;
            p1 = Point(x, y, z);
            // set p2.x p2.y p2.z from in
            in >> x >> y >> z;
            p2 = Point(x, y, z);
            // set p3.x p3.y p3.z from in
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
            // set tx ty tz from in
            in >> tx >> ty >> tz;
            Transform t;
            t.translate(tx, ty, tz);
            st.top() = st.top() * t;
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            // set sx sy sz from in
            in >> sx >> sy >> sz;
            Transform t;
            t.scale(sx, sy, sz);
            st.top() = st.top() * t;
        }
        else if (command == "rotate")
        {
            double angle, rx, ry, rz;
            // set angle rx ry rz from in
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

    Point l = (look - eye)/(look - eye).getMagnitude();
    Point r = (l ^ up)/(l ^ up).getMagnitude();
    Point u = (r ^ l)/(r ^ l).getMagnitude();

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

        // Explicitly break at the end of the file (optional)
        if (in.eof())
        {
            break;
        }
    }
}