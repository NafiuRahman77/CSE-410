#include <bits/stdc++.h>

#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

int drawgrid;
int drawaxes;

double sphere_radius = 10;
double sphere_stacks = 25;
double sphere_slices = 30;
double translate_value = 20;


int clockwise = 1;
int counterclockwise = -1;
int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;


struct Point
{
	double x = 0, y = 0, z = 0;
    // constructor
    Point(double x = 0, double y = 0, double z = 0){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    // copy constructor
    Point(const Point &p){
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
    // assignment operator
    Point& operator=(const Point &p){
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        return *this;
    }
    // destructor
    ~Point(){}
    // addition operator
    Point operator+(const Point &p){
        return Point(this->x + p.x, this->y + p.y, this->z + p.z);
    }
    // subtraction operator
    Point operator-(const Point &p){
        return Point(this->x - p.x, this->y - p.y, this->z - p.z);
    }
    //scalar multiplication operator
    Point operator*(const double &d){
        return Point(this->x * d, this->y * d, this->z * d);
    }
    // division operator
    Point operator/(const double &d){
        return Point(this->x / d, this->y / d, this->z / d);
    }
    // dot product
    double operator*(const Point &p){
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }
    // cross product
    Point operator^(const Point &p){
        return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
    }
   
};

struct Point pos(100,100,0), u(0,0,1), r(-1/sqrt(2.0), 1/sqrt(2.0),0), l(-1/sqrt(2.0), -1/sqrt(2.0), 0);


void drawAxes(){
	if(drawaxes==1){
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid(){
	if(drawgrid==1){
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i = -8; i <= 8; i++){
				if(i == 0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':    // Look left
			//l = Rotation(l, u, counterclockwise);
            l.x = l.x * cos(rotation_angle) - (l^u).x * sin(rotation_angle);
            l.y = l.y * cos(rotation_angle) - (l^u).y * sin(rotation_angle);
            l.z = l.z * cos(rotation_angle) - (l^u).z * sin(rotation_angle);
            
			//r = Rotation(r, u, counterclockwise);
            r.x = r.x * cos(rotation_angle) - (r^u).x * sin(rotation_angle);
            r.y = r.y * cos(rotation_angle) - (r^u).y * sin(rotation_angle);
            r.z = r.z * cos(rotation_angle) - (r^u).z * sin(rotation_angle);
			break;

        case '2':    // Look right
			//l = Rotation(l, u, clockwise);
            l.x = l.x * cos(rotation_angle) + (l^u).x * sin(rotation_angle);
            l.y = l.y * cos(rotation_angle) + (l^u).y * sin(rotation_angle);
            l.z = l.z * cos(rotation_angle) + (l^u).z * sin(rotation_angle);
            //r = Rotation(r, u, clockwise);
            r.x = r.x * cos(rotation_angle) + (r^u).x * sin(rotation_angle);
            r.y = r.y * cos(rotation_angle) + (r^u).y * sin(rotation_angle);
            r.z = r.z * cos(rotation_angle) + (r^u).z * sin(rotation_angle);

            break;

        case '3':    // Look up
			// l = Rotation(l, r, counterclockwise);
			// u = Rotation(u, r, counterclockwise);
            l.x = l.x * cos(rotation_angle) - (l^r).x * sin(rotation_angle);
            l.y = l.y * cos(rotation_angle) - (l^r).y * sin(rotation_angle);
            l.z = l.z * cos(rotation_angle) - (l^r).z * sin(rotation_angle);
            
            u.x = u.x * cos(rotation_angle) - (u^r).x * sin(rotation_angle);    
            u.y = u.y * cos(rotation_angle) - (u^r).y * sin(rotation_angle);
            u.z = u.z * cos(rotation_angle) - (u^r).z * sin(rotation_angle);
            break;

        case '4':    // Look down
			// l = Rotation(l, r, clockwise);
			// u = Rotation(u, r, clockwise);
            l.x = l.x * cos(rotation_angle) + (l^r).x * sin(rotation_angle);
            l.y = l.y * cos(rotation_angle) + (l^r).y * sin(rotation_angle);
            l.z = l.z * cos(rotation_angle) + (l^r).z * sin(rotation_angle);

            u.x = u.x * cos(rotation_angle) + (u^r).x * sin(rotation_angle);
            u.y = u.y * cos(rotation_angle) + (u^r).y * sin(rotation_angle);
            u.z = u.z * cos(rotation_angle) + (u^r).z * sin(rotation_angle);
            
            break;

        case '5':    // tilt clockwise
			// r = Rotation(r, l, counterclockwise);
			// u = Rotation(u, l, counterclockwise);
            r.x = r.x * cos(rotation_angle) - (r^l).x * sin(rotation_angle);
            r.y = r.y * cos(rotation_angle) - (r^l).y * sin(rotation_angle);
            r.z = r.z * cos(rotation_angle) - (r^l).z * sin(rotation_angle);

            u.x = u.x * cos(rotation_angle) - (u^l).x * sin(rotation_angle);
            u.y = u.y * cos(rotation_angle) - (u^l).y * sin(rotation_angle);
            u.z = u.z * cos(rotation_angle) - (u^l).z * sin(rotation_angle);
            break;

        case '6':    // tilt counter clockwise
			// r = Rotation(r, l, clockwise);
			// u = Rotation(u, l, clockwise);

            r.x = r.x * cos(rotation_angle) + (r^l).x * sin(rotation_angle);
            r.y = r.y * cos(rotation_angle) + (r^l).y * sin(rotation_angle);
            r.z = r.z * cos(rotation_angle) + (r^l).z * sin(rotation_angle);

            u.x = u.x * cos(rotation_angle) + (u^l).x * sin(rotation_angle);
            u.y = u.y * cos(rotation_angle) + (u^l).y * sin(rotation_angle);
            u.z = u.z * cos(rotation_angle) + (u^l).z * sin(rotation_angle);
            
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key, zoom out
			pos.x -= l.x;
			pos.y -= l.y;
			pos.z -= l.z;
			break;

		case GLUT_KEY_UP:		// up arrow key, zoom in
			pos.x += l.x;
			pos.y += l.y;
			pos.z += l.z;
			break;

		case GLUT_KEY_RIGHT:   // right arrow, move right
			pos.x += r.x;
			pos.y += r.y;
			pos.z += r.z;
			break;

		case GLUT_KEY_LEFT:    // left arrow, move left
			pos.x -= r.x;
			pos.y -= r.y;
			pos.z -= r.z;
			break;

		case GLUT_KEY_PAGE_UP:  // page up, move up
			pos.x += u.x;
			pos.y += u.y;
			pos.z += u.z;
			break;

		case GLUT_KEY_PAGE_DOWN:   // page down, move down
			pos.x -= u.x;
			pos.y -= u.y;
			pos.z -= u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			if(translate_value > 0){
				sphere_radius += 5;
				translate_value -= 5;
			}
			break;

		case GLUT_KEY_END:
			if(sphere_radius > 0){
				sphere_radius -= 5;
				translate_value += 5;
			}
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid=1-drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//Camera Position, position of where camera is looking, aand the up direction of camera
    gluLookAt(pos.x, pos.y, pos.z,	pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//add objects
	drawAxes();
	drawGrid();

		//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;

	// pos.x = 100;
	// pos.y = 100;
	// pos.z = 0;

	// l.x = -1/sqrt(2.0);
	// l.y = -1/sqrt(2.0);
	// l.z = 0;

	// r.x = -1/sqrt(2.0);
	// r.y = 1/sqrt(2.0);
	// r.z = 0;

	// u.x = 0;
	// u.y = 0;
	// u.z = 1;

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 1 and 2: Cube and Sphere");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}