#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define KEY_ESCAPE 27
#define STEPS 16
#define CYLINDER_ANGLE 70.5287794


typedef struct {
    GLdouble x, y, z;
} Point;

// Global variables
Point eyePos;   // pos
Point lookDir;  // l
Point rightDir; // r
Point upDir;    // u
Point center;

const double OneByRoot2 = 1.0 / (sqrt(2));

bool isAxes = true;
bool isFillTriangle = true;
bool isOctahedron = true;
bool isSphere = true;
bool isCylinder = true;

bool colorToggle = false;

// Vertices of front facing triangle
Point vx;
Point vy;
Point vz;

double sphereRadius;
double objectRotationHorAngle;
double objectRotationVerAngle;

using namespace std;

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

// color functions
void colorBlue() {glColor3f(0.0f, 0.0f, 1.0f);} // Blue

void colorRed() {glColor3f(1.0f, 0.0f, 0.0f);} // Red

void colorGreen() {glColor3f(0.0f, 1.0f, 0.0f);} // Green

void colorCyan() {glColor3f(0.0f, 1.0f, 1.0f);} // Cyan

void colorMagenta() {glColor3f(1.0f, 0.0f, 1.0f);} // Magenta

void colorYellow() {glColor3f(1.0f, 1.0f, 0.0f);} // Yellow

void point(Point A)
{
    glVertex3f(A.x, A.y, A.z);
}

void drawTriangle()
{
    glPushMatrix();
        double scaleFactor = 1 - sqrt(2) * sphereRadius;
        double translateFactor = 1/sqrt(3) * sphereRadius;
        glTranslatef(translateFactor, translateFactor, translateFactor);
        // glTranslatef(sphereRadius/sqrt(3), sphereRadius/sqrt(3), sphereRadius/sqrt(3));
        glScalef(scaleFactor, scaleFactor, scaleFactor);
        if (isFillTriangle) glBegin(GL_TRIANGLES);
        else glBegin(GL_LINE_LOOP);
        point(vx);
        point(vy);
        point(vz);
        glEnd();
    glPopMatrix();
}

// https://www.songho.ca/opengl/gl_sphere.html
vector<vector<Point>> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;
    vector<vector<Point>> vertices(pointsPerRow);
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis


    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices[i].push_back({v[0], v[1], v[2]});
        }
    }

    return vertices;
}

void drawSphereSegment()
{
    vector<vector<Point>> vertices = buildUnitPositiveX(3);
    int pointsPerRow = (int)pow(2, 3) + 1;
    int index = 0;
    glScalef(sphereRadius, sphereRadius, sphereRadius); 
    for (int i = 0; i < pointsPerRow - 1; i++)
    {
        glBegin(GL_QUADS);
        for (int j = 0; j < pointsPerRow - 1; j++)
        {
            point(vertices[i][j]); 
            point(vertices[i][j + 1]);
            point(vertices[i + 1][j + 1]);
            point(vertices[i + 1][j]);
        }
        glEnd();
    }
}

void drawAllSegmentsSphere(){
    double dist = 1 - sqrt(2) * sphereRadius;
    double translateFactor[4][3] = {{dist, 0, 0}, {0, 0, -dist}, {-dist, 0, 0}, {0, 0, dist}};

    for (int i = 0; i < 4; i++) {
        if (i%2 == 0) colorRed();
        else colorBlue();
        glPushMatrix();
            glTranslatef(translateFactor[i][0], translateFactor[i][1], translateFactor[i][2]);
            glRotatef(i * 90, 0, 1, 0);
            drawSphereSegment();
        glPopMatrix();
    }

    colorGreen();  

    glPushMatrix();
        glTranslatef(0, dist, 0);
        glRotatef(90, 0, 0, 1);
        drawSphereSegment();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -dist, 0);
        glRotatef(-90, 0, 0, 1);
        drawSphereSegment();
    glPopMatrix();

}

void drawPyramid()
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
            if (colorToggle) colorCyan();
            else colorMagenta();
            glRotatef(i * 90, 0, 1, 0);
            drawTriangle();
        glPopMatrix();
        colorToggle = !colorToggle;
    }
}

void drawOctahedron()
{
    drawPyramid();
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        drawPyramid();
    glPopMatrix();
}

void drawCylinderSegment(float angle, float radius, float height) {
    const int numSegments = 100;
    const float segmentAngle = angle * 3.1415f / 180.0f;

    glPushMatrix();
        glTranslatef(0.0f, -height/2, 0.0f);
        glRotatef(angle/2, 0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= numSegments; ++i) {
            float theta = i * segmentAngle / numSegments;

            float x = radius*cos(theta);
            float z = radius*sin(theta);

            glVertex3f(x, 0.0f, z);
            glVertex3f(x, height*1.0f, z);
        }
        glEnd();
    glPopMatrix();
}

void drawAllSegmentsCylinder(){
    double radius = sphereRadius; 
    double height = sqrt(2) - 2*radius;
    double t = (1 - sqrt(2) * radius)/2;

    colorYellow();
    
    // 8 cylinder segments (parralel to XY and YZ plane)
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
            glRotatef(i * 180, 1, 0, 0);
            for (int j = 0; j < 4; j++) {
                glPushMatrix();
                    glRotatef(j * 90, 0, 1, 0);
                    glPushMatrix();
                        glTranslatef(t, t, 0);
                        glRotatef(45, 0, 0, 1);
                        drawCylinderSegment(CYLINDER_ANGLE, radius, height);
                    glPopMatrix();
                glPopMatrix();
            }
        glPopMatrix();
    }   
    
    // 4 cylinder segments (parallel to XZ plane)
    for (int j = 0; j < 4; j++) {
        glPushMatrix();
            glRotatef(j * 90, 0, 1, 0);
            glPushMatrix();
                glRotatef(90, 1, 0, 0);
                glPushMatrix();
                    glTranslatef(t, t, 0);
                    glRotatef(45, 0, 0, 1);
                    drawCylinderSegment(CYLINDER_ANGLE, radius, height);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    }

}

void drawAxes(int length)
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);

    glColor3f(0, 1, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);

    glColor3f(0, 0, 1); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);
    glEnd();
}

void resetScene(){
    // Set up the camera
    eyePos = {2, 2, 2};     // pos
    lookDir = {-2, -2, -2}; // l
    upDir = {0, 1, 0};      // u
    rightDir = {upDir.z * (lookDir.y - eyePos.y) - upDir.y * (lookDir.z - eyePos.z), 
                upDir.x * (lookDir.z - eyePos.z) - upDir.z * (lookDir.x - eyePos.x), 
                upDir.y * (lookDir.x - eyePos.x) - upDir.x * (lookDir.y - eyePos.y)}; // r, cross product of l and u

    // Initial vertices of triangle
    vx = {1, 0, 0};
    vy = {0, 1, 0};
    vz = {0, 0, 1};

    // Initial sphere radius (starts from zero)
    sphereRadius = 0;
    // Initial object rotation angle (no rotation without input)
    objectRotationHorAngle = 0;
    objectRotationVerAngle = 0;
}

//! display()
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    center = {eyePos.x + lookDir.x, eyePos.y + lookDir.y, eyePos.z + lookDir.z};
    rightDir = {upDir.z * (lookDir.y - eyePos.y) - upDir.y * (lookDir.z - eyePos.z), 
                upDir.x * (lookDir.z - eyePos.z) - upDir.z * (lookDir.x - eyePos.x), 
                upDir.y * (lookDir.x - eyePos.x) - upDir.x * (lookDir.y - eyePos.y)};
    // control viewing (or camera)
    gluLookAt(eyePos.x, eyePos.y, eyePos.z,
              center.x, center.y, center.z,
              upDir.x, upDir.y, upDir.z);
    // draw
    glPushMatrix();
        // glRotatef(objectRotationHorAngle, upDir.x, upDir.y, upDir.z);
        glRotatef(objectRotationHorAngle, 0, 1, 0);
        // glRotatef(objectRotationVerAngle, rightDir.x, rightDir.y, rightDir.z);
        glPushMatrix();
            // glRotatef(objectRotationVerAngle, 1, 0, -1);
            if (isAxes)         drawAxes(2);
            if (isOctahedron)   drawOctahedron();
            if (isSphere)       drawAllSegmentsSphere();
            if (isCylinder)     drawAllSegmentsCylinder();
        // glPopMatrix();
    glPopMatrix();
    glutSwapBuffers(); // Render now
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int xx, int yy)
{
    double rateDeg = 5;
    double rate = degToRad(rateDeg);
    double s, v = 0.01;
    switch (key){

    case '1': // look left
        rightDir.x = rightDir.x * cos(rate) + lookDir.x * sin(rate);
        rightDir.y = rightDir.y * cos(rate) + lookDir.y * sin(rate);
        rightDir.z = rightDir.z * cos(rate) + lookDir.z * sin(rate);

        lookDir.x = lookDir.x * cos(rate) - rightDir.x * sin(rate);
        lookDir.y = lookDir.y * cos(rate) - rightDir.y * sin(rate);
        lookDir.z = lookDir.z * cos(rate) - rightDir.z * sin(rate);
        break;

    case '2': // look right
        rightDir.x = rightDir.x * cos(-rate) + lookDir.x * sin(-rate);
        rightDir.y = rightDir.y * cos(-rate) + lookDir.y * sin(-rate);
        rightDir.z = rightDir.z * cos(-rate) + lookDir.z * sin(-rate);

        lookDir.x = lookDir.x * cos(-rate) - rightDir.x * sin(-rate);
        lookDir.y = lookDir.y * cos(-rate) - rightDir.y * sin(-rate);
        lookDir.z = lookDir.z * cos(-rate) - rightDir.z * sin(-rate);
        break;

    case '3': // look up
        lookDir.x = lookDir.x * cos(rate) + upDir.x * sin(rate);
        lookDir.y = lookDir.y * cos(rate) + upDir.y * sin(rate);
        lookDir.z = lookDir.z * cos(rate) + upDir.z * sin(rate);

        upDir.x = upDir.x * cos(rate) - lookDir.x * sin(rate);
        upDir.y = upDir.y * cos(rate) - lookDir.y * sin(rate);
        upDir.z = upDir.z * cos(rate) - lookDir.z * sin(rate);
        break;

    case '4': // look down
        lookDir.x = lookDir.x * cos(-rate) + upDir.x * sin(-rate);
        lookDir.y = lookDir.y * cos(-rate) + upDir.y * sin(-rate);
        lookDir.z = lookDir.z * cos(-rate) + upDir.z * sin(-rate);

        upDir.x = upDir.x * cos(-rate) - lookDir.x * sin(-rate);
        upDir.y = upDir.y * cos(-rate) - lookDir.y * sin(-rate);
        upDir.z = upDir.z * cos(-rate) - lookDir.z * sin(-rate);
        break;

    case '5': // tilt counter-clockwise
        upDir.x = upDir.x * cos(rate) + rightDir.x * sin(rate);
        upDir.y = upDir.y * cos(rate) + rightDir.y * sin(rate);
        upDir.z = upDir.z * cos(rate) + rightDir.z * sin(rate);

        rightDir.x = rightDir.x * cos(rate) - upDir.x * sin(rate);
        rightDir.y = rightDir.y * cos(rate) - upDir.y * sin(rate);
        rightDir.z = rightDir.z * cos(rate) - upDir.z * sin(rate);
        break;

    case '6': // tilt clockwise
        upDir.x = upDir.x * cos(-rate) + rightDir.x * sin(-rate);
        upDir.y = upDir.y * cos(-rate) + rightDir.y * sin(-rate);
        upDir.z = upDir.z * cos(-rate) + rightDir.z * sin(-rate);

        rightDir.x = rightDir.x * cos(-rate) - upDir.x * sin(-rate);
        rightDir.y = rightDir.y * cos(-rate) - upDir.y * sin(-rate);
        rightDir.z = rightDir.z * cos(-rate) - upDir.z * sin(-rate);
        break;

    case 'a':
        objectRotationHorAngle += rateDeg;
        break;

    case 'd':
        objectRotationHorAngle -= rateDeg;
        break;

    case 'w':
        objectRotationVerAngle += rateDeg;
        break;

    case 's':
        objectRotationVerAngle -= rateDeg;
        break;

    case 'r':
        resetScene();
        break;

    case 'f':
        isFillTriangle = !isFillTriangle;
        break;

    case 'g':
        isOctahedron = !isOctahedron;
        break;
    
    case 'h':
        isSphere = !isSphere;
        break;
    
    case 'j':
        isCylinder = !isCylinder;
        break;
    
    case 'x':
        isAxes = !isAxes;
        break;

    case ',': 
        sphereRadius += OneByRoot2/16.0;
        if(sphereRadius > OneByRoot2) sphereRadius = OneByRoot2;
        break;
    
    case '.': 
        sphereRadius -= OneByRoot2/16.0;
        if(sphereRadius < 0) sphereRadius = 0;
        break; 

    default:
        return;
    }

    glutPostRedisplay();
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y)
{
    double rate = 0.01;
    switch (key){

    case GLUT_KEY_UP: 
        eyePos.x += lookDir.x * rate;
        eyePos.y += lookDir.y * rate;
        eyePos.z += lookDir.z * rate;
        break;
    case GLUT_KEY_DOWN: 
        eyePos.x -= lookDir.x * rate;
        eyePos.y -= lookDir.y * rate;
        eyePos.z -= lookDir.z * rate;
        break;

    case GLUT_KEY_RIGHT:
        eyePos.x += rightDir.x * rate;
        eyePos.y += rightDir.y * rate;
        eyePos.z += rightDir.z * rate;
        break;
    case GLUT_KEY_LEFT:
        eyePos.x -= rightDir.x * rate;
        eyePos.y -= rightDir.y * rate;
        eyePos.z -= rightDir.z * rate;
        break;

    case GLUT_KEY_PAGE_UP:
        eyePos.x += upDir.x * rate;
        eyePos.y += upDir.y * rate;
        eyePos.z += upDir.z * rate;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eyePos.x -= upDir.x * rate;
        eyePos.y -= upDir.y * rate;
        eyePos.z -= upDir.z * rate;
        break;

    default:
        return;
    }
    glutPostRedisplay();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char **argv)
{
    resetScene(); // initial position

    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(1000, 100);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("It's Cubin' Time");
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    // glutIdleFunc(idle);                         // Register idle callback handler for window re-paint
    glutReshapeFunc(reshapeListener);    // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);  // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener); // Register callback handler for special-key event
    initGL();
    glutMainLoop();
    return 0;
}