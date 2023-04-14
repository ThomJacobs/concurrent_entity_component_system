#include "component_storage.h"

void np_ecs::ComponentArray::add_component(entity_t entity, IComponent* component)
{
	assert(entity < components.size());
	components[entity] = component;
}

bool np_ecs::ComponentArray::has_entity(entity_t entity)
{
	return entity < components.size() || components[entity] != nullptr;
}

void np_ecs::ComponentArray::remove_component(entity_t entity)
{
	assert(entity < components.size() || components[entity] != nullptr);
	components[entity] = nullptr;
}

np_ecs::IComponent* np_ecs::ComponentArray::operator[](entity_t entity)
{
	assert(entity < components.size() && components[entity] != nullptr); 
	return components[entity];
}
