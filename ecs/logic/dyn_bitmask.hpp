#ifndef NP_ECS_DYN_BITMASK_H
#define NP_ECS_DYN_BITMASK_H

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

namespace np_ecs
{
	//Constants:
	const unsigned int BIT_ONE = 1;
	const unsigned int BITS_PER_BYTE = 8;
	constexpr unsigned int BIT_TO_INDEX(unsigned int bit) { return bit / BITS_PER_BYTE; }
	constexpr unsigned int bit_to_decimal(unsigned int bit) { return BIT_ONE << bit; }

	class DynBitmask
	{
	private:
		//Attributes:
		std::vector<char> data;

	public:
		//Default constructor:
		DynBitmask() { data = std::vector<char>(); }

		//Methods:
		unsigned int most_significant_bit() { return data.size() * BITS_PER_BYTE; }

		void set(unsigned int bit, bool value)
		{
			unsigned int mask = BIT_ONE << (bit % BITS_PER_BYTE);
			if (mask > most_significant_bit()) data.resize(ceil((float)mask / (float)BITS_PER_BYTE));
			data[BIT_TO_INDEX(bit)] = value ? data[BIT_TO_INDEX(bit)] | mask : data[BIT_TO_INDEX(bit)] & (~mask);
		}

		bool has_bit(unsigned int bit)
		{
			if (data.empty() || bit > most_significant_bit()) return false; //Bit is out of range.
			return ((BIT_ONE << bit) & data[BIT_TO_INDEX(bit)]) > 0;
		}

		bool has_signature(DynBitmask& external)
		{
			if (data.empty() || external.data.empty()) return false;
			for (int i = 0; i < data.size(); i++) { if ((data[i] & external.data[i]) != external.data[i]) { return false; } }
			return true;
		}

		bool has_signature(unsigned int external)
		{
			return (get_decimal() & external) == external;
		}

		unsigned int get_decimal()
		{
			unsigned int value = 0;
			for (int i = 0; i < data.size(); i++) { value += (unsigned int)data[i]; }
			return value;
		}

		void reset() { data.clear(); data.resize(NULL); }

		//Operators:
		bool operator==(DynBitmask& external) { return get_decimal() == external.get_decimal(); }
		bool operator!=(DynBitmask& external) { return get_decimal() != external.get_decimal(); }
		bool operator==(unsigned int external) { return get_decimal() == external; }
		bool operator!=(unsigned int external) { return get_decimal() != external; }
	};
}

#endif // !NP_ECS_BITMASK_H