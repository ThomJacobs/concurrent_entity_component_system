#pragma once

//Includes:
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <iostream>
#include "../logic/system.h"

namespace np_ecs
{
	class ThreadPool
	{
	private:
		//Attributes:
		std::vector<std::thread> threads;
		unsigned int count = 0;

		//Methods:
		void process(ISystem* system);

	public:
		//Attributes:
		bool active = true;
		bool paused = false;

		//Constructor:
		ThreadPool();

		//Methods:
		bool allocate(ISystem* system);
		bool all_systems_paused();
		void force_pause();
		void start_pause();
		void unpause();

		//Destructor:
		~ThreadPool();
	};
}