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


struct point
{
	double x = 0, y = 0, z = 0;
};

struct point pos, u, r, l;

point Cross_Product(point u, point v){
	point cross;
	cross.x = u.y * v.z - u.z * v.y;
	cross.y = u.z * v.x - u.x * v.z;
	cross.z = u.x * v.y - u.y * v.x;

	return cross;
}

point Rotation(point v, point reference, int dir){
	point cross = Cross_Product(v, reference);
	point new_point;

	double actual_rotation_angle = dir * rotation_angle;
	
	new_point.x = v.x * cos(rotation_angle) + cross.x * sin(actual_rotation_angle);
	new_point.y = v.y * cos(rotation_angle) + cross.y * sin(actual_rotation_angle);
	new_point.z = v.z * cos(rotation_angle) + cross.z * sin(actual_rotation_angle); 

	return new_point;
}


class Triangle{
public:
    point a, b, c;
    Triangle(point a, point b, point c){
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw(){
        glBegin(GL_TRIANGLES);{
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }glEnd();
    }
};


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

void drawSquare(double a){
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void DrawCube(double a){
	glColor3f(1,1,1);

	// top square
	glPushMatrix();{
		glTranslatef(0, 0, translate_value + sphere_radius);
		drawSquare(translate_value);
	}
	glPopMatrix();

	// bottom square
	glPushMatrix();{
		glTranslatef(0, 0, -translate_value - sphere_radius);
		drawSquare(translate_value);
	}
	glPopMatrix();

	// right square
	glPushMatrix();{
		glTranslatef(0, translate_value + sphere_radius, 0);
		glRotatef(90, 1, 0, 0); 
		drawSquare(translate_value);
	}
	glPopMatrix();

	// left square
	glPushMatrix();{
		glTranslatef(0, -translate_value - sphere_radius, 0);
		glRotatef(90, 1, 0, 0); 
		drawSquare(translate_value);
	}
	glPopMatrix();

	// front square
	glPushMatrix();{
		glTranslatef(translate_value + sphere_radius, 0, 0);
		glRotatef(90, 0, 1, 0); 
		drawSquare(translate_value);
	}
	glPopMatrix();

	// back square
	glPushMatrix();{
		glTranslatef(-translate_value - sphere_radius, 0, 0);
		glRotatef(90, 0, 1, 0); 
		drawSquare(translate_value);
	}
	glPopMatrix();
	
}


void drawcylinder(int radius, int slices, int stacks){
	struct point pos[100][100];
	double h,r;
	int height = 0;
	for(int i = 0; i < stacks; i++){
		h = height;
		r = radius;
		for(int j = 0; j <= slices; j++){
			pos[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			pos[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			pos[i][j].z=h;
		}
		height = translate_value;
	}

	//draw quads using generated pos
	for(int i = 0; i < stacks-1; i++){
		for(int j = 0; j < slices; j++){
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(pos[i][j].x,pos[i][j].y,pos[i][j].z);
				glVertex3f(pos[i][j+1].x,pos[i][j+1].y,pos[i][j+1].z);
				glVertex3f(pos[i+1][j+1].x,pos[i+1][j+1].y,pos[i+1][j+1].z);
				glVertex3f(pos[i+1][j].x,pos[i+1][j].y,pos[i+1][j].z);

				//lower hemisphere
                glVertex3f(pos[i][j].x,pos[i][j].y,-pos[i][j].z);
				glVertex3f(pos[i][j+1].x,pos[i][j+1].y,-pos[i][j+1].z);
				glVertex3f(pos[i+1][j+1].x,pos[i+1][j+1].y,-pos[i+1][j+1].z);
				glVertex3f(pos[i+1][j].x,pos[i+1][j].y,-pos[i+1][j].z);
			}glEnd();
		}
	}
}

void DrawCylinder(){
	glColor3f(0, 1, 0);

	// between +x and + y
	glPushMatrix();{
		glTranslatef(translate_value, translate_value, 0);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// between +x and -y
	glPushMatrix();{
		glTranslatef(translate_value, -translate_value, 0);
		glRotatef(-90, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// between -x and +y
	glPushMatrix();{
		glTranslatef(-translate_value, +translate_value, 0);
		glRotatef(90, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// between -x and -y
	glPushMatrix();{
		glTranslatef(-translate_value, -translate_value, 0);
		glRotatef(-180, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// bottom layer
	// along +y
	glPushMatrix();{
		glTranslatef(0, translate_value, -translate_value);
		glRotatef(90, 0, 1, 0);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along +x
	glPushMatrix();{
		glTranslatef(translate_value, 0, -translate_value);
		glRotatef(-90, 1, 0, 0);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along -x
	glPushMatrix();{
		glTranslatef(-translate_value, 0, -translate_value);
		glRotatef(-90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along -y
	glPushMatrix();{
		glTranslatef(0, -translate_value, -translate_value);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(-180, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// top layer
	// along +y
	glPushMatrix();{
		glTranslatef(0, translate_value, translate_value);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along +x
	glPushMatrix();{
		glTranslatef(translate_value, 0, translate_value);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along -x
	glPushMatrix();{
		glTranslatef(-translate_value, 0, translate_value);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();

	// along -y
	glPushMatrix();{
		glTranslatef(0, -translate_value, translate_value);
		glRotatef(90, 0, 1, 0);
		glRotatef(-180, 0, 0, 1);
		drawcylinder(sphere_radius, sphere_slices, 2);
	}
	glPopMatrix();
}


void drawSphere(double radius,int slices,int stacks){
	struct point pos[100][100];
	int i,j;
	double h,r;

	//generate pos
	for(i=0;i<=stacks;i++){
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++){
			pos[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			pos[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			pos[i][j].z=h;
		}
	}
	//draw quads using generated pos
	for(i=0;i<stacks;i++){
		for(j=0;j<slices;j++){
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(pos[i][j].x,pos[i][j].y,pos[i][j].z);
				glVertex3f(pos[i][j+1].x,pos[i][j+1].y,pos[i][j+1].z);
				glVertex3f(pos[i+1][j+1].x,pos[i+1][j+1].y,pos[i+1][j+1].z);
				glVertex3f(pos[i+1][j].x,pos[i+1][j].y,pos[i+1][j].z);
			}glEnd();
		}
	}
}


void DrawfullSphere(double radius,int slices,int stacks){
	glColor3f(1, 0, 0);

	// between +x and +y and +z
	glPushMatrix();{
		glTranslatef(translate_value, translate_value, translate_value);
		drawSphere(radius, slices, stacks);
	}
	glPopMatrix();
	
	// between +x and -y and +z
	glPushMatrix();{
	glTranslatef(translate_value, -translate_value, translate_value);
	glRotatef(-90, 0, 0, 1);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	// between -x and +y and +z
	glPushMatrix();{
	glTranslatef(-translate_value, translate_value, translate_value);
	glRotatef(90, 0, 0, 1);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	// between -x and -y and +z
	glPushMatrix();{
	glTranslatef(-translate_value, -translate_value, translate_value);
	glRotatef(180, 0, 0, 1);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	// between +x and +y and -z
	glPushMatrix();{
	glTranslatef(translate_value, translate_value, -translate_value);
	glRotatef(90, 0, 1, 0);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	//between +x and -y and -z
	glPushMatrix();{
	glTranslatef(translate_value, -translate_value, -translate_value);
	glRotatef(-180, 1, 0, 0);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	// between -x and +y and -z
	glPushMatrix();{
	glTranslatef(-translate_value, translate_value, -translate_value);
	glRotatef(180, 0, 1, 0);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();

	// between -x and -y and -z
	glPushMatrix();{
	glTranslatef(-translate_value, -translate_value, -translate_value);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	drawSphere(radius, slices, stacks);
	}
	glPopMatrix();
}

    
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':    // Look left
			l = Rotation(l, u, counterclockwise);
			r = Rotation(r, u, counterclockwise);
			break;

        case '2':    // Look right
			l = Rotation(l, u, clockwise);
			r = Rotation(r, u, clockwise);
            break;

        case '3':    // Look up
			l = Rotation(l, r, counterclockwise);
			u = Rotation(u, r, counterclockwise);
            break;

        case '4':    // Look down
			l = Rotation(l, r, clockwise);
			u = Rotation(u, r, clockwise);
            break;

        case '5':    // tilt clockwise
			r = Rotation(r, l, counterclockwise);
			u = Rotation(u, l, counterclockwise);
            break;

        case '6':    // tilt counter clockwise
			r = Rotation(r, l, clockwise);
			u = Rotation(u, l, clockwise);
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

	DrawfullSphere(sphere_radius, sphere_slices, sphere_stacks);
	DrawCube(sphere_radius);
	DrawCylinder();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;

	pos.x = 100;
	pos.y = 100;
	pos.z = 0;

	l.x = -1/sqrt(2.0);
	l.y = -1/sqrt(2.0);
	l.z = 0;

	r.x = -1/sqrt(2.0);
	r.y = 1/sqrt(2.0);
	r.z = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

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