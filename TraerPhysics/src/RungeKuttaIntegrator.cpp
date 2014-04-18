#include "RungeKuttaIntegrator.h"

namespace traer { namespace physics {

	RungeKuttaIntegrator::RungeKuttaIntegrator( ParticleSystem* system )
	{
		s = system;
	}
	
	void RungeKuttaIntegrator::allocateParticles()
	{
		while ( s->particles.size() > originalPositions.size() )
		{
			originalPositions.push_back( Vec3f() );
			originalVelocities.push_back( Vec3f() );
			k1Forces.push_back( Vec3f() );
			k1Velocities.push_back( Vec3f() );
			k2Forces.push_back( Vec3f() );
			k2Velocities.push_back( Vec3f() );
			k3Forces.push_back( Vec3f() );
			k3Velocities.push_back( Vec3f() );
			k4Forces.push_back( Vec3f() );
			k4Velocities.push_back( Vec3f() );
		}
	}
	
	void RungeKuttaIntegrator::step( const float &deltaT )
	{	
		allocateParticles();
		/////////////////////////////////////////////////////////
		// save original position and velocities
		
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{		
				originalPositions[i]= ( p->position );
				originalVelocities[i]=( p->velocity );
			}
			
			p->force=Vec3f(0,0,0);	// and clear the forces
		}
		
		////////////////////////////////////////////////////////
		// get all the k1 values
		 
		s->applyForces(); 

		// save the intermediate forces
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
				k1Forces[i]= p->force ;
				k1Velocities[i]=  p->velocity ;                
			}
			
			p->force=Vec3f(0,0,0);	// and clear the forces
		}
		
		////////////////////////////////////////////////////////////////
		// get k2 values
		 
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
				Vec3f originalPosition = originalPositions[i];
				Vec3f k1Velocity = k1Velocities[i];
				
                p->position=  originalPosition + (k1Velocity * (0.5f * deltaT)) ;
				
				Vec3f originalVelocity = originalVelocities[i];
				Vec3f k1Force = k1Forces[i];

				p->velocity=  originalVelocity + k1Force * 0.5f * deltaT / p->mass ;
			}
		}
		
		s->applyForces();

		// save the intermediate forces
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
				k2Forces[i]= ( p->force );
				k2Velocities[i]= ( p->velocity );                
			}
			
			p->force=Vec3f(0,0,0);	// and clear the forces now that we are done with them
		}
		
		
		/////////////////////////////////////////////////////
		// get k3 values
		
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
                Vec3f originalPosition = originalPositions[i];
                Vec3f k2Velocity = k2Velocities[i];
				
                p->position= ( originalPosition + k2Velocity * 0.5f * deltaT );
				
                Vec3f originalVelocity = originalVelocities[i];
                Vec3f k2Force = k2Forces[i];

                p->velocity= ( originalVelocity + k2Force * 0.5f * deltaT / p->mass );
			}
		}
        
		s->applyForces();
		
		// save the intermediate forces
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
				k3Forces[i]= ( p->force );
				k3Velocities[i]= ( p->velocity );                
			}
			
			p->force=Vec3f(0,0,0);	// and clear the forces now that we are done with them
		}
		
		
		//////////////////////////////////////////////////
		// get k4 values
		 
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
                Vec3f originalPosition = originalPositions[i];
				Vec3f k3Velocity = k3Velocities[i];
				
				p->position= ( originalPosition + k3Velocity * deltaT);
				
				Vec3f originalVelocity = originalVelocities[i];
				Vec3f k3Force = k3Forces[i];

				p->velocity= ( originalVelocity + k3Force * deltaT / p->mass );
			}
		}
		
		s->applyForces();

		// save the intermediate forces
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
			if ( !p->fixed )
			{
				k4Forces[i]= ( p->force );
				k4Velocities[i]= ( p->velocity );                
			}			
		}
		
		
		
		/////////////////////////////////////////////////////////////
		// put them all together and what do you get?
		
		for ( size_t i = 0; i < s->particles.size(); ++i )
		{
			Particle* p = s->particles[i];
            p->age += deltaT;
			 
			if ( !p->fixed )
			{
				// update position
				
                Vec3f originalPosition = originalPositions[i];
				Vec3f k1Velocity = k1Velocities[i];
				Vec3f k2Velocity = k2Velocities[i];
				Vec3f k3Velocity = k3Velocities[i];
				Vec3f k4Velocity = k4Velocities[i];
				
				p->position= ( originalPosition + deltaT / 6.0f * ( k1Velocity + 2.0f*k2Velocity + 2.0f*k3Velocity + k4Velocity ) );
				
				// update velocity
				
                Vec3f originalVelocity = originalVelocities[i];
				Vec3f k1Force = k1Forces[i];
				Vec3f k2Force = k2Forces[i];
				Vec3f k3Force = k3Forces[i];
				Vec3f k4Force = k4Forces[i];
				
				
                p->velocity= ( originalVelocity + deltaT / ( 6.0f * p->mass ) * ( k1Force + 2.0f*k2Force + 2.0f*k3Force + k4Force ) );

				 
			}
		}
		 
	}

} } // namespace traer::physics