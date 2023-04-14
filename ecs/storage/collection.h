#ifndef NP_ECS_COLLECTION_H
#define NP_ECS_COLLECTION_H

//Includes:
#include "../allocators/entities.hpp"

namespace np_ecs
{
	struct Collection
	{
	public:
		//Attributes:
		std::vector<entity_t> entities;
	};
}

#endif // !NP_ECS_COLLECTION_H