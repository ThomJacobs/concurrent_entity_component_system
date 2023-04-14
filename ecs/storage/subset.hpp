#ifndef NP_ECS_SUBSET_HPP
#define NP_ECS_SUBSET_HPP

//Includes:
#include <vector>
#include "../include/type_definitions.h"

namespace np_ecs
{
	//Forward delcaration.
	class World;

	class Subset
	{
	public:
		std::vector<entity_t> entities;

		//Methods:
		World* get_world() { return world; }

		//Operators:
		template<class T> 
		T& get(entity_t entity)
		{
			assert(world != nullptr);
			return world->get_component<T>(entity);
		}

	public:
		World* world;
	};
}

#endif //!NP_ECS_SUBSET_HPP