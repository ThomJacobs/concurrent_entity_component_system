#include "thread_pool_proto.h"

void np_ecs::ThreadPool::process(ISystem* system)
{
	//While the thread pool is active:
	while (active)
	{
		if (paused)
		{
			if (!system->paused) { system->paused = true;  count++; }
			continue;
		}
		else if (system->paused && !paused) { system->paused = false; }

		else if (system == nullptr) continue;

		system->update();
	}
}

np_ecs::ThreadPool::ThreadPool() : threads(std::vector<std::thread>())
{

}

bool np_ecs::ThreadPool::allocate(ISystem* system)
{
	if (threads.size() >= std::thread::hardware_concurrency()) return false;
	
	threads.push_back( std::thread( std::bind(&ThreadPool::process, this, system) ) );

	return true;
}

bool np_ecs::ThreadPool::all_systems_paused()
{
	paused = true;
	return count == threads.size();
}

void np_ecs::ThreadPool::force_pause()
{
	start_pause();
}

void np_ecs::ThreadPool::start_pause()
{
	count = 0;
	paused = true;
}

void np_ecs::ThreadPool::unpause()
{
	paused = false;
	count = 0;
}

np_ecs::ThreadPool::~ThreadPool()
{
	active = false;

	for (int i = 0; i < threads.size(); i++)
	{
		if (threads[i].joinable()) threads[i].join();
	}

	threads.clear();
}