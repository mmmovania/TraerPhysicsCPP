#pragma once

#include "Vector.h"

namespace traer { namespace physics {

class Particle
{
public:  
  
    Particle( const float m=1);
	void setLocked( bool b ){ locked = b; };
	void makeFixed(){
		fixed = true; 
	}
	void makeFree() {
		fixed = false;
	}
    Vec3f position;
    Vec3f velocity;
    Vec3f force;
    float mass;
    float age;
    bool fixed;
	bool locked;
    
    void reset();
  
};

} } // namespace traer::physics