#ifndef NP_ECS_ENTITIES_H
#define NP_ECS_ENTITIES_H

//Includes:
#include  <queue>
#include "../logic/dyn_bitmask.hpp"
#include "../include/type_definitions.h"

namespace np_ecs
{
	class EntityAllocator
	{
	private:
		//Attributes:
		std::vector<DynBitmask> signatures;
		std::queue<entity_t> available_entities;

	public:
		//Methods:
		bool validate(entity_t entity) { return entity < signatures.size(); }

		entity_t allocate()
		{
			if (!available_entities.empty())
			{
				entity_t entity = available_entities.front();
				available_entities.pop();
				return entity;
			}

			signatures.push_back(DynBitmask());
			return signatures.size() - 1;
		}

		DynBitmask& get(unsigned int entity)
		{
			assert(validate(entity));

			return signatures[entity];
		}

		void free(unsigned int entity)
		{
			assert(validate(entity));

			available_entities.push(entity);
			signatures[entity].reset();
		}

		//Friends:
		friend class World;
	};

	/***************************************************************
	* Searching & Sorting Algorithms.
	*/

	static index_t partition(entity_t* components, index_t low, index_t high)
	{
		int pivot = components[high];

		int i = (low - 1);

		for (int j = low; j < high; j++)
		{
			if (components[j] <= pivot)
			{
				i++;

				int temp = components[i];
				components[i] = components[j];
				components[j] = temp;
			}
		}

		int temp = components[i + 1];
		components[i + 1] = components[high];
		components[high] = temp;

		return (i + 1);
	}

	static void quick_sort(entity_t* components, index_t low, index_t high)
	{
		if (low < high)
		{
			unsigned int pivot = partition(components, low, high);
			quick_sort(components, low, pivot - 1);
			quick_sort(components, pivot + 1, high);
		}
	}

	static bool contains(entity_t* components, entity_t value, index_t low, index_t high)
	{
		while (low != high)
		{
			unsigned int mid = (low + high) / 2;

			if (value == components[mid]) return true;

			else if (value > components[mid]) low = mid + 1;

			else high = mid - 1;
		}

		return false;
	}

	static void emplace(entity_t* components, entity_t value, index_t low, index_t high)
	{
		while (low != high)
		{
			unsigned int mid = (low + high) / 2;

			if (value == components[mid]) return;

			else if (value > components[mid]) low = mid + 1;

			else high = mid - 1;
		}

		components[low] = value;
	}

	///<summary> Utilises the binary search algorithm to determine the position of a value in a ordered vector of unsigned integers. </summary>
	static void emplace(std::vector<entity_t>& components, entity_t value, index_t low, index_t high)
	{
		while (low != high)
		{
			unsigned int mid = (low + high) / 2;

			if (value == components[mid]) return;

			else if (value > components[mid]) low = mid + 1;

			else high = mid - 1;
		}

		components.insert(components.begin() + low, value);
	}
}

#endif // !NP_ECS_ENTITIES_H