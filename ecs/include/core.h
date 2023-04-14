#ifndef NP_ECS_CORE_H
#define NP_ECS_CORE_H

//Includes:
#include <unordered_map>
#include "../allocators/components.hpp"
#include "../parallel/thread_pool_proto.h"
#include "../storage/components.hpp"
#include <algorithm>
#include "../parallel/thread_allocator.hpp"
#include "../storage/component_storage.h"
#include "record.h"
#include <mutex>

namespace np_ecs
{
	class World
	{
	private:
		//Attributes:
		unsigned int entity_max, component_max;
		EntityAllocator entity_allocator;
		DynBitmask signature;
		std::vector<ComponentArray> containers;
		std::vector<ISystem*> systems;
		std::queue<ITask*> tasks;
		ThreadAllocator thread_allocator;
		ThreadPool pool;
		std::mutex mutex;

	public:
		//For testing purposes only.
		std::condition_variable condition;

	public:
		//Constructors:
		World() : entity_max(NULL), component_max(NULL) { }
		World(unsigned int max_components, unsigned int max_entities) : entity_max(max_entities), component_max(max_components) { containers.resize(max_components); }

		//Methods:
		void initialise();
		void update();
		bool await() { return tasks.empty(); }

		template<typename S> ISystem* register_system(DynBitmask bitmask, bool parallel = false)
		{
			assert(validate_system(S));

			ISystem* s = (ISystem*) new S();
			s->local_world = this;
			s->signature = bitmask.get_decimal();

			//Add any entities with components to the system.
			for (int i = 0; i < entity_allocator.signatures.size(); i++)
			{
				if (entity_allocator.signatures[i].has_signature(bitmask))
				{
					emplace(s->entities, i, 0, s->entities.size());
				}
			}

			if (parallel && pool.allocate(s)) s->parallel = true;

			s->start();
			systems.push_back(s);

			return s;
		}

		void schedule_add_component(entity_t entity, IComponent* component, flag_t bit_flag);
		void schedule_delete_component(entity_t entity, flag_t flag);

		IComponent* get_component(entity_t entity, flag_t flag)
		{
			return containers[flag][entity];
		}

		template<typename T> T get_value(entity_t entity, flag_t flag = get_bit_flag<T>()) { return *((T*) containers[flag][entity]); }
		template<typename T> T& get_ref(entity_t entity, flag_t flag = get_bit_flag<T>()) { return *((T*)containers[flag][entity]); }

		template<typename T> IComponent* get_component(entity_t entity)
		{
			size_t flag = get_bit_flag<T>();
			return containers[flag][entity];
		}

		template<typename T> void add_component(entity_t entity, IComponent* component)
		{
			add_component(entity, component, get_bit_flag<T>());
		}

		void add_component(entity_t entity, IComponent* component, flag_t bit_flag);

		void delete_component_immediate(entity_t entity, flag_t flag)
		{
			assert(entity_allocator.validate(entity) && signature.has_bit(flag));
			containers[flag].remove_component(entity);

			//Remove entity from relevant collections.
			for (int i = 0; i < systems.size(); i++)
			{
				if (systems[i]->signature == flag)
				{
					systems[i]->entities.erase(std::find(systems[i]->entities.begin(), systems[i]->entities.end(), entity));
				}

				//Subset for entity with only the added component.
				else if (entity_allocator.signatures[entity].has_signature(systems[i]->signature))
				{
					systems[i]->entities.erase(std::find(systems[i]->entities.begin(), systems[i]->entities.end(), entity));
				}
			}
		}

		entity_t generate_entity() { return entity_allocator.allocate(); }
	};
}

#endif // !NP_ECS_CORE_H