#include "system.h"
#include "../include/core.h"

np_ecs::Result np_ecs::ISystem::frame()
{
	while (active)
	{
		//Wait for resources to be loaded.
		std::unique_lock<std::mutex> lock(mutex);
		local_world->condition.wait(lock, std::bind(&np_ecs::World::await, local_world));

		update();
	}

	return Result::Sucess;
}
