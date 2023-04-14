#include "record.h"
#include "core.h"

void np_ecs::DeleteComponentTask::process(World* world)
{
	world->delete_component_immediate(entity, component_flag);
}

void np_ecs::AddComponentTask::process(World* world)
{
	world->add_component(entity, component, component_flag);
}

np_ecs::ITask::ITask(entity_t entity, flag_t flag, IComponent* component) : entity(entity), component_flag(flag), component(component)
{

}
