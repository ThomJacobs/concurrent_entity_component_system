#ifndef NP_ECS_COMPONENT_HPP
#define NP_ECS_COMPONENT_HPP

//Includes:
#include <queue>
#include <unordered_map>
#include "entities.hpp"

namespace np_ecs
{

	class ComponentAllocator
	{
	private:
		//Attributes:
		std::queue<unsigned int> free;
		std::unordered_map<hash_t, index_t> registry;
		size_t current_id;

		//Methods:
		size_t next_identity()
		{
			if (free.empty()) return ++current_id;

			unsigned int id = free.front();
			free.pop();
			return id;
		}

	public:
		//Constructor:
		ComponentAllocator() : current_id(NULL) { }

		//Methods:
		template<typename T> bool is_registered()
		{
			return registry.find(HASH_CODE(T)) != registry.end();
		}

		template<typename T> size_t get_id()
		{
			//Type deriving IComponent has not been registered.
			if (!is_registered<T>())
			{
				registry.insert(std::pair<unsigned int, unsigned int>(HASH_CODE(T), next_identity()));
			}

			return registry[HASH_CODE(T)];
		}

		template<typename T> void free_identity()
		{
			unsigned int hash = HASH_CODE(T);

			if (!is_registered<T>()) return;

			free.push(registry[hash]);
			registry.erase(hash);
		}

		template<typename A, typename B> void swap_identities()
		{
			unsigned int a = HASH_CODE(A), b = HASH_CODE(B);

			unsigned int b_value = registry[b];
			registry[b] = registry[a];
			registry[a] = b_value;
		}
	};

}

#endif // !NP_ECS_COMPONENT_HPP