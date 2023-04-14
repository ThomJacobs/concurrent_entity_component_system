#ifndef NP_ECS_SYSTEM_H
#define NP_ECS_SYSTEM_H

//Includes:
#include "../include/type_definitions.h"
#include <thread>
#include <set>
#include <functional>
#include <mutex>

//Defines:
#define validate_system(type) std::is_base_of<ISystem, type>::value 

namespace np_ecs
{
	//Forward declaration.
	class World;

	//Enums.
	enum class Result { Sucess, Failure, Unexpected};

	class ISystem
	{
	public:
		//Attributes:
		std::vector<entity_t> entities;
		World* local_world;
		bool parallel = false;
		bool active = true;
		bool paused = false;
		unsigned int signature = NULL;
		std::mutex mutex;

	public:
		//Constructors:
		ISystem() : local_world(nullptr), entities(std::vector<entity_t>()) { }
		ISystem(World* world) : local_world(world), entities(std::vector<entity_t>()) { }

		//Methods:
		World* world() { return local_world; }

		//Abstract methods:
		void process_entities()
		{

		}

		Result frame();

		virtual Result update() = 0;
		virtual Result start() = 0;

		//Friends:
		friend class World;
	};

	class SystemArray
	{

	};

	/*
	class IParallelSystem : ISystem
	{
	private:
		//Attributes:
		bool active;
		std::condition_variable condition;
		std::mutex mutex;

	public:
		//Attributes:
		bool* paused;

		//Methods:
		virtual Result update() = 0;
		virtual Result start() = 0;
		bool is_unpaused() { return !*paused; }

		void frame()
		{
			while (active)
			{
				//Wait before updating?
				std::unique_lock<std::mutex> lock(mutex);
				condition.wait(lock, std::bind(&IParallelSystem::is_unpaused, this));

				update();
			}
		}
	};
	*/
}

#endif // !NP_ECS_SYSTEM_H