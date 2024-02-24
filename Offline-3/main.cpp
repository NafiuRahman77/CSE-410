#include <bits/stdc++.h>

#include <GL/glut.h>
#include "Point.hpp"

using namespace std;

#define pi (2 * acos(0.0))
// ray tracing problem
// 
int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;

Point pos(2, 2, 2), u(0, 0, 1), r(-2, 2, 0), l(-2, -2, -2);

void drawAxes()
{

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }
    glEnd();
}

void loadData(){
    
}

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {

    case '1':

        l.x = l.x * cos(rotation_angle) - (l ^ u).x * sin(rotation_angle);
        l.y = l.y * cos(rotation_angle) - (l ^ u).y * sin(rotation_angle);
        l.z = l.z * cos(rotation_angle) - (l ^ u).z * sin(rotation_angle);

        r.x = r.x * cos(rotation_angle) - (r ^ u).x * sin(rotation_angle);
        r.y = r.y * cos(rotation_angle) - (r ^ u).y * sin(rotation_angle);
        r.z = r.z * cos(rotation_angle) - (r ^ u).z * sin(rotation_angle);

        break;

    case '2':

        l.x = l.x * cos(rotation_angle) + (l ^ u).x * sin(rotation_angle);
        l.y = l.y * cos(rotation_angle) + (l ^ u).y * sin(rotation_angle);
        l.z = l.z * cos(rotation_angle) + (l ^ u).z * sin(rotation_angle);

        r.x = r.x * cos(rotation_angle) + (r ^ u).x * sin(rotation_angle);
        r.y = r.y * cos(rotation_angle) + (r ^ u).y * sin(rotation_angle);
        r.z = r.z * cos(rotation_angle) + (r ^ u).z * sin(rotation_angle);

        break;

    case '3':

        l.x = l.x * cos(rotation_angle) - (l ^ r).x * sin(rotation_angle);
        l.y = l.y * cos(rotation_angle) - (l ^ r).y * sin(rotation_angle);
        l.z = l.z * cos(rotation_angle) - (l ^ r).z * sin(rotation_angle);

        u.x = u.x * cos(rotation_angle) - (u ^ r).x * sin(rotation_angle);
        u.y = u.y * cos(rotation_angle) - (u ^ r).y * sin(rotation_angle);
        u.z = u.z * cos(rotation_angle) - (u ^ r).z * sin(rotation_angle);

        break;

    case '4':
        l.x = l.x * cos(rotation_angle) + (l ^ r).x * sin(rotation_angle);
        l.y = l.y * cos(rotation_angle) + (l ^ r).y * sin(rotation_angle);
        l.z = l.z * cos(rotation_angle) + (l ^ r).z * sin(rotation_angle);

        u.x = u.x * cos(rotation_angle) + (u ^ r).x * sin(rotation_angle);
        u.y = u.y * cos(rotation_angle) + (u ^ r).y * sin(rotation_angle);
        u.z = u.z * cos(rotation_angle) + (u ^ r).z * sin(rotation_angle);

        break;

    case '5':
        r.x = r.x * cos(rotation_angle) - (r ^ l).x * sin(rotation_angle);
        r.y = r.y * cos(rotation_angle) - (r ^ l).y * sin(rotation_angle);
        r.z = r.z * cos(rotation_angle) - (r ^ l).z * sin(rotation_angle);

        u.x = u.x * cos(rotation_angle) - (u ^ l).x * sin(rotation_angle);
        u.y = u.y * cos(rotation_angle) - (u ^ l).y * sin(rotation_angle);
        u.z = u.z * cos(rotation_angle) - (u ^ l).z * sin(rotation_angle);
        break;

    case '6':
        r.x = r.x * cos(rotation_angle) + (r ^ l).x * sin(rotation_angle);
        r.y = r.y * cos(rotation_angle) + (r ^ l).y * sin(rotation_angle);
        r.z = r.z * cos(rotation_angle) + (r ^ l).z * sin(rotation_angle);

        u.x = u.x * cos(rotation_angle) + (u ^ l).x * sin(rotation_angle);
        u.y = u.y * cos(rotation_angle) + (u ^ l).y * sin(rotation_angle);
        u.z = u.z * cos(rotation_angle) + (u ^ l).z * sin(rotation_angle);

        break;

    case ',':

        break;

    case '.':

        break;
    case 'a':

        break;
    case 'd':

        break;
    case 'w':

        break;
    case 's':

        break;
    default:
        break;
    }
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN: // down arrow key, zoom out
        pos = pos - l * 0.01;
        break;

    case GLUT_KEY_UP: // up arrow key, zoom in
        pos = pos + l * 0.01;
        break;

    case GLUT_KEY_RIGHT: // right arrow, move right
        pos = pos + r * 0.01;
        break;

    case GLUT_KEY_LEFT: // left arrow, move left
        pos = pos - r * 0.01;
        break;

    case GLUT_KEY_PAGE_UP: // page up, move up
        pos = pos + u * 0.01;
        break;

    case GLUT_KEY_PAGE_DOWN: // page down, move down
        pos = pos - u * 0.01;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;

    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}

void mouseListener(int button, int state, int x, int y)
{ // x, y is the x-y of the screen (2D)
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)

            break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)

            break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display()
{
    // clear the display

    glClearColor(0, 0, 0, 0); // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    // initialize the matrix
    glLoadIdentity();

    // Camera Position, position of where camera is looking, aand the up direction of camera
    gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

    // again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    drawAxes();

    // ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void init()
{
    // codes for initialization

    // clear the screen
    glClearColor(0, 0, 0, 0);

    // load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    // initialize the matrix
    glLoadIdentity();

    // give PERSPECTIVE parameters
    gluPerspective(80, 1, 1, 1000.0);
    // field of view in the Y (vertically)
    // aspect ratio that determines the field of view in the X direction (horizontally)
    // near this->distToTranslateance
    // far this->distToTranslateance
}

void animate()
{
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(1000, 100);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

    glutCreateWindow("Task 1 and 2: Cube and Sphere");

    init();

    glEnable(GL_DEPTH_TEST); // enable Depth Testing

    glutDisplayFunc(display); // display callback function
    glutIdleFunc(animate);    // what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop(); // The main loop of OpenGL

    return 0;
}