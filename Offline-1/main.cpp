#include <bits/stdc++.h>

#include <GL/glut.h>

using namespace std;

#define pi (2 * acos(0.0))

int drawgrid;
int drawaxes;

double sphereRadius = 0;
double translate_value = 20;

int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;
double CYLINDER_ANGLE = 70.5287794;

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

struct Point pos(2, 2, 2), u(0, 1, 0), r(2, 0, -2), l(-2, -2, -2);

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

void drawPyramid()
{

	for (int i = 0; i < 4; i++)
	{

		glPushMatrix();
		{
			glColor3f((i + 1) % 2, i % 2, 1.0f); // purple / cyan
			glRotatef(90 * i, 0, 1, 0);
			double scaleFactor = 1 - sqrt(2) * sphereRadius;
			double translateFactor = 1 / sqrt(3) * sphereRadius;
			glTranslatef(translateFactor, translateFactor, translateFactor);
			glScalef(scaleFactor, scaleFactor, scaleFactor);
			Triangle t1(Point(0, 0, 1), Point(0, 1, 0), Point(1, 0, 0));
			t1.draw();
		}
		glPopMatrix();
	}

	// rotate by 180
	glPushMatrix();
	{
		glRotatef(180, 1, 0, 0);
		for (int i = 0; i < 4; i++)
		{

			glPushMatrix();
			{
				glColor3f((i + 1) % 2, i % 2, 1.0f); // purple / cyan
				glRotatef(90 * i, 0, 1, 0);
				double scaleFactor = 1 - sqrt(2) * sphereRadius;
				double translateFactor = 1 / sqrt(3) * sphereRadius;
				glTranslatef(translateFactor, translateFactor, translateFactor);
				glScalef(scaleFactor, scaleFactor, scaleFactor);
				Triangle t1(Point(0, 0, 1), Point(0, 1, 0), Point(1, 0, 0));
				t1.draw();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void drawGrid()
{
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); // grey
		glBegin(GL_LINES);
		{
			for (int i = -8; i <= 8; i++)
			{
				if (i == 0)
					continue; // SKIP the MAIN axes

				// lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				// lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}
		glEnd();
	}
}

// https://www.songho.ca/opengl/gl_sphere.html
vector<vector<Point>> buildUnitPositiveX(int subdivision)
{
	const float DEG2RAD = acos(-1) / 180.0f;

	// compute the number of vertices per row, 2^n + 1
	int pointsPerRow = (int)pow(2, subdivision) + 1;
	vector<float> vertices;
	float n1[3]; // normal of longitudinal plane rotating along Y-axis
	float n2[3]; // normal of latitudinal plane rotating along Z-axis
	float v[3];	 // direction vector intersecting 2 planes, n1 x n2
	float a1;	 // longitudinal angle along Y-axis
	float a2;	 // latitudinal angle along Z-axis

	// rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
	for (unsigned int i = 0; i < pointsPerRow; ++i)
	{
		// normal for latitudinal plane
		// if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
		// therefore, it is rotating (0,1,0) vector by latitude angle a2
		a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
		n2[0] = -sin(a2);
		n2[1] = cos(a2);
		n2[2] = 0;

		// rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
		for (unsigned int j = 0; j < pointsPerRow; ++j)
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
			float scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			v[0] *= scale;
			v[1] *= scale;
			v[2] *= scale;

			// add a vertex into array
			vertices.push_back(v[0]);
			vertices.push_back(v[1]);
			vertices.push_back(v[2]);
		}
	}

	vector<Point> points_;
	for (int i = 0; i < vertices.size(); i += 3)
	{
		if (i + 2 < vertices.size()) // Check if the indices are within bounds
		{
			points_.push_back({vertices[i], vertices[i + 1], vertices[i + 2]});
		}
	}

	vector<vector<Point>> points;
	for (int i = 0; i < pointsPerRow; i++)
	{
		vector<Point> temp;
		for (int j = 0; j < pointsPerRow; j++)
		{
			temp.push_back(points_[i * pointsPerRow + j]);
		}
		points.push_back(temp);
	}

	return points;
}

// draw one segment of sphere
void drawSphereSegment()
{
	vector<vector<Point>> points = buildUnitPositiveX(4);

	for (int i = 0; i < points.size(); i++)
	{
		for (int j = 0; j < points[i].size(); j++)
		{
			glBegin(GL_QUADS);
			{
				if (i < points.size() - 1 && j < points[i].size() - 1)
				{
					glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
					glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
					glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
					glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				}
			}
			glEnd();
		}
	}
}

// draw all six segments of sphere
void drawSphere()
{

	double dist = 1 - sqrt(2) * sphereRadius;
	double translateFactor[6][3] = {{dist, 0, 0}, {0, 0, -dist}, {-dist, 0, 0}, {0, 0, dist}, {0, dist, 0}, {0, -dist, 0}};
	double rotFactor[2][3] = {{1, 0, 0}, {-1, 0, 0}};
	for (int i = 0; i < 6; i++)
	{
		glPushMatrix();
		glTranslatef(translateFactor[i][0], translateFactor[i][1], translateFactor[i][2]);
		if (i <= 3)
		{
			glColor3f((i + 1) % 2, (i % 2), 0);
			glRotatef(i * 90, 0, 1, 0);
		}
		else
		{
			glColor3f(0, 0, 1);
			glRotatef(90 * rotFactor[i - 4][0], 0, 0, 1);
		}
		glScalef(sphereRadius, sphereRadius, sphereRadius);
		drawSphereSegment();
		glPopMatrix();
	}
}

void drawCylinderSegment(float angle = CYLINDER_ANGLE, float radius = sphereRadius, float height = sqrt(2) - 2 * sphereRadius)
{
	const int numSegments =(int)(angle * 2 / 0.1) + 1;
	double t = (1 - sqrt(2) * radius)/2;

	vector<Point> points;
	glPushMatrix();
		glTranslatef(t, t, 0);
		glPushMatrix();
		glRotatef(45, 0, 0, 1);
		glTranslatef(0.0f, -height / 2, 0.0f);
		glRotatef(angle / 2, 0.0f, 1.0f, 0.0f);
		for (int i = 0; i <= numSegments; i++)
		{
			float x = radius * cos(i * 0.1);
			float z = radius * sin(i * 0.1);
			points.push_back(Point(x, 0.0f, z));
		}
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < points.size(); i++)
		{
			glVertex3f(points[i].x, points[i].y, points[i].z);
			glVertex3f(points[i].x, points[i].y + height, points[i].z);
		}
		glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawAllSegmentsCylinder()
{

	// yellow color
	glColor3f(1, 1, 0);

	// 8 cylinder segments (parralel to XY and YZ plane)

	for (int j = 0; j < 4; j++)
	{
		glPushMatrix();
		glRotatef(j * 90, 0, 1, 0);
		drawCylinderSegment();
		glPopMatrix();
	}

	glPushMatrix();
		glRotatef(180, 1, 0, 0);
		for (int j = 0; j < 4; j++)
		{
			glPushMatrix();
			glRotatef(j * 90, 0, 1, 0);
			drawCylinderSegment();
			glPopMatrix();
		}

		// 4 cylinder segments (parallel to XZ plane)

		glRotatef(270, 1, 0, 0);
		for (int j = 0; j < 4; j++)
		{
			glRotatef(j * 90, 0, 1, 0);
			glPushMatrix();
			drawCylinderSegment();
			glPopMatrix();
		}
	glPopMatrix();
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
		sphereRadius += 0.05;
		// triangleLength -= 0.05;
		if (sphereRadius > 1 / sqrt(2.0))
			sphereRadius = 1 / sqrt(2.0);
		// if (triangleLength < 0)
		// 	triangleLength = 0;
		break;

	case '.':

		sphereRadius -= 0.05;
		// triangleLength += 0.05 ;
		if (sphereRadius < 0)
			sphereRadius = 0;
		// if (triangleLength > 1 / sqrt(2.0))
		// 	triangleLength = 1 / sqrt(2.0);
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
	// drawGrid();

	// draw triangle

	drawPyramid();

	drawSphere();

	drawAllSegmentsCylinder();

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