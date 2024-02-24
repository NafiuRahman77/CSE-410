#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<bits/stdc++.h>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct {
    double x, y;
} Point;

double anim_x = 0.25;

using namespace std;

void drawCircle(double radius, Point center){
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<360; i++){
        double angle = degToRad(i);
        double x = center.x + radius*cos(angle);
        double y = center.y + radius*sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

double angle = 0, angle2=0;


void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    // draw circle

    glColor3f(0, 0, 1);
    drawCircle(0.5, {0, 0});

    // draw a line from center to a point on the circle
   
    glBegin(GL_LINES);
    Point center = {0, 0};
    glVertex2f(center.x, center.y);
    Point marker = {center.x + 0.5*cos(degToRad(angle)), center.y + 0.5*sin(degToRad(angle))};
    glVertex2f(marker.x, marker.y);
    glEnd();
    
    // vary the angle of the line and draw the line continuously such that it looks like a rotating line
   
    angle += 0.05;
    if(angle > 360) angle = 0;

    // now that we have drawn the line and the circle, we will draw a sine wave that will be traced by the line
     glColor3f(1, 0, 0);
    drawCircle(0.2,marker);

    // draw a line from center to a point on the circle
   
    glBegin(GL_LINES);
    Point center2 = marker;
    glVertex2f(center2.x, center2.y);
    Point marker2 = {center2.x + 0.2*cos(degToRad(angle2)), center2.y + 0.2*sin(degToRad(angle2))};
    glVertex2f(marker2.x, marker2.y);
    glEnd();

    angle2 += 0.15;
    if(angle2 > 360) angle2 = 0;

    glColor3f(1, 1, 0);
    drawCircle(0.05,marker2);

    glFlush();
}

void idle(){
    glutPostRedisplay();
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int xx, int yy)
{
    switch (key){

    case 'a': 
        break;

    default:
        return;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
    glutInitWindowPosition(0, 0);
    glutCreateWindow("opengl");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardListener);
    anim8(0);
    glutMainLoop();
    return 0;
}
