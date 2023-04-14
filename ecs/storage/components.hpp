#ifndef NP_ECS_COMPONENTS_HPP
#define NP_ECS_COMPONENTS_HPP

//Includes:
#include "../allocators/components.hpp"
#include "../logic/dyn_bitmask.hpp"

namespace np_ecs
{
	struct IComponent
	{

	};

	static unsigned int flag_zero = 0;
	template<typename T> static unsigned int get_bit_flag()
	{
		static unsigned int flag = flag_zero++;
		return flag;
	}

	template<typename T> static DynBitmask get_bitmask()
	{
		DynBitmask b;
		b.set(get_bit_flag<T>(), true);
		return b;
	}

#define component_signature(type) 1 << bit_flag<type>()
#define hash_code(type) typeid(type).hash_code()

	template<typename type, typename... types> DynBitmask get_architype()
	{
		DynBitmask i;
		i.set(get_bit_flag<type>(), true);

		([&]
			{
				i.set(get_bit_flag<types>(), true);
			} (), ...);

		return i;
	}
}

#endif // ! NP_ECS_COMPONENTS_HPP