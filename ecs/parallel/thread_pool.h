#ifndef NP_ECS_THREAD_POOL_HPP
#define NP_ECS_THREAD_POOL_HPP

//Includes:
#include<thread>
#include<vector>
#include<functional>
#include"queue.h"
#include"../logic/system.h"
//#include"thread_guard.h"

namespace np_ecs
{
	class ThreadPool
	{
	private:
		//Attributes:
		std::vector<std::thread> threads;
		thread_safe::Queue<std::function<Result()>> tasks;
		std::atomic<bool> active;

	public:
		//Constructor:
		ThreadPool() : active(true)
		{
			for (int i = 0; i < Size(); i++)
			{
				std::thread t(&ThreadPool::poll, this);
				threads.push_back(std::move(t));
			}
		}

		//Methods:
		const unsigned int Size() { return std::thread::hardware_concurrency(); }

		void poll_task(std::function<Result()> task)
		{
			tasks.Enqueue(task);
		}

		//Destructor:
		~ThreadPool()
		{
			//Sequential...
			while (!tasks.IsEmpty()) { tasks.Retrieve()(); }

			//Destruct thread objects.
			active = false;

			for (int i = 0; i < threads.size(); i++)
			{
				if (threads[i].joinable()) threads[i].join();
			}

			threads.clear();
			threads.resize(NULL);
		}

	private:
		//Methods:
		void poll()
		{
			while (active)
			{

				std::function<Result()> task;

				if (tasks.TryRetrieve(&task))
				{
					task();
				}

				std::this_thread::yield();
			}
		}
	};
}

#endif //!NP_ECS_THREAD_POOL_HPP 