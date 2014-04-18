//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//RandomArboretum Demo adapted from http://murderandcreate.com/physics/

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/freeglut.h>
#include <limits>

const int width = 800; 
const int height = 600;
 
#include "..\..\TraerPhysics\src\ParticleSystem.h" 
#include "..\..\TraerPhysics\src\Particle.h"
#include "..\..\TraerPhysics\src\Spring.h"
 
using namespace traer::physics;
  
const float NODE_SIZE = 10;
const float EDGE_LENGTH = 20;
const float EDGE_STRENGTH = 0.2f;
const float SPACER_STRENGTH = 1000;

ParticleSystem*		physics;
float scale = 1;
float centroidX = 0;
float centroidY = 0;

float random(int minR, int maxR) {
	return ((maxR - minR) * ( (float)rand() / (float)RAND_MAX ) + minR);
}

void addSpacersToNode( Particle* p, Particle* r ) {
	for ( Particle* q: physics->particles ) {
		  if ( p != q && p != r )
			  physics->makeAttraction( p, q, -SPACER_STRENGTH, 20 );
	}
}

void makeEdgeBetween( Particle* a, Particle* b )
{
	physics->makeSpring( a, b, EDGE_STRENGTH, EDGE_STRENGTH, EDGE_LENGTH );
}

void addNode()
{ 
	Particle* p = physics->makeParticle();
	Particle* q = physics->getParticle( (int)random( 0, physics->numberOfParticles()-1) );
	while ( q == p )
		q = physics->getParticle( (int)random( 0, physics->numberOfParticles()-1) );
	addSpacersToNode( p, q );
	makeEdgeBetween( p, q );
	p->position.x = q->position.x + random( -1, 1 );
	p->position.y = q->position.y + random( -1, 1 );
}

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{  
		//mouse->position.x = (float)x;
		//mouse->position.y = (float)(height-y);
		addNode();	 
	} 

	if(s == GLUT_UP) { 
	}
}
void OnMouseMove(int x, int y)
{  
	addNode();
	glutPostRedisplay();
}


void initialize() {
	physics->clear();
	physics->makeParticle();
}

void OnInit() { 
	physics	= new ParticleSystem(0, 0.1f);  
	initialize(); 

	glPointSize((float)NODE_SIZE);
	glClearColor(1,1,1,1);
	glEnable(GL_POINT_SMOOTH);
}

void OnShutdown() {

	physics->clear();
	delete physics;
}

void Update() {
	physics->tick(); 
}

void DrawNetwork()
{      
    // draw particles
	glBegin(GL_POINTS);		
		for ( Particle* v:physics->particles)
		{			 
			glColor3ub(160,160,160);	
			glVertex2f(v->position.x, v->position.y);
		}
	glEnd();  

	//draw edges
	glBegin(GL_LINES);
		for ( Spring* e : physics->springs ) {
			Particle* a = e->getOneEnd();
			Particle* b = e->getTheOtherEnd();
			glVertex2f( a->position.x, a->position.y );
			glVertex2f( b->position.x, b->position.y );
		}		
	glEnd();
}

void updateCentroid() {
  float 
    xMax = -std::numeric_limits<float>::infinity(), 
    xMin =  std::numeric_limits<float>::infinity(), 
    yMin =  std::numeric_limits<float>::infinity(), 
    yMax = -std::numeric_limits<float>::infinity();

  for (Particle* p: physics->particles)
  { 
	xMax = std::max( xMax, p->position.x );
    xMin = std::min( xMin, p->position.x );
    yMin = std::min( yMin, p->position.y );
    yMax = std::max( yMax, p->position.y );
  }
  float deltaX = xMax-xMin;
  float deltaY = yMax-yMin;
  
  centroidX = xMin + 0.5f * deltaX;
  centroidY = yMin + 0.5f * deltaY;
  
  if ( deltaY > deltaX )
    scale = height/(deltaY+50);
  else
    scale = width/(deltaX+50);
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); 

	Update();
	
	if(physics->numberOfParticles() > 1)
		updateCentroid();

	glLoadIdentity();
	glTranslatef(width/2, height/2, 0);
	glScalef(scale, scale, scale);
	glTranslatef(-centroidX, -centroidY, 0);
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

void OnKey(unsigned char key, int x, int y) {
	if(key == ' ') {
		addNode();
	}
	if(key == 'c') {
		initialize();
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Traer Physics - Random Arboretum");
	

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender);
	glutKeyboardFunc(OnKey);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
}
 