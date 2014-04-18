//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//Box Demo adapted from http://murderandcreate.com/physics/

#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/freeglut.h>

const int width = 800; 
const int height = 600;

#include "..\..\TraerPhysics\src\ParticleSystem.h" 
#include "..\..\TraerPhysics\src\Particle.h"
#include "..\..\TraerPhysics\src\Spring.h"

using namespace traer::physics;

float bigBoxSize = 200;
float littleBoxSize = 8;

float xRotation = 0;
float yRotation = 0;

float gravityMagnitude = -1;
float bounceDamp = 0.5;

int numberOfLittleBoxes = 400;

float repulsionStrength = 5;
float repulsionMinimum = 5;

ParticleSystem*		physics;
GLfloat MV[16];
float gx = 0;
float gy = 0;
float gz = 0;

void setRotation( float xRotate, float yRotate ) {
	physics->setGravity(gx,	gy, gz);
	xRotation = xRotate;
	yRotation = yRotate; 
}

void OnMouseMove(int x, int y)
{ 
	float mx = (float)x;
	float my = (float)(height-y);
	setRotation( my, mx); 

	glutPostRedisplay();
}

float random(float minR, float maxR) {
	return ((maxR - minR) * ( (float)rand() / (float)RAND_MAX ) + minR);
}

void OnInit() { 
	physics	= new ParticleSystem(gravityMagnitude, 0.01f);  
	physics->setIntegrator(ParticleSystem::MODIFIED_EULER);

	srand(0); 

	for ( int i = 0; i < numberOfLittleBoxes; i++ ) {
		Particle* p = physics->makeParticle( 1.0f, 
			random( -0.45f*littleBoxSize, 0.45f*littleBoxSize ),
			random( -0.45f*littleBoxSize, 0.45f*littleBoxSize ),
			random( -0.45f*littleBoxSize, 0.45f*littleBoxSize ) ); 

		for ( int j = 0; j < i; j++ ) {
			Particle* q = physics->getParticle( j );
			physics->makeAttraction( p, q, -repulsionStrength, repulsionMinimum );
		}
	} 

	//setup lighting and material
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST); 
	 
	GLfloat ambient[4]={0,0,0,0};
	GLfloat diffuse[4]={1,1,1,1};
	GLfloat mat_diffuse[4]={0.5,0.5,0.5,1};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

	glDisable(GL_LIGHTING);
}


void OnShutdown() { 
	delete physics;
}

void Update() {
	physics->tick(); 
}

void DrawLittleBoxes() {

	for ( int i = 0; i < physics->numberOfParticles(); i++ )
	{
		float t = (float)i/(physics->numberOfParticles()-1);

		glPushMatrix();

		Particle* p = physics->getParticle( i );
		glTranslatef( p->position.x, p->position.y, p->position.z );
		glutSolidCube( littleBoxSize );

		glPopMatrix();
	}
}

void Bounce()
{
	float collisionPoint = 0.5f*( bigBoxSize - littleBoxSize );
	for ( int i = 0; i < physics->numberOfParticles(); i++ )
	{
		Particle* p = physics->getParticle( i );

		if ( p->position.x < -collisionPoint )
		{
			p->position.x = -collisionPoint;
			p->velocity.x = -p->velocity.x;
		}
		else
			if ( p->position.x > collisionPoint )
			{
				p->position.x =  collisionPoint;
				p->velocity.x = -bounceDamp*p->velocity.x;
			}

			if ( p->position.y < -collisionPoint )
			{
				p->position.y = -collisionPoint;
				p->velocity.y = -bounceDamp*p->velocity.y;
			}
			else
				if ( p->position.y > collisionPoint )
				{
					p->position.y = collisionPoint;
					p->velocity.y = -bounceDamp*p->velocity.y;
				}

				if ( p->position.z < -collisionPoint )
				{
					p->position.z = -collisionPoint;
					p->velocity.z = -bounceDamp*p->velocity.z;
				}
				else
					if ( p->position.z > collisionPoint )
					{
						p->position.z = collisionPoint;
						p->velocity.z = -bounceDamp*p->velocity.z;
					}
	} 
}



void Draw()
{       
	glTranslatef(0,0,-bigBoxSize*2);
	glRotatef(xRotation, 1,0,0 );
	glRotatef(yRotation, 0,1,0 );

	//get the Up axis in view space from the modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, MV);
	gx = -MV[1];
	gy = -MV[5];
	gz = -MV[9];

	glutWireCube(bigBoxSize);
	
	glEnable(GL_LIGHTING);
	DrawLittleBoxes();   
	glDisable(GL_LIGHTING);
}

void OnRender() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); 

	glLoadIdentity();
	Update();
	Bounce();

	Draw();

	glutSwapBuffers();
}


void OnReshape(int nw, int nh) {
	glViewport(0, 0, nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	gluPerspective(60.0f, (float)nw/ nh, 0.1, 1000);  
	glMatrixMode(GL_MODELVIEW);
}

void OnIdle() { 
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Traer Physics - Boxes");

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender); 
	glutPassiveMotionFunc(OnMouseMove);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
}
