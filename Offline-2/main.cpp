//main.cpp
#include<bits/stdc++.h>
#include"Point.hpp"
#include"Triangle.hpp"
#include"Transform.hpp"

using namespace std;

int main(){
    
    ifstream in("scene.txt");
    ofstream out("stage1.txt");

    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    //set eye.x eye.y eye.z from in
    double x,y,z;
    in>>x>>y>>z;
    eye = Point(x,y,z);
    //set look.x look.y look.z from in
    in>>x>>y>>z;
    look = Point(x,y,z);
    //set up.x up.y up.z from in
    in>>x>>y>>z;
    up = Point(x,y,z);
   
    //set fovY from in
    in>>fovY;
    //set aspectRatio from in
    in>>aspectRatio;
    //set near from in
    in>>near;
    //set far from in
    in>>far;

    stack<Transform> st;
    Transform t;
    st.push(t);

    // print top of stack
    st.top().print();

}