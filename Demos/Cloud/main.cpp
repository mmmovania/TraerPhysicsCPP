//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//Cloud Demo adapted from http://murderandcreate.com/physics/

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/freeglut.h>
#include <vector>

const int width = 800; 
const int height = 600;
  
#include "..\..\TraerPhysics\src\ParticleSystem.h" 
#include "..\..\TraerPhysics\src\Particle.h"
#include "..\..\TraerPhysics\src\Spring.h"
 
using namespace traer::physics;
  
const int BALL_SIZE = 10;
const int HALF_BALL_SIZE = BALL_SIZE/2; 
const int MAX_PARTICLES = 1000;
const float ATTRACTION_RANGE = 100;  

ParticleSystem*		physics;
Particle *mouse; 

bool picked = false;

std::vector<Particle*> others(MAX_PARTICLES);
std::vector<Attraction*> attrs(MAX_PARTICLES);

 

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{  
		mouse->position.x = (float)x;
		mouse->position.y = (float)(height-y);
		picked = true;
	} 

	if(s == GLUT_UP) { 
		picked = false;
	}
}
void OnMouseMove(int x, int y)
{ 
	mouse->position.x = (float)x;
	mouse->position.y = (float)(height-y);
	glutPostRedisplay();
}

float random(int minR, int maxR) {
	return ((maxR - minR) * ( (float)rand() / (float)RAND_MAX ) + minR);
}

void OnInit() { 
	physics	= new ParticleSystem();  
	mouse = physics->makeParticle();
	mouse->makeFixed();
	 
	srand(0); 
	for ( int i = 0; i < MAX_PARTICLES; i++ ) {
		others[i] = physics->makeParticle( 1.0, random( 0, width ), random( 0, height ), 0 );
		attrs[i]  = physics->makeAttraction( mouse, others[i], 5000, 50 ); 
	} 
	 
	mouse->position.x = (float)(width/2);
	mouse->position.y = (float)(height/2);

	glPointSize((float)BALL_SIZE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1,1,1,1);
}

void OnShutdown() {
	for ( int i = 0; i < MAX_PARTICLES; i++ ) {
		physics->removeParticle(others[i]);
		physics->removeAttraction(attrs[i]);
	}
	physics->removeParticle(mouse); 

	others.clear();
	attrs.clear();
	delete physics;
}
 


void Update() { 
    physics->tick(); 
}

void DrawCircle(const float radius, const float x0=0, const float y0=0, const int numSegment=32) {
	float dTheta = float(2*M_PI)/numSegment;
	float x = 0;
	float y = 0;
	float theta = float(M_PI)/2.0f;

	glBegin(GL_LINE_LOOP);
		for(int i=0;i<numSegment; ++i) {
			x = x0 + radius * cosf(theta);
			y = y0 + radius * sinf(theta);
			glVertex2f(x,y);
			theta += dTheta;
		}
	glEnd();
}
void Draw()
{      
    // draw particles
	glBegin(GL_POINTS);		
	
		for ( Particle* v:physics->particles) 
		{ 
			if(v==mouse)
				if(picked)
					glColor3f(0,1,0);
				else
					glColor3f(1,0,0);
			else
				glColor4f(0.5,0.5,0.5,0.5);	
			glVertex2f(v->position.x, v->position.y);
		}
	glEnd();  

	//draw range of the mouse point
	glColor3f(1,0,0);
	DrawCircle(ATTRACTION_RANGE, mouse->position.x, mouse->position.y);
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); 

	Update();
	 
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
	glutCreateWindow("Traer Physics - Cloud");
	

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
}
 