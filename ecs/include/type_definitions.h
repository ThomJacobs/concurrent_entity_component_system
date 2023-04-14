#ifndef ECS_TYPE_DEFINITIONS
#define ECS_TYPE_DEFINITIONS

//Common includes:
#include <assert.h>
#include <vector>

//Defines:
#define HASH_CODE(type) typeid(type).hash_code()

namespace np_ecs
{
	//Type definitions:
	typedef unsigned int entity_t;
	typedef unsigned int component_id_t;
	typedef unsigned int index_t;
	typedef unsigned int hash_t;
	typedef unsigned int mask_t;
	typedef unsigned int denary_signature_t;
	typedef unsigned int flag_t;
}

#endif // !ECS_TYPE_DEFINITIONS