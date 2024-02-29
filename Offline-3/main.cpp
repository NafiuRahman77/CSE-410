#include <bits/stdc++.h>

#include <GL/glut.h>
#include "header.h"
#include "bitmap_image.hpp"

using namespace std;

#define pi (2 * acos(0.0))
// ray tracing problem
//
int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;

Vector3D pos(0, 150, 50), u(0, 0, 1), r(-1, 0, 0), l(0, -1, 0);
vector<Object *> objects;
vector<PointLight> point_lights;
vector<SpotLight> spot_lights;

bitmap_image image;

int recursion_level, image_height, image_width;

void loadData()
{

    // load from scene.txt
    ifstream scene_file;
    scene_file.open("scene_test.txt");

    scene_file >> recursion_level;
    scene_file >> image_width;
    image_height = image_width;

    cout<<recursion_level<<endl;
    cout<<image_width<<endl;

    int number_of_objects;
    scene_file >> number_of_objects;
    cout << number_of_objects << endl;
    for (int i = 0; i < number_of_objects; i++)
    {
        string object_type;
        scene_file >> object_type;
        cout << object_type << endl;

        if (object_type == "sphere")
        {
            double x, y, z, radius;
            double color_r, color_g, color_b;
            double ambient, diffuse, specular, reflection, shininess;

            scene_file >> x >> y >> z >> radius;
            scene_file >> color_r >> color_g >> color_b;
            scene_file >> ambient >> diffuse >> specular >> reflection >> shininess;

            cout << x << " " << y << " " << z << " " << radius << endl;
            cout << color_r << " " << color_g << " " << color_b << endl;
            cout << ambient << " " << diffuse << " " << specular << " " << reflection << endl;
            cout << shininess << endl;
            cout<<endl;

            Object *s = new Sphere(Vector3D(x, y, z), radius);
            s->setColor(Color(color_r, color_g, color_b));
            s->setCoefficients(ambient, diffuse, specular, reflection);
            s->setShine(shininess);
            objects.push_back(s);
        }
        if (object_type == "triangle")
        {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            double color_r, color_g, color_b;
            double ambient, diffuse, specular, reflection, shininess;

            scene_file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            scene_file >> color_r >> color_g >> color_b;
            scene_file >> ambient >> diffuse >> specular >> reflection >> shininess;

            cout<<x1<<" "<<y1<<" "<<z1<<endl;
            cout<<x2<<" "<<y2<<" "<<z2<<endl;
            cout<<x3<<" "<<y3<<" "<<z3<<endl;
            cout<<color_r<<" "<<color_g<<" "<<color_b<<endl;
            cout<<ambient<<" "<<diffuse<<" "<<specular<<" "<<reflection<<endl;
            cout<<shininess<<endl;

            Object *t = new Triangle(Vector3D(x1, y1, z1), Vector3D(x2, y2, z2), Vector3D(x3, y3, z3));
            t->setColor(Color(color_r, color_g, color_b));
            t->setCoefficients(ambient, diffuse, specular, reflection);
            t->setShine(shininess);
            objects.push_back(t);
        }

        if (object_type == "general")
        {
            double A, B, C, D, E, F, G, H, I, J;
            double reference_x, reference_y, reference_z, length, width, height;
            double color_r, color_g, color_b;
            double ambient, diffuse, specular, reflection, shininess;

            scene_file >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            scene_file >> reference_x >> reference_y >> reference_z >> length >> width >> height;
            scene_file >> color_r >> color_g >> color_b;
            scene_file >> ambient >> diffuse >> specular >> reflection >> shininess;

            cout<<A<<" "<<B<<" "<<C<<" "<<D<<" "<<E<<" "<<F<<" "<<G<<" "<<H<<" "<<I<<" "<<J<<endl;
            cout<<reference_x<<" "<<reference_y<<" "<<reference_z<<" "<<length<<" "<<width<<" "<<height<<endl;
            cout<<color_r<<" "<<color_g<<" "<<color_b<<endl;
            cout<<ambient<<" "<<diffuse<<" "<<specular<<" "<<reflection<<endl;
            cout<<shininess<<endl;

            Object *g = new General(A, B, C, D, E, F, G, H, I, J);
            g->reference_point = Vector3D(reference_x, reference_y, reference_z);
            g->length = length;
            g->width = width;
            g->height = height;
            g->setColor(Color(color_r, color_g, color_b));
            g->setCoefficients(ambient, diffuse, specular, reflection);
            g->setShine(shininess);
            objects.push_back(g);
        }
    }

    Object *f = new Floor(400, 10);
	f->setCoefficients(0.5, 0.3, 0.25, 0.125);
    f->setShine(50);
    objects.push_back(f);

    int number_of_point_lights;
    scene_file >> number_of_point_lights;

    for (int i = 0; i < number_of_point_lights; i++)
    {
        double x, y, z, intensity_r, intensity_g, intensity_b;
        scene_file >> x >> y >> z >> intensity_r >> intensity_g >> intensity_b;
        PointLight pl(Vector3D(x, y, z), Color(intensity_r, intensity_g, intensity_b));
        point_lights.push_back(pl);
    }

    int number_of_spot_lights;
    scene_file >> number_of_spot_lights;

    for (int i = 0; i < number_of_spot_lights; i++)
    {
        double x, y, z, intensity_r, intensity_g, intensity_b, direction_x, direction_y, direction_z, angle;
        scene_file >> x >> y >> z >> intensity_r >> intensity_g >> intensity_b >> direction_x >> direction_y >> direction_z >> angle;
        SpotLight sl(PointLight(Vector3D(x, y, z), Color(intensity_r, intensity_g, intensity_b)), Vector3D(direction_x, direction_y, direction_z), angle);
        spot_lights.push_back(sl);
    }

    
	
}

double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;

void capture(){
    

    image.setwidth_height(image_width, image_height);
    image.set_all_channels(0, 0, 0);

    double plane_distance = (windowHeight / 2.0) / tan((viewAngle / 2.0) * (pi / 180.0));
    Vector3D topleft = pos + l * plane_distance - r * (windowWidth / 2.0) + u * (windowHeight / 2.0);

    double du = windowWidth / image_width;
    double dv = windowHeight / image_height;

    topleft = topleft + r * (du / 2.0) - u * (dv / 2.0);
    int nearest = -1;
    double t_min = 1000000000,t=0;
    for (int i = 0; i < image_width; i++)
    {
        for (int j = 0; j < image_height; j++)
        {
           // calculate curPixel using topleft,r,u,i,j,du,dv
            Vector3D curPixel = topleft + r * i * du - u * j * dv;
            Vector3D direction = curPixel - pos;
            direction = direction/sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            Ray ray(pos, direction);
            
            nearest = -1;
            t_min = -1;
            Color f(0,0,0);
            for (int k = 0; k < objects.size(); k++)
            {
                Color c(0,0,0);
                t = objects[k]->intersect_2(point_lights,spot_lights,objects,ray,c,1);


                if(t <= 0)
                {
                    continue;
                }
                //cout<<t<<endl;
                if(t_min < 0)
                {
                    t_min = t;
                    nearest = k;
                    f = c;
                }
                else if ( t < t_min)
                {
                    t_min = t;
                    nearest = k;
                    f = c;
                }
            }
            // if (nearest != -1)
            // {
            //     Color color = objects[nearest]->getColor();
            //     image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);
            // }
            if(nearest != -1)
            {
                image.set_pixel(i,j,f.r*255,f.g*255,f.b*255);
            }

        }

        
    }


    image.save_image("out.bmp");
    cout<<"Image saved"<<endl;

}

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

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    
    case '0':
    cout<<spot_lights.size()<<endl;
        capture();
        
        break;

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
        // cout<<r.x<<" "<<r.y<<" "<<r.z<<endl;

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
        pos = pos - l * 1;
        break;

    case GLUT_KEY_UP: // up arrow key, zoom in
        pos = pos + l * 1;
        break;

    case GLUT_KEY_RIGHT: // right arrow, move right
        pos = pos + r * 1;
        break;

    case GLUT_KEY_LEFT: // left arrow, move left
        pos = pos - r * 1;
        break;

    case GLUT_KEY_PAGE_UP: // page up, move up
        pos = pos + u * 1;
        break;

    case GLUT_KEY_PAGE_DOWN: // page down, move down
        pos = pos - u * 1;
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // draw the objects
    // Sphere s(Vector3D(0, 0, 20), 20);
    // s.setColor(Color(1, 0, 0));
    // s.draw();
    // 50 30 0
    // 70 60 0
    // 50 45 50
    // Triangle t(Vector3D(50, 30, 0), Vector3D(70, 60, 0), Vector3D(50, 45, 50));
    // t.setColor(Color(0, 1, 0));
    // t.draw();

    // Triangle t1(Vector3D(70, 60, 0), Vector3D(30, 60, 0), Vector3D(50, 45, 50));
    // t1.setColor(Color(1, 0, 0));
    // t1.draw();

    // Triangle t2(Vector3D(30, 60, 0), Vector3D(50, 30, 0), Vector3D(50, 45, 50));
    // t2.setColor(Color(0, 0, 1));
    // t2.draw();
    // Floor f(400, 10);
    // f.draw();

    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
    //cout<<objects.size()<<endl; 

    for(int i = 0; i < point_lights.size(); i++){
        point_lights[i].draw();
    }

    for(int i = 0; i < spot_lights.size(); i++){
        spot_lights[i].draw();
    }

    // ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void init()
{ // clear the screen
    glClearColor(0, 0, 0, 0);

    /************************
    / set-up projection here
    ************************/
    // load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    // initialize the matrix
    glLoadIdentity();

    // give PERSPECTIVE parameters
    gluPerspective(80, 1, 1, 1000.0);
    loadData();
    
}

void animate()
{
    // codes for any changes in Models, Camera
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
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