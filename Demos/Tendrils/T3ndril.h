#ifndef T3NDRIL_INCLUDED
#define T3NDRIL_INCLUDED
#pragma once
#include <vector>
#include "..\..\TraerPhysics\src\ParticleSystem.h" 
#include "..\..\TraerPhysics\src\Particle.h"  
#include "..\..\TraerPhysics\src\Spring.h"  

using namespace traer::physics;

class T3ndril
{
public:
	std::vector<Particle*> particles;
	std::vector<Spring*>   springs;
	ParticleSystem* physics;

	T3ndril( ParticleSystem* p, Vec3f firstPoint, Particle* followPoint ) {

		physics = p;

		Particle* firstParticle = p->makeParticle( 1.0f, firstPoint.x, firstPoint.y, firstPoint.z );
		particles.push_back( firstParticle );
		physics->makeSpring( followPoint, firstParticle, 0.1f, 0.1f, 5 );
	}

	void addPoint( Vec3f p )
	{
		Particle* thisParticle = physics->makeParticle( 1.0f, p.x, p.y, p.z );
		springs.push_back( physics->makeSpring(particles.back(),
			thisParticle, 
			1.0f,
			1.0f,
			glm::distance((particles.back())->position,thisParticle->position ) ) );
		particles.push_back( thisParticle );
	}
};

#endif