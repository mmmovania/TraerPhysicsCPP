//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//Tendrils Demo adapted from http://murderandcreate.com/physics/

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/freeglut.h>
#include <vector>
 
#include "T3ndril.h"

using namespace traer::physics;
 
const int width = 800; 
const int height = 600; 
 
ParticleSystem*		physics;
Particle *mouse; 
  
std::vector<T3ndril*> tendrils;
 
float greyer = 255;
bool drawing = false;
bool nothingDrawn = false;

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{  
		mouse->position.x = (float)x;
		mouse->position.y = (float)(height-y);
		tendrils.push_back(new T3ndril(physics, Vec3f(mouse->position.x, mouse->position.y,0), mouse));
		drawing = true;
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	} 

	if(s == GLUT_UP) { 
		drawing = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}
void OnMouseMove(int x, int y)
{ 
	((T3ndril*)tendrils.back())->addPoint( Vec3f(x, height-y, 0 ) );
	glutPostRedisplay();
	
}

void OnMouseMovePassive(int x, int y) {
	mouse->position.x = (float)x;
	mouse->position.y = (float)(height-y); 
	glutPostRedisplay();
	
}

float random(int minR, int maxR) {
	return ((maxR - minR) * ( (float)rand() / (float)RAND_MAX ) + minR);
}

void OnInit() { 
	physics	= new ParticleSystem(0.0, 0.05f);  
	mouse = physics->makeParticle();
	mouse->makeFixed(); 
  
	glClearColor(1,1,1,1);
}

void OnShutdown() { 
	physics->removeParticle(mouse); 
	delete physics;
}
 

void Update() {  
    physics->tick(); 
}
void DrawElastic( T3ndril* t )
{
  float lastStretch = 1;
  for ( size_t i = 0; i < t->particles.size()-1; ++i )
  {
    Vec3f firstPoint = ((Particle*)t->particles.at(i))->position;
    Vec3f firstAnchor =  i < 1 ? firstPoint : ((Particle*)t->particles.at( i-1 ))->position;
    Vec3f secondPoint = i+1 < t->particles.size() ? ((Particle*)t->particles.at( i+1 ))->position: firstPoint;
    Vec3f secondAnchor = i+2 < t->particles.size() ? ((Particle*)t->particles.at( i+2 ))->position : secondPoint;
      
     
    Spring* s = (Spring*)t->springs.at( i );
    float springStretch = 2.5f*s->restLength/s->currentLength();
          
    lastStretch = springStretch;
      
    glVertex2f(firstAnchor.x, firstAnchor.y);
	glVertex2f(firstPoint.x, firstPoint.y);
    glVertex2f(secondPoint.x, secondPoint.y);
    glVertex2f(secondAnchor.x, secondAnchor.y);
  }
}

void Draw()
{   
	glBegin(GL_LINES);
		glColor3ub( 255 - GLubyte(greyer), 255-GLubyte(greyer), 255-GLubyte(greyer) );
		for ( size_t i = 0; i < tendrils.size()-1; ++i ) {
			T3ndril* t = (T3ndril*)tendrils.at( i );
			DrawElastic( t );
		}
		glColor3f(0,0,0);
		if ( tendrils.size()-1 >= 0 )
			DrawElastic( (T3ndril*)tendrils.back());  
	glEnd();
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); 

	if(!drawing) {
		Update();
		if ( greyer < 255 )
			greyer *= 1.11111f;
		if ( greyer > 255 )
			greyer = 255;
	} else {
		if ( greyer >= 64 )
			greyer *= 0.9f;
	}
	Draw();

	glutSwapBuffers();
}


void OnReshape(int nw, int nh) {
	glViewport(0, 0, nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluOrtho2D(0,nw,0,nh);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OnIdle() { 
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Traer Physics - Tendrils");
	

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutPassiveMotionFunc(OnMouseMovePassive);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
}
 