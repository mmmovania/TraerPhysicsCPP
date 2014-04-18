#pragma once

#include <vector>
#include "Vector.h"
#include "Integrator.h"
#include "ParticleSystem.h"

namespace traer { namespace physics {

class RungeKuttaIntegrator : public Integrator
{

public:

	std::vector<Vec3f> originalPositions;
	std::vector<Vec3f> originalVelocities;
	std::vector<Vec3f> k1Forces;
	std::vector<Vec3f> k1Velocities;
	std::vector<Vec3f> k2Forces;
	std::vector<Vec3f> k2Velocities;
	std::vector<Vec3f> k3Forces;
	std::vector<Vec3f> k3Velocities;
	std::vector<Vec3f> k4Forces;
	std::vector<Vec3f> k4Velocities;
	
	ParticleSystem* s;
	
	RungeKuttaIntegrator( ParticleSystem* s );
	
	void allocateParticles();
	
	void step( const float &deltaT );

};

} } // namespace traer::physics