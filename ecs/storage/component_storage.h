#ifndef NP_ECS_COMPONENT_STORAGE_HPP
#define NP_ECS_COMPONENT_STORAGE_HPP

//Includes:
#include "components.hpp"
#include <assert.h>

namespace np_ecs
{
	struct ComponentArray
	{
	public:
		//Attributes:
		std::vector<IComponent*> components;

		//Constructor:
		ComponentArray() { }
		ComponentArray(unsigned int size) { components.resize(size); }

		//Methods:
		void add_component(entity_t entity, IComponent* component);

		bool has_entity(entity_t entity);

		void remove_component(entity_t entity);

		//Operators:
		IComponent* operator[] (entity_t entity);
	};
}

#endif //!NP_ECS_COMPONENT_STORAGE_HPP