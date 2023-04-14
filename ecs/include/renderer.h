#pragma once
//Includes:
#include "core.h"

namespace np_ecs
{
	class ECS_Renderer
	{
	private:
		//Attributes:
		World* world;

	public:
		//Constructors:
		ECS_Renderer();
		ECS_Renderer(World* world);

		//Methods:
		unsigned int initialise();
		unsigned int update();
		unsigned int exit();
		World* get_world() { return world; }
	};
}