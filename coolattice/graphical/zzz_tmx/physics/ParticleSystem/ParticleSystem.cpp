#include "ParticleSystem.h"


namespace tmx { namespace physics { namespace particlesystem {

	bool is_source_dead(const Source& source)
	{
		return (glfwGetTime() - source.startingTime) > source.sourceLifeTime;
	}

}}}