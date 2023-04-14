#ifndef NP_ECS_THREAD_ALLOCATOR_HPP
#define NP_ECS_THREAD_ALLOCATOR_HPP

//Includes:
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <functional>
#include "../logic/system.h"

namespace np_ecs
{
	class ThreadAllocator
	{
	private:
		//Attributes:
		std::vector<std::thread*> threads;
		std::atomic<unsigned int> limit;
		std::mutex mutex;

	public:
		//Constructor:
		ThreadAllocator()
		{
			limit.store(std::thread::hardware_concurrency());
		}

		//Methods:
		bool assign(std::function<Result()> process)
		{
			std::lock_guard<std::mutex> lock(mutex);
			if (threads.size() >= limit - 1) return false;

			threads.push_back(new std::thread(process));

			return true;
		}

		//Destructor:
		~ThreadAllocator()
		{
			if (threads.size() <= 0) return;

			for (int i = 0; i < threads.size(); i++) { if (threads[i]->joinable()) threads[i]->join(); }
			threads.clear();
			threads.resize(NULL);
		}
	};
}

#endif //!NP_ECS_THREAD_ALLOCATOR_HPP