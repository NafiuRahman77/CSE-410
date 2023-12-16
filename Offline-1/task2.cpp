#include <bits/stdc++.h>

#include <GL/glut.h>

using namespace std;

#define pi (2 * acos(0.0))

int drawgrid;
int drawaxes;

double sphereRadius = 1 / (2 * sqrt(2.0));
double translate_value = 20;

int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;
double CYLINDER_ANGLE = 70.5287794;

double ballRot = 1;

struct Point
{
	double x = 0, y = 0, z = 0;
	// constructor
	Point(double x = 0, double y = 0, double z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// copy constructor
	Point(const Point &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}
	// assignment operator
	Point &operator=(const Point &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
		return *this;
	}
	// destructor
	~Point() {}
	// addition operator
	Point operator+(const Point &p)
	{
		return Point(this->x + p.x, this->y + p.y, this->z + p.z);
	}
	// subtraction operator
	Point operator-(const Point &p)
	{
		return Point(this->x - p.x, this->y - p.y, this->z - p.z);
	}
	// scalar multiplication operator
	Point operator*(const double &d)
	{
		return Point(this->x * d, this->y * d, this->z * d);
	}
	// division operator
	Point operator/(const double &d)
	{
		return Point(this->x / d, this->y / d, this->z / d);
	}
	// dot product
	double operator*(const Point &p)
	{
		return this->x * p.x + this->y * p.y + this->z * p.z;
	}
	// cross product
	Point operator^(const Point &p)
	{
		return Point(this->y * p.z - this->z * p.y, this->z * p.x - this->x * p.z, this->x * p.y - this->y * p.x);
	}
};

class Triangle
{
public:
	Point a, b, c;
	Triangle(Point a, Point b, Point c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}

	void draw()
	{
		glPushMatrix();
		{

			glBegin(GL_TRIANGLES);
			{
				glVertex3f(a.x, a.y, a.z);
				glVertex3f(b.x, b.y, b.z);
				glVertex3f(c.x, c.y, c.z);
			}
			glEnd();
		}
		glPopMatrix();
	}
};

struct Point pos(3, 3, 5), u(0, 0, 1), r(-1 / sqrt(2.0), 1 / sqrt(2.0), 0), l(-1 / sqrt(2.0), -1 / sqrt(2.0), -2);
struct Point ballPos(0, 0, 0);
// perpendicular to ballPos
struct Point ballPerp(0, 0, 0);

void drawAxes()
{
	if (drawaxes == 1)
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
}

int gridSize = 8; // Adjust the size of the grid as needed
float squareSize = 1.0f;
// draw a 4 by 4 red bounding wall
//  Draw a 4x4 red bounding box as a wall
float boxSize = 8.0f * squareSize;
float boxHeight = 0.5f; // Adjust the height of the wall as needed

float minX = -boxSize / 2.0f;
float maxX = boxSize / 2.0f;
float minY = -boxSize / 2.0f;
float maxY = boxSize / 2.0f;

float minZ = 0.0f;
float maxZ = boxHeight;

void drawChessBoard()
{

	for (int i = -gridSize; i < gridSize; ++i)
	{
		for (int j = -gridSize; j < gridSize; ++j)
		{
			float x = i * squareSize;
			float y = j * squareSize;

			if ((i + j) % 2 == 0)
				glColor3f(1.0, 1.0, 1.0); // White square
			else
				glColor3f(0.0, 0.0, 0.0); // Black square

			glBegin(GL_QUADS);
			{
				glVertex3f(x, y, 0);
				glVertex3f(x + squareSize, y, 0);
				glVertex3f(x + squareSize, y + squareSize, 0);
				glVertex3f(x, y + squareSize, 0);
			}
			glEnd();
		}
	}

	glColor3f(1.0, 0.0, 0.0); // Red color

	glBegin(GL_QUADS);
	{
		// Front face
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);

		// Back face
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0);

		// Left face
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(minX, minY, maxZ);

		// Right face
		glVertex3f(maxX, minY, minZ);
		glVertex3f(maxX, maxY, minZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, minY, maxZ);

		// Top face
		glVertex3f(minX, maxY, minZ);
		glVertex3f(minX, maxY, maxZ);
		glVertex3f(maxX, maxY, maxZ);
		glVertex3f(maxX, maxY, minZ);

		// Bottom face
		glVertex3f(minX, minY, minZ);
		glVertex3f(minX, minY, maxZ);
		glVertex3f(maxX, minY, maxZ);
		glVertex3f(maxX, minY, minZ);
	}
	glEnd();
}

int sectorCount = 16;
int stackCount = 8;

vector<vector<Point>> vertices(sectorCount + 1, vector<Point>(stackCount + 1));

void drawSphere()
{

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		for (int j = 0; j < vertices[i].size() - 1; j++)
		{
			Point p1 = vertices[i][j];
			Point p2 = vertices[(i + 1)][j];
			Point p3 = vertices[i][(j + 1)];
			Point p4 = vertices[(i + 1)][(j + 1)];

			Triangle t1(p1, p3, p2);
			Triangle t2(p2, p3, p4);

			glColor3f(1, 0, 0);
			t1.draw();
			glColor3f(0, 1, 0);
			t2.draw();
		}
	}
}

double theta = 0, radius = 0;

Point ball_up(0, 0, 1);
Point ball_right(0, 1, 0);
Point ball_look(1, 0, 0);

Point RodriguezRotate(Point l, Point r, double angle)
{
	Point k = l ^ r;
	Point rotated = l * cos(angle) + k * sin(angle);
	return rotated;
}

double rollangle = 0;

int simulator = 0;
int simId = 0;
double simStartPosx = 0, simStartPosy = 0, simStartPosz = 0, simVel = 0;

void simScheduler();

void simulateBall(int a){
	if(simulator == 0)
		return;
	ballPos.x += 0.1 * cos(ballRot);
	ballPos.y += 0.1 * sin(ballRot);

	ball_look = ball_look + ballPos;

	rollangle += 0.5;

	glutTimerFunc(30, simulateBall, 0);
}

void simFlipX(int id)
{

	if(id != simId || simulator == 0)
		return;
	// reflect the ball's direction if it hits the wall

	ballRot = pi - ballRot;

	ball_look = RodriguezRotate(ball_look, ball_right, -pi);
	ball_up = RodriguezRotate(ball_up, ball_right, -pi);
    
	printf("ballRot: %lf\n", ballRot);
	simScheduler();
}

void simFlipY(int id)
{
	if(id != simId || simulator == 0)
		return;

	ballRot = -ballRot;

	ball_look = RodriguezRotate(ball_look, ball_up, -pi);
	ball_right = RodriguezRotate(ball_right, ball_up, -pi);

	simScheduler();
}

void simScheduler()
{
	simId++;
	simStartPosx = ballPos.x;
	simStartPosy = ballPos.y;
	simStartPosz = ballPos.z;
	simVel = 0.1;
	double A = ballRot, d = simVel / 360 * 2 * pi * sphereRadius;
	double dx = d * cos(A), dy = d * sin(A);

	// check whether tx or ty is greater, ie time to hit  the boundary wall
	double tx = (dx > 0) ? (4 - sphereRadius - simStartPosx) / dx : (-4 + sphereRadius - simStartPosx) / dx;
	double ty = (dy > 0) ? (4 - sphereRadius - simStartPosy) / dy : (-4 + sphereRadius - simStartPosy) / dy;

	cout << "tx: " << tx << " ty: " << ty << endl;

	if (tx < ty)
	{
		glutTimerFunc( tx/5, simFlipX, simId);
	}
	else
	{
		glutTimerFunc( ty/5, simFlipY, simId);
	}
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '1': // Look left
			  // l = Rotation(l, u, counterclockwise);
		l.x = l.x * cos(rotation_angle) - (l ^ u).x * sin(rotation_angle);
		l.y = l.y * cos(rotation_angle) - (l ^ u).y * sin(rotation_angle);
		l.z = l.z * cos(rotation_angle) - (l ^ u).z * sin(rotation_angle);

		// r = Rotation(r, u, counterclockwise);
		r.x = r.x * cos(rotation_angle) - (r ^ u).x * sin(rotation_angle);
		r.y = r.y * cos(rotation_angle) - (r ^ u).y * sin(rotation_angle);
		r.z = r.z * cos(rotation_angle) - (r ^ u).z * sin(rotation_angle);
		break;

	case '2': // Look right
			  // l = Rotation(l, u, clockwise);
		l.x = l.x * cos(rotation_angle) + (l ^ u).x * sin(rotation_angle);
		l.y = l.y * cos(rotation_angle) + (l ^ u).y * sin(rotation_angle);
		l.z = l.z * cos(rotation_angle) + (l ^ u).z * sin(rotation_angle);
		// r = Rotation(r, u, clockwise);
		r.x = r.x * cos(rotation_angle) + (r ^ u).x * sin(rotation_angle);
		r.y = r.y * cos(rotation_angle) + (r ^ u).y * sin(rotation_angle);
		r.z = r.z * cos(rotation_angle) + (r ^ u).z * sin(rotation_angle);

		break;

	case '3': // Look up
			  // l = Rotation(l, r, counterclockwise);
			  // u = Rotation(u, r, counterclockwise);
		l.x = l.x * cos(rotation_angle) - (l ^ r).x * sin(rotation_angle);
		l.y = l.y * cos(rotation_angle) - (l ^ r).y * sin(rotation_angle);
		l.z = l.z * cos(rotation_angle) - (l ^ r).z * sin(rotation_angle);

		u.x = u.x * cos(rotation_angle) - (u ^ r).x * sin(rotation_angle);
		u.y = u.y * cos(rotation_angle) - (u ^ r).y * sin(rotation_angle);
		u.z = u.z * cos(rotation_angle) - (u ^ r).z * sin(rotation_angle);
		break;

	case '4': // Look down
			  // l = Rotation(l, r, clockwise);
			  // u = Rotation(u, r, clockwise);
		l.x = l.x * cos(rotation_angle) + (l ^ r).x * sin(rotation_angle);
		l.y = l.y * cos(rotation_angle) + (l ^ r).y * sin(rotation_angle);
		l.z = l.z * cos(rotation_angle) + (l ^ r).z * sin(rotation_angle);

		u.x = u.x * cos(rotation_angle) + (u ^ r).x * sin(rotation_angle);
		u.y = u.y * cos(rotation_angle) + (u ^ r).y * sin(rotation_angle);
		u.z = u.z * cos(rotation_angle) + (u ^ r).z * sin(rotation_angle);

		break;

	case '5': // tilt clockwise
			  // r = Rotation(r, l, counterclockwise);
			  // u = Rotation(u, l, counterclockwise);
		r.x = r.x * cos(rotation_angle) - (r ^ l).x * sin(rotation_angle);
		r.y = r.y * cos(rotation_angle) - (r ^ l).y * sin(rotation_angle);
		r.z = r.z * cos(rotation_angle) - (r ^ l).z * sin(rotation_angle);

		u.x = u.x * cos(rotation_angle) - (u ^ l).x * sin(rotation_angle);
		u.y = u.y * cos(rotation_angle) - (u ^ l).y * sin(rotation_angle);
		u.z = u.z * cos(rotation_angle) - (u ^ l).z * sin(rotation_angle);
		break;

	case '6': // tilt counter clockwise
			  // r = Rotation(r, l, clockwise);
			  // u = Rotation(u, l, clockwise);

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

	case 'i':

	{
		ballPos.x += 0.1 * cos(ballRot);
		ballPos.y += 0.1 * sin(ballRot);

		ball_look = ball_look + ballPos;

		rollangle += 0.5;

		// reflect the ball's direction if it hits the wall
		if (ballPos.x > 4 - sphereRadius || ballPos.x < -4 + sphereRadius)
		{
			ballRot = pi - ballRot;

			ball_look = RodriguezRotate(ball_look, ball_right, -pi);
			ball_up = RodriguezRotate(ball_up, ball_right, -pi);
		}
		if (ballPos.y > 4 - sphereRadius || ballPos.y < -4 + sphereRadius)
		{
			ballRot = -ballRot;

			ball_look = RodriguezRotate(ball_look, ball_up, -pi);
			ball_right = RodriguezRotate(ball_right, ball_up, -pi);
		}

		break;
	}

	case 'k':
	{
		ballPos.x -= 0.1 * cos(ballRot);
		ballPos.y -= 0.1 * sin(ballRot);

		ball_look = ball_look + ballPos;

		rollangle -= 0.5;

		// reflect the ball's direction if it hits the wall
		if (ballPos.x > 4 - sphereRadius || ballPos.x < -4 + sphereRadius)
		{
			ballRot = pi - ballRot;

			ball_look = RodriguezRotate(ball_look, ball_right, -pi);
			ball_up = RodriguezRotate(ball_up, ball_right, -pi);
		}
		if (ballPos.y > 4 - sphereRadius || ballPos.y < -4 + sphereRadius)
		{
			ballRot = -ballRot;

			ball_look = RodriguezRotate(ball_look, ball_up, pi);
			ball_right = RodriguezRotate(ball_right, ball_up, pi);
		}

		
		break;
	}
	case 'j':
	{
		ballRot += 1;
		// if (ballRot > 2 * pi)
		// 	ballRot = 0;

		ball_look = RodriguezRotate(ball_look, ball_up, -1);
		ball_right = RodriguezRotate(ball_right, ball_up, -1);

		if(simulator == 1){
			simScheduler();
		}


		break;
	}

	case 'l':
		printf("l pressed\n");

		ballRot -= 1;
		// if (ballRot > 2 * pi)
		// 	ballRot = 0;

		ball_look = RodriguezRotate(ball_look, ball_up, 1);
		ball_right = RodriguezRotate(ball_right, ball_up, 1);

		if(simulator == 1){
			simScheduler();
		}


		break;
	//space bar	;
	case 32:
		printf("space pressed\n");
		simulator = 1- simulator;
		if(simulator == 1){
			simulateBall(0);
			simScheduler();
		}
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
		if (translate_value > 0)
		{
			sphereRadius += 5;
			translate_value -= 5;
		}
		break;

	case GLUT_KEY_END:
		if (sphereRadius > 0)
		{
			sphereRadius -= 5;
			translate_value += 5;
		}
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
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawgrid = 1 - drawgrid;
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}

// draw a thin pointy arrow
void drawArrow()
{
	glPushMatrix();
	// glRotatef(90, 0, 1, 0);
	glPushMatrix();
	{
		glTranslatef(0, 0, 0.5);
		glScalef(0.05, 0.05, 1);
		glutSolidCube(0.9);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0, 0, 1);
		glutSolidCone(0.1, 0.4, 10, 10);
	}
	glPopMatrix();

	glPopMatrix();
}

priority_queue<double> pq;
// event driven simulation

void rollBall()
{
	ballPos.x += 0.1 * cos(ballRot);
	ballPos.y += 0.1 * sin(ballRot);

	ball_look = ball_look + ballPos;

	rollangle += 0.5;

	// reflect the ball's direction if it hits the wall
	if (ballPos.x > 2 - sphereRadius || ballPos.x < -2 + sphereRadius)
	{
		ballRot = pi - ballRot;

		ball_look = RodriguezRotate(ball_look, ball_right, -pi);
		ball_up = RodriguezRotate(ball_up, ball_right, -pi);
	}
	if (ballPos.y > 2 - sphereRadius || ballPos.y < -2 + sphereRadius)
	{
		ballRot = -ballRot;

		ball_look = RodriguezRotate(ball_look, ball_up, -pi);
		ball_right = RodriguezRotate(ball_right, ball_up, -pi);
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

	// add objects
	drawAxes();
	drawChessBoard();

	glPushMatrix();
	{
		glTranslatef(ballPos.x, ballPos.y, ballPos.z);
		glRotatef(rollangle * 180 / pi, ball_right.x, ball_right.y, ball_right.z);
		drawSphere();
	}
	glPopMatrix();

	// move arrow along the ball's direction
	glPushMatrix();
	{
		glTranslatef(ballPos.x, ballPos.y, ballPos.z);
		glRotatef(ballRot * 180 / pi, 0, 0, 1);
		glRotatef(90, 0, 1, 0);
		drawArrow();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(ballPos.x, ballPos.y, ballPos.z);
		drawArrow();
	}
	glPopMatrix();

	// rollBall();
	// ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void init()
{
	// codes for initialization
	drawgrid = 1;
	drawaxes = 1;

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
	// near distance
	// far distance

	float sectorStep = 2 * pi / sectorCount;
	float stackStep = pi / stackCount;

	float sectorAngle, stackAngle;

	for (int i = 0; i <= sectorCount; i++)
	{
		sectorAngle = i * sectorStep;
		for (int j = 0; j <= stackCount; j++)
		{
			double theta1 = sectorStep * i;
			// double theta2 = sectorStep * (i + 1);
			double phi1 = pi / 2.0 - stackStep * j;
			// double phi2 = pi / 2.0 - stackStep * (j + 1);

			Point p(sphereRadius * cos(phi1) * cos(theta1), sphereRadius * cos(phi1) * sin(theta1), sphereRadius * sin(phi1));

			vertices[i].push_back(p);
		}
	}
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
	glutIdleFunc(animate);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop(); // The main loop of OpenGL

	return 0;
}