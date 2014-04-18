
#include "Particle.h"

namespace traer { 
	namespace physics {
		Particle::Particle( const float m )
		{
			mass = m;
			age = 0;
			fixed = false;
			locked = false;
			position=Vec3f(0,0,0);
			velocity=Vec3f(0,0,0);
			force=Vec3f(0,0,0);
		}

		void Particle::reset()
		{
			mass = 1.0f;
			age = 0;
			fixed = false;
			locked = false;
			position=Vec3f(0,0,0);
			velocity=Vec3f(0,0,0);
			force=Vec3f(0,0,0);
		}
	}
} // namespace traer::physics