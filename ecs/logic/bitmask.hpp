#ifndef NP_ECS_BITMASK_H
#define NP_ECS_BITMASK_H

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

namespace np_ecs
{
	//Constants:
	const unsigned int BIT_ONE = 1;
	const unsigned int BITS_PER_BYTE = 8;
	constexpr unsigned int BIT_TO_INDEX(unsigned int bit) { return bit / BITS_PER_BYTE; }
	constexpr unsigned int bit_to_decimal(unsigned int bit) { return BIT_ONE << bit; }

	template<size_t Size> class Bitmask
	{
	private:
		//Attributes:
		char* data;

	public:
		//Default constructor:
		Bitmask() { data = (char*)calloc(ceil(Size / (float)BITS_PER_BYTE), sizeof(char)); }

		//Methods:
		void set(unsigned int bit, bool value)
		{
			assert(BIT_TO_INDEX(bit) < Size);
			unsigned int mask = BIT_ONE << (bit % BITS_PER_BYTE);
			data[BIT_TO_INDEX(bit)] = value ? data[BIT_TO_INDEX(bit)] | mask : data[BIT_TO_INDEX(bit)] & (~mask);
		}

		bool has_bit(unsigned int bit)
		{
			assert(BIT_TO_INDEX(bit) < Size);
			return ((BIT_ONE << bit) & data[BIT_TO_INDEX(bit)]) > 0;
		}

		bool has_signature(Bitmask<Size>& bitmask)
		{
			for (int i = 0; i < ceil(Size / (float)BITS_PER_BYTE); i++) { if ((data[i] & bitmask.data[i]) != bitmask.data[i]) { return false; } }
			return true;
		}

		bool has_signature(unsigned int external)
		{
			return (get_decimal() & external) == external;
		}

		unsigned int get_decimal()
		{
			unsigned int value = 0;
			for (int i = 0; i < ceil(Size / (float)BITS_PER_BYTE); i++) { value += (unsigned int)data[i]; }
			return value;
		}

		void reset() { for (int i = 0; i < ceil(Size / (float)BITS_PER_BYTE); i++) { data[i] = 0; } }

		//Operators:
		bool operator==(Bitmask<Size>& external) { return get_decimal() == external.get_decimal(); }
		bool operator!=(Bitmask<Size>& external) { return get_decimal() != external.get_decimal(); }
		bool operator==(unsigned int external) { return get_decimal() == external; }
		bool operator!=(unsigned int external) { return get_decimal() != external; }
	};
}

#endif // !NP_ECS_BITMASK_H