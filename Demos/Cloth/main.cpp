//Port of Processing TraerPhysics library to C++
//
//Author: Muhammad Mobeen Movania
//Last Modified: 18 April 2014
//
//Cloth Demo adapted from http://murderandcreate.com/physics/

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

const int NODE_SIZE			= 10;
const int GRID_SIZE			= 20;

const float SPRING_STRENGTH	= 0.75f;
const float SPRING_DAMPING	= 0.5f;
  
ParticleSystem*		physics;
vector<Particle*>	particles;
	
int selected_index = -1;
	 
  

void OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{ 
		size_t N = physics->particles.size();
		
		for(size_t i=0;i<N;++i) {
			Particle* p = physics->particles[i];
			float dx = x - p->position.x;
			float dy = (height-y) - p->position.y;
			float dist = sqrtf(dx*dx + dy*dy);
			if(dist < NODE_SIZE) {
				selected_index = i;
				p->makeFixed();
			    break;
			}
		}
	} 

	if (s == GLUT_UP)
	{
		if(selected_index !=-1) {
			Particle* p = physics->particles[selected_index];
		
			p->position.x = (float)x;
			p->position.y = (float)(height-y);

			if(selected_index !=0 && selected_index!=(GRID_SIZE-1))
				p->makeFree();

			selected_index = -1;
		}
	}
}

void OnMouseMove(int x, int y)
{
	if(selected_index != -1) {
		Particle* p = physics->particles[selected_index]; 
		p->position.x = (float)x;
		p->position.y = (float)(height-y); 
	}
	glutPostRedisplay();
}


void OnInit() { 
	physics	= new ParticleSystem( -0.1f, 0.01f);  
	float gridStepX = (float)(( width/2.0f) / GRID_SIZE);
	float gridStepY = (float)(( height/2.0f) / GRID_SIZE);
	
	for( int i=0; i<GRID_SIZE; i++ ){
		for( int j=0; j<GRID_SIZE;j++ ){ 
			particles.push_back( physics->makeParticle( 1, j * gridStepX + (width / 4), height - (i * gridStepY + 20), 0 ) );
			if( j > 0 )
				physics->makeSpring( particles[i*GRID_SIZE + j - 1], particles[i*GRID_SIZE + j], SPRING_STRENGTH, SPRING_DAMPING, gridStepX );
		}
	}

	
	for( int j=0; j<GRID_SIZE; j++ ){
		for( int i=1; i<GRID_SIZE; i++ ){
			physics->makeSpring( particles[ (i - 1) * GRID_SIZE + j], particles[i*GRID_SIZE + j], SPRING_STRENGTH, SPRING_DAMPING, gridStepY );
		}
	}
	 
	int p2i = GRID_SIZE-1; 
	
	particles[0]->makeFixed();
	particles[p2i]->makeFixed();  
	 
	glPointSize(NODE_SIZE/2.0f);
}

void OnShutdown() {
	size_t N = particles.size();
	for(size_t i=0;i<N;++i) 
		physics->removeParticle(particles[i]);
	particles.clear(); 

	physics->clear();
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
			if(i == selected_index)
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
	glutCreateWindow("Traer Physics - Cloth Demo");
	

	glutReshapeFunc(OnReshape);
	glutDisplayFunc(OnRender);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutCloseFunc(OnShutdown);
	glutIdleFunc(OnIdle);

	OnInit();

	glutMainLoop();
} 