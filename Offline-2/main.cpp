//main.cpp
#include<iostream>
#include"Point.hpp"
#include"Triangle.hpp"
#include"Transform.hpp"

using namespace std;

int main(){
    
    Transform t;
    t.translate(1,2,3);
    t.print();
    cout<<endl;

    Transform t2;
    t2.scale(1,2,3);
    t2.print();

    cout<<endl;

    //multiply t and t2 
    Transform t3 = t*t2;
    t3.print();
}