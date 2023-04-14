#include "core.h"

void np_ecs::World::initialise()
{
	for (int i = 0; i < systems.size(); i++)
	{
		if (!systems[i]->parallel) systems[i]->start();
	}
}

void np_ecs::World::update()
{
	//Process tasks: adding and removing components from entities.

	if (!tasks.empty() && pool.all_systems_paused())
	{
		while (!tasks.empty())
		{
			ITask* task = tasks.front();
			task->process(this);

			tasks.pop();
		}

		pool.unpause();
	}



	//condition.notify_all();

	//Update systems.
	for (int i = 0; i < systems.size(); i++)
	{
		if (!systems[i]->parallel) systems[i]->update();
	}
}

void np_ecs::World::schedule_add_component(entity_t entity, IComponent* component, flag_t bit_flag)
{
	tasks.push(new AddComponentTask(entity, bit_flag, component));
}

void np_ecs::World::schedule_delete_component(entity_t entity, flag_t flag)
{
	tasks.push(new DeleteComponentTask(entity, flag));
}

void np_ecs::World::add_component(entity_t entity, IComponent* component, flag_t bit_flag)
{
	{
		assert(entity_allocator.validate(entity));

		//Register component.
		if (!signature.has_bit(bit_flag))
		{
			containers[bit_flag] = ComponentArray(component_max);
			signature.set(bit_flag, true);
		}

		//Add component signature to entity.
		entity_allocator.get(entity).set(bit_flag, true);
		containers[bit_flag].add_component(entity, component);

		//Add entity to relevant collections.
		for (int i = 0; i < systems.size(); i++)
		{
			if (systems[i]->signature == bit_flag)
			{
				emplace(systems[i]->entities, entity, 0, systems[i]->entities.size());
			}

			//Subset for entity with only the added component.
			else if (entity_allocator.signatures[entity].has_signature(systems[i]->signature))
			{
				emplace(systems[i]->entities, entity, 0, systems[i]->entities.size());
			}
		}
	}
}
