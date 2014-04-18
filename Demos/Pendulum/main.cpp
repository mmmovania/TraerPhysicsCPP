//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//SimplePendulum Demo adapted from http://murderandcreate.com/physics/

#include <GL/freeglut.h>

const int width = 800; 
const int height = 600;


#include <sstream>
#include <vector>

#include "..\..\TraerPhysics\src\ParticleSystem.h" 
#include "..\..\TraerPhysics\src\Particle.h"
#include "..\..\TraerPhysics\src\Spring.h"
 
using namespace traer::physics;
using std::vector;

#define NODE_SIZE 10
const float SPRING_STRENGTH=0.1f;
const float SPRING_DAMPING=0.01f;
  
ParticleSystem*		physics;
Particle *p, *anchor;
Spring* s;
  
const int GRID_SIZE=3;
bool picked = false;

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{ 
		p->makeFixed();
		p->position.x = (float)x;
		p->position.y = (float)(height-y);
		picked = true;
	} 

	if(s == GLUT_UP) {
		p->makeFree();
		picked = false;
	}
}
void OnMouseMove(int x, int y)
{ 
	p->position.x = (float)x;
	p->position.y = (float)(height-y);
	glutPostRedisplay();
}


void OnInit() { 
	physics	= new ParticleSystem( -1.f, 0.05f);  
	p = physics->makeParticle(1.0, width*0.5f, height*0.5f,0);
	anchor = physics->makeParticle( 1.0, width*0.5f, height*0.5f, 0 );
	anchor->makeFixed(); 
	s = physics->makeSpring( p, anchor, 0.5, 0.5, 120 );
	glPointSize(NODE_SIZE);
}

void OnShutdown() {
	physics->removeParticle(p);
	physics->removeParticle(anchor);
	physics->removeSpring(s);
	delete physics;
}

void Update() {
    physics->tick(); 
}

void DrawNetwork()
{      
    // draw vertices
	glBegin(GL_POINTS);
		
		for ( int i = 0; i < physics->numberOfParticles(); ++i )
		{
			Particle* v = physics->getParticle( i );
			if(i==0 && picked)
				glColor3f(0,1,0);
			else
				glColor3f(0.63f, 0.63f, 0.63f);	
			glVertex2f(v->position.x, v->position.y);
		}
	glEnd();
    
    // draw springs 
	glBegin(GL_LINES);
		glColor3f(0.5,0.5,0.5);
		for ( int i = 0; i < physics->numberOfSprings(); ++i )
		{
			Spring* e = physics->getSpring( i );
			Particle* a = e->getOneEnd();
			Particle* b = e->getTheOtherEnd();
			glVertex2f(a->position.x, a->position.y);
			glVertex2f(b->position.x, b->position.y);                 
		}
	glEnd();
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); 

	Update();
	 
	DrawNetwork();

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
	glutCreateWindow("Traer Physics - Pendulum Demo");
	

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
}
 