///////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2014 G-Truc Creation (www.g-truc.net)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2011-05-03
// Updated : 2011-05-03
// Licence : This source is under MIT licence
// File    : test/core/func_integer.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <glm/integer.hpp>
#include <glm/gtc/vec1.hpp>
#include <vector>
#include <ctime>
#include <cstdio>

enum result
{
	SUCCESS,
	FAIL,
	ASSERT,
	STATIC_ASSERT
};

namespace bitfieldInsert
{
	template <typename genType, typename sizeType>
	struct type
	{
		genType		Base;
		genType		Insert;
		sizeType	Offset;
		sizeType	Bits;
		genType		Return;
	};

	typedef type<glm::uint, glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0xff000000, 0x0000ff00,  8,  8, 0xff00ff00},
		{0xffff0000, 0x0000ffff, 16, 16, 0x00000000},
		{0x0000ffff, 0xffff0000, 16, 16, 0xffffffff},
		{0x00000000, 0xffffffff,  0, 32, 0xffffffff},
		{0x00000000, 0xffffffff,  0,  0, 0x00000000}
	};

	int test()
	{
		int Error = 0;
		glm::uint count = sizeof(Data32) / sizeof(typeU32);
		
		for(glm::uint i = 0; i < count; ++i)
		{
			glm::uint Return = glm::bitfieldInsert(
				Data32[i].Base,
				Data32[i].Insert,
				Data32[i].Offset,
				Data32[i].Bits);

			Error += Data32[i].Return == Return ? 0 : 1;
		}
		
		return Error;
	}
}//bitfieldInsert

namespace bitfieldExtract
{
	template <typename genType, typename sizeType>
	struct type
	{
		genType		Value;
		sizeType	Offset;
		sizeType	Bits;
		genType		Return;
		result		Result;
	};

	typedef type<glm::uint, glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0xffffffff, 0,32, 0xffffffff, SUCCESS},
		{0xffffffff, 8, 0, 0x00000000, SUCCESS},
		{0x00000000, 0,32, 0x00000000, SUCCESS},
		{0x0f0f0f0f, 0,32, 0x0f0f0f0f, SUCCESS},
		{0x00000000, 8, 0, 0x00000000, SUCCESS},
		{0x80000000,31, 1, 0x00000001, SUCCESS},
		{0x7fffffff,31, 1, 0x00000000, SUCCESS},
		{0x00000300, 8, 8, 0x00000003, SUCCESS},
		{0x0000ff00, 8, 8, 0x000000ff, SUCCESS},
		{0xfffffff0, 0, 5, 0x00000010, SUCCESS},
		{0x000000ff, 1, 3, 0x00000007, SUCCESS},
		{0x000000ff, 0, 3, 0x00000007, SUCCESS},
		{0x00000000, 0, 2, 0x00000000, SUCCESS},
		{0xffffffff, 0, 8, 0x000000ff, SUCCESS},
		{0xffff0000,16,16, 0x0000ffff, SUCCESS},
		{0xfffffff0, 0, 8, 0x00000000, FAIL},
		{0xffffffff,16,16, 0x00000000, FAIL},
		//{0xffffffff,32, 1, 0x00000000, ASSERT}, // Throw an assert 
		//{0xffffffff, 0,33, 0x00000000, ASSERT}, // Throw an assert 
		//{0xffffffff,16,16, 0x00000000, ASSERT}, // Throw an assert 
	};

	int test()
	{
		int Error = 0;

		glm::uint count = sizeof(Data32) / sizeof(typeU32);

		for(glm::uint i = 0; i < count; ++i)
		{
			glm::uint Return = glm::bitfieldExtract(
				Data32[i].Value, 
				Data32[i].Offset, 
				Data32[i].Bits);
			
			bool Compare = Data32[i].Return == Return;

			if(Data32[i].Result == SUCCESS && Compare)
				continue;
			else if(Data32[i].Result == FAIL && !Compare)
				continue;

			Error += 1;
		}

		return Error;
	}
}//extractField

namespace bitfieldReverse
{
/*
	GLM_FUNC_QUALIFIER unsigned int bitfieldReverseLoop(unsigned int v)
	{
		unsigned int Result(0);
		unsigned int const BitSize = static_cast<unsigned int>(sizeof(unsigned int) * 8);
		for(unsigned int i = 0; i < BitSize; ++i)
		{
			unsigned int const BitSet(v & (static_cast<unsigned int>(1) << i));
			unsigned int const BitFirst(BitSet >> i);
			Result |= BitFirst << (BitSize - 1 - i);
		}
		return Result;
	}

	GLM_FUNC_QUALIFIER glm::uint64_t bitfieldReverseLoop(glm::uint64_t v)
	{
		glm::uint64_t Result(0);
		glm::uint64_t const BitSize = static_cast<glm::uint64_t>(sizeof(unsigned int) * 8);
		for(glm::uint64_t i = 0; i < BitSize; ++i)
		{
			glm::uint64_t const BitSet(v & (static_cast<glm::uint64_t>(1) << i));
			glm::uint64_t const BitFirst(BitSet >> i);
			Result |= BitFirst << (BitSize - 1 - i);
		}
		return Result;
	}
*/
	template <typename T, glm::precision P, template <typename, glm::precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<T, P> bitfieldReverseLoop(vecType<T, P> const & v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitfieldReverse' only accept integer values");

		vecType<T, P> Result(0);
		T const BitSize = static_cast<T>(sizeof(T) * 8);
		for(T i = 0; i < BitSize; ++i)
		{
			vecType<T, P> const BitSet(v & (static_cast<T>(1) << i));
			vecType<T, P> const BitFirst(BitSet >> i);
			Result |= BitFirst << (BitSize - 1 - i);
		}
		return Result;
	}

	template <typename T>
	GLM_FUNC_QUALIFIER T bitfieldReverseLoop(T v)
	{
		return bitfieldReverseLoop(glm::tvec1<T>(v)).x;
	}

	GLM_FUNC_QUALIFIER uint32_t bitfieldReverseUint32(uint32_t x)
	{
		x = (x & 0x55555555) <<  1 | (x & 0xAAAAAAAA) >>  1;
		x = (x & 0x33333333) <<  2 | (x & 0xCCCCCCCC) >>  2;
		x = (x & 0x0F0F0F0F) <<  4 | (x & 0xF0F0F0F0) >>  4;
		x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
		x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
		return x;
	}

	GLM_FUNC_QUALIFIER uint64_t bitfieldReverseUint64(uint64_t x)
	{
		x = (x & 0x5555555555555555) <<  1 | (x & 0xAAAAAAAAAAAAAAAA) >>  1;
		x = (x & 0x3333333333333333) <<  2 | (x & 0xCCCCCCCCCCCCCCCC) >>  2;
		x = (x & 0x0F0F0F0F0F0F0F0F) <<  4 | (x & 0xF0F0F0F0F0F0F0F0) >>  4;
		x = (x & 0x00FF00FF00FF00FF) <<  8 | (x & 0xFF00FF00FF00FF00) >>  8;
		x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
		x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
		return x;
	}

	template <bool EXEC = false>
	struct compute_bitfieldReverseStep
	{
		template <typename T, glm::precision P, template <class, glm::precision> class vecType>
		GLM_FUNC_QUALIFIER static vecType<T, P> call(vecType<T, P> const & v, T, T)
		{
			return v;
		}
	};

	template <>
	struct compute_bitfieldReverseStep<true>
	{
		template <typename T, glm::precision P, template <class, glm::precision> class vecType>
		GLM_FUNC_QUALIFIER static vecType<T, P> call(vecType<T, P> const & v, T Mask, T Shift)
		{
			return (v & Mask) << Shift | (v & (~Mask)) >> Shift;
		}
	};

	template <typename T, glm::precision P, template <typename, glm::precision> class vecType>
	GLM_FUNC_QUALIFIER vecType<T, P> bitfieldReverseOps(vecType<T, P> const & v)
	{
		vecType<T, P> x(v);
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  2>::call<T, P, vecType>(x, T(0x5555555555555555ull), static_cast<T>( 1));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  4>::call<T, P, vecType>(x, T(0x3333333333333333ull), static_cast<T>( 2));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >=  8>::call<T, P, vecType>(x, T(0x0F0F0F0F0F0F0F0Full), static_cast<T>( 4));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 16>::call<T, P, vecType>(x, T(0x00FF00FF00FF00FFull), static_cast<T>( 8));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 32>::call<T, P, vecType>(x, T(0x0000FFFF0000FFFFull), static_cast<T>(16));
		x = compute_bitfieldReverseStep<sizeof(T) * 8 >= 64>::call<T, P, vecType>(x, T(0x00000000FFFFFFFFull), static_cast<T>(32));
		return x;
	}

	template <typename genType>
	GLM_FUNC_QUALIFIER genType bitfieldReverseOps(genType x)
	{
		return bitfieldReverseOps(glm::tvec1<genType, glm::defaultp>(x)).x;
	}

	template <typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
		result		Result;
	};

	typedef type<glm::uint> typeU32;

	typeU32 const Data32[] =
	{
		{0x00000001, 0x80000000, SUCCESS},
		{0x0000000f, 0xf0000000, SUCCESS},
		{0x000000ff, 0xff000000, SUCCESS},
		{0xf0000000, 0x0000000f, SUCCESS},
		{0xff000000, 0x000000ff, SUCCESS},
		{0xffffffff, 0xffffffff, SUCCESS},
		{0x00000000, 0x00000000, SUCCESS}
	};

	typedef type<glm::uint64> typeU64;

#if(((GLM_COMPILER & GLM_COMPILER_GCC) == GLM_COMPILER_GCC) && (GLM_COMPILER < GLM_COMPILER_GCC44))
	typeU64 const Data64[] =
	{
		{0xf000000000000000LLU, 0x000000000000000fLLU, SUCCESS},
		{0xffffffffffffffffLLU, 0xffffffffffffffffLLU, SUCCESS},
		{0x0000000000000000LLU, 0x0000000000000000LLU, SUCCESS}
	};
#else
	typeU64 const Data64[] =
	{
		{0x00000000000000ff, 0xff00000000000000, SUCCESS},
		{0x000000000000000f, 0xf000000000000000, SUCCESS},
		{0xf000000000000000, 0x000000000000000f, SUCCESS},
		{0xffffffffffffffff, 0xffffffffffffffff, SUCCESS},
		{0x0000000000000000, 0x0000000000000000, SUCCESS}
	};
#endif

	int test32_bitfieldReverse()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = glm::bitfieldReverse(Data32[i].Value);
			
			bool Compare = Data32[i].Return == Return;
			
			if(Data32[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test32_bitfieldReverseLoop()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseLoop(Data32[i].Value);
			
			bool Compare = Data32[i].Return == Return;
			
			if(Data32[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test32_bitfieldReverseUint32()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseUint32(Data32[i].Value);
			
			bool Compare = Data32[i].Return == Return;
			
			if(Data32[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test32_bitfieldReverseOps()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data32) / sizeof(typeU32);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint Return = bitfieldReverseOps(Data32[i].Value);
			
			bool Compare = Data32[i].Return == Return;
			
			if(Data32[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test64_bitfieldReverse()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = glm::bitfieldReverse(Data64[i].Value);
			
			bool Compare = Data64[i].Return == Return;
			
			if(Data64[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test64_bitfieldReverseLoop()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseLoop(Data64[i].Value);
			
			bool Compare = Data64[i].Return == Return;
			
			if(Data32[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test64_bitfieldReverseUint64()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseUint64(Data64[i].Value);
			
			bool Compare = Data64[i].Return == Return;
			
			if(Data64[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test64_bitfieldReverseOps()
	{
		int Error = 0;
		std::size_t const Count = sizeof(Data64) / sizeof(typeU64);
		
		for(std::size_t i = 0; i < Count; ++i)
		{
			glm::uint64 Return = bitfieldReverseOps(Data64[i].Value);
			
			bool Compare = Data64[i].Return == Return;
			
			if(Data64[i].Result == SUCCESS)
				Error += Compare ? 0 : 1;
			else
				Error += Compare ? 1 : 0;
		}
		
		return Error;
	}

	int test()
	{
		int Error = 0;

		Error += test32_bitfieldReverse();
		Error += test32_bitfieldReverseLoop();
		Error += test32_bitfieldReverseUint32();
		Error += test32_bitfieldReverseOps();

		Error += test64_bitfieldReverse();
		Error += test64_bitfieldReverseLoop();
		Error += test64_bitfieldReverseUint64();
		Error += test64_bitfieldReverseOps();

		return Error;
	}

	int perf32()
	{
		int Error = 0;

		glm::uint32 Count = 10000000;
		std::vector<glm::uint32> Data;
		Data.resize(static_cast<std::size_t>(Count));

		std::clock_t Timestamps0 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = glm::bitfieldReverse(k);

		std::clock_t Timestamps1 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseLoop(k);

		std::clock_t Timestamps2 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseUint32(k);

		std::clock_t Timestamps3 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseOps(k);

		std::clock_t Timestamps4 = std::clock();

		std::printf("glm::bitfieldReverse: %d clocks\n", static_cast<unsigned int>(Timestamps1 - Timestamps0));
		std::printf("bitfieldReverseLoop: %d clocks\n", static_cast<unsigned int>(Timestamps2 - Timestamps1));
		std::printf("bitfieldReverseUint32: %d clocks\n", static_cast<unsigned int>(Timestamps3 - Timestamps2));
		std::printf("bitfieldReverseOps: %d clocks\n", static_cast<unsigned int>(Timestamps4 - Timestamps3));

		return Error;
	}

	int perf64()
	{
		int Error = 0;

		glm::uint64 Count = 10000000;
		std::vector<glm::uint64> Data;
		Data.resize(static_cast<std::size_t>(Count));

		std::clock_t Timestamps0 = std::clock();

		for(glm::uint32 k = 0; k < Count; ++k)
			Data[k] = glm::bitfieldReverse(k);

		std::clock_t Timestamps1 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseLoop(k);

		std::clock_t Timestamps2 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseUint64(k);

		std::clock_t Timestamps3 = std::clock();

		for(glm::uint64 k = 0; k < Count; ++k)
			Data[k] = bitfieldReverseOps(k);

		std::clock_t Timestamps4 = std::clock();

		std::printf("glm::bitfieldReverse - 64: %d clocks\n", static_cast<unsigned int>(Timestamps1 - Timestamps0));
		std::printf("bitfieldReverseLoop - 64: %d clocks\n", static_cast<unsigned int>(Timestamps2 - Timestamps1));
		std::printf("bitfieldReverseUint - 64: %d clocks\n", static_cast<unsigned int>(Timestamps3 - Timestamps2));
		std::printf("bitfieldReverseOps - 64: %d clocks\n", static_cast<unsigned int>(Timestamps4 - Timestamps3));

		return Error;
	}

	int perf()
	{
		int Error = 0;

		Error += perf32();
		Error += perf64();

		return Error;
	}
}//bitfieldReverse

namespace findMSB
{
	template <typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
	};

	template <typename genIUType>
	GLM_FUNC_QUALIFIER int findMSB_095(genIUType Value)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");
		
		if(Value == genIUType(0) || Value == genIUType(-1))
			return -1;
		else if(Value > 0)
		{
			genIUType Bit = genIUType(-1);
			for(genIUType tmp = Value; tmp > 0; tmp >>= 1, ++Bit){}
			return Bit;
		}
		else //if(Value < 0)
		{
			int const BitCount(sizeof(genIUType) * 8);
			int MostSignificantBit(-1);
			for(int BitIndex(0); BitIndex < BitCount; ++BitIndex)
				MostSignificantBit = (Value & (1 << BitIndex)) ? MostSignificantBit : BitIndex;
			assert(MostSignificantBit >= 0);
			return MostSignificantBit;
		}
	}

	template <typename genIUType>
	GLM_FUNC_QUALIFIER int findMSB_nlz1(genIUType x)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<genIUType>::is_integer, "'findMSB' only accept integer values");
/*
		int Result = 0;
		for(std::size_t i = 0, n = sizeof(genIUType) * 8; i < n; ++i)
			Result = Value & static_cast<genIUType>(1 << i) ? static_cast<int>(i) : Result;
		return Result;
*/
/*
		genIUType Bit = genIUType(-1);
		for(genIUType tmp = Value; tmp > 0; tmp >>= 1, ++Bit){}
		return Bit;
*/
		int n;

		if (x == 0) return(32);
		n = 0;
		if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
		if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
		if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
		if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
		if (x <= 0x7FFFFFFF) {n = n + 1;}
		return n;
	}

	int findMSB_nlz2(unsigned int x)
	{
		unsigned y;
		int n;

		n = 32;
		y = x >>16;  if (y != 0) {n = n -16;  x = y;}
		y = x >> 8;  if (y != 0) {n = n - 8;  x = y;}
		y = x >> 4;  if (y != 0) {n = n - 4;  x = y;}
		y = x >> 2;  if (y != 0) {n = n - 2;  x = y;}
		y = x >> 1;  if (y != 0) return n - 2;
		return n - x;
	}

	int perf_950()
	{
		type<glm::uint> const Data[] =
		{
			//{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		std::clock_t Timestamps1 = std::clock();

		for(std::size_t k = 0; k < 10000000; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int>); ++i)
		{
			int Result = findMSB_095(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps2 = std::clock();

		std::printf("findMSB - 0.9.5: %d clocks\n", static_cast<unsigned int>(Timestamps2 - Timestamps1));

		return Error;
	}

	int perf_ops()
	{
		type<int> const Data[] =
		{
			{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		std::clock_t Timestamps1 = std::clock();

		for(std::size_t k = 0; k < 10000000; ++k)
		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int>); ++i)
		{
			int Result = findMSB_nlz1(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
		}

		std::clock_t Timestamps2 = std::clock();

		std::printf("findMSB - nlz1: %d clocks\n", static_cast<unsigned int>(Timestamps2 - Timestamps1));

		return Error;
	}


	int test_findMSB()
	{
		type<glm::uint> const Data[] =
		{
			//{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int>); ++i)
		{
			int Result = glm::findMSB(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
			assert(!Error);
		}

		return Error;
	}

	int test_nlz1()
	{
		type<glm::uint> const Data[] =
		{
			//{0x00000000, -1},
			{0x00000001,  0},
			{0x00000002,  1},
			{0x00000003,  1},
			{0x00000004,  2},
			{0x00000005,  2},
			{0x00000007,  2},
			{0x00000008,  3},
			{0x00000010,  4},
			{0x00000020,  5},
			{0x00000040,  6},
			{0x00000080,  7},
			{0x00000100,  8},
			{0x00000200,  9},
			{0x00000400, 10},
			{0x00000800, 11},
			{0x00001000, 12},
			{0x00002000, 13},
			{0x00004000, 14},
			{0x00008000, 15},
			{0x00010000, 16},
			{0x00020000, 17},
			{0x00040000, 18},
			{0x00080000, 19},
			{0x00100000, 20},
			{0x00200000, 21},
			{0x00400000, 22},
			{0x00800000, 23},
			{0x01000000, 24},
			{0x02000000, 25},
			{0x04000000, 26},
			{0x08000000, 27},
			{0x10000000, 28},
			{0x20000000, 29},
			{0x40000000, 30}
		};

		int Error(0);

		for(std::size_t i = 0; i < sizeof(Data) / sizeof(type<int>); ++i)
		{
			int Result = findMSB_nlz2(Data[i].Value);
			Error += Data[i].Return == Result ? 0 : 1;
		}

		return Error;
	}

	int test()
	{
		int Error(0);

		Error += test_findMSB();
		//Error += test_nlz1();

		return Error;
	}

	int perf()
	{
		int Error(0);

		Error += perf_950();
		Error += perf_ops();

		return Error;
	}
}//findMSB

namespace findLSB
{
	template <typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
	};

	type<int> const DataI32[] =
	{
		{0x00000001,  0},
		{0x00000003,  0},
		{0x00000002,  1}
	};

	int test()
	{
		int Error(0);

		for(std::size_t i = 0; i < sizeof(DataI32) / sizeof(type<int>); ++i)
		{
			int Result = glm::findLSB(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
			assert(!Error);
		}

		return Error;
	}
}//findLSB

namespace uaddCarry
{
	int test()
	{
		int Error(0);
		
		{
			glm::uint x = 16;
			glm::uint y = 17;
			glm::uint Carry = 0;
			glm::uint Result = glm::uaddCarry(x, y, Carry);

			Error += Carry == 1 ? 0 : 1;
			Error += Result == 33 ? 0 : 1;
		}

		{
			glm::uvec1 x(16);
			glm::uvec1 y(17);
			glm::uvec1 Carry(0);
			glm::uvec1 Result(glm::uaddCarry(x, y, Carry));

			Error += glm::all(glm::equal(Carry, glm::uvec1(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec1(33))) ? 0 : 1;
		}

		{
			glm::uvec2 x(16);
			glm::uvec2 y(17);
			glm::uvec2 Carry(0);
			glm::uvec2 Result(glm::uaddCarry(x, y, Carry));

			Error += glm::all(glm::equal(Carry, glm::uvec2(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec2(33))) ? 0 : 1;
		}

		{
			glm::uvec3 x(16);
			glm::uvec3 y(17);
			glm::uvec3 Carry(0);
			glm::uvec3 Result(glm::uaddCarry(x, y, Carry));

			Error += glm::all(glm::equal(Carry, glm::uvec3(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec3(33))) ? 0 : 1;
		}

		{
			glm::uvec4 x(16);
			glm::uvec4 y(17);
			glm::uvec4 Carry(0);
			glm::uvec4 Result(glm::uaddCarry(x, y, Carry));

			Error += glm::all(glm::equal(Carry, glm::uvec4(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec4(33))) ? 0 : 1;
		}

		return Error;
	}
}//namespace uaddCarry

namespace usubBorrow
{
	int test()
	{
		int Error(0);
		
		{
			glm::uint x = 16;
			glm::uint y = 17;
			glm::uint Borrow = 0;
			glm::uint Result = glm::usubBorrow(x, y, Borrow);

			Error += Borrow == 1 ? 0 : 1;
			Error += Result == 1 ? 0 : 1;
		}

		{
			glm::uvec1 x(16);
			glm::uvec1 y(17);
			glm::uvec1 Borrow(0);
			glm::uvec1 Result(glm::usubBorrow(x, y, Borrow));

			Error += glm::all(glm::equal(Borrow, glm::uvec1(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec1(1))) ? 0 : 1;
		}

		{
			glm::uvec2 x(16);
			glm::uvec2 y(17);
			glm::uvec2 Borrow(0);
			glm::uvec2 Result(glm::usubBorrow(x, y, Borrow));

			Error += glm::all(glm::equal(Borrow, glm::uvec2(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec2(1))) ? 0 : 1;
		}

		{
			glm::uvec3 x(16);
			glm::uvec3 y(17);
			glm::uvec3 Borrow(0);
			glm::uvec3 Result(glm::usubBorrow(x, y, Borrow));

			Error += glm::all(glm::equal(Borrow, glm::uvec3(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec3(1))) ? 0 : 1;
		}

		{
			glm::uvec4 x(16);
			glm::uvec4 y(17);
			glm::uvec4 Borrow(0);
			glm::uvec4 Result(glm::usubBorrow(x, y, Borrow));

			Error += glm::all(glm::equal(Borrow, glm::uvec4(1))) ? 0 : 1;
			Error += glm::all(glm::equal(Result, glm::uvec4(1))) ? 0 : 1;
		}

		return Error;
	}
}//namespace usubBorrow

namespace umulExtended
{
	int test()
	{
		int Error(0);
		
		{
			glm::uint x = 2;
			glm::uint y = 3;
			glm::uint msb = 0;
			glm::uint lsb = 0;
			glm::umulExtended(x, y, msb, lsb);

			Error += msb == 0 ? 0 : 1;
			Error += lsb == 6 ? 0 : 1;
		}

		{
			glm::uvec1 x(2);
			glm::uvec1 y(3);
			glm::uvec1 msb(0);
			glm::uvec1 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::uvec1(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::uvec1(6))) ? 0 : 1;
		}

		{
			glm::uvec2 x(2);
			glm::uvec2 y(3);
			glm::uvec2 msb(0);
			glm::uvec2 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::uvec2(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::uvec2(6))) ? 0 : 1;
		}

		{
			glm::uvec3 x(2);
			glm::uvec3 y(3);
			glm::uvec3 msb(0);
			glm::uvec3 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::uvec3(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::uvec3(6))) ? 0 : 1;
		}

		{
			glm::uvec4 x(2);
			glm::uvec4 y(3);
			glm::uvec4 msb(0);
			glm::uvec4 lsb(0);
			glm::umulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::uvec4(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::uvec4(6))) ? 0 : 1;
		}

		return Error;
	}
}//namespace umulExtended

namespace imulExtended
{
	int test()
	{
		int Error(0);
		
		{
			int x = 2;
			int y = 3;
			int msb = 0;
			int lsb = 0;
			glm::imulExtended(x, y, msb, lsb);

			Error += msb == 0 ? 0 : 1;
			Error += lsb == 6 ? 0 : 1;
		}

		{
			glm::ivec1 x(2);
			glm::ivec1 y(3);
			glm::ivec1 msb(0);
			glm::ivec1 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::ivec1(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::ivec1(6))) ? 0 : 1;
		}

		{
			glm::ivec2 x(2);
			glm::ivec2 y(3);
			glm::ivec2 msb(0);
			glm::ivec2 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::ivec2(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::ivec2(6))) ? 0 : 1;
		}

		{
			glm::ivec3 x(2);
			glm::ivec3 y(3);
			glm::ivec3 msb(0);
			glm::ivec3 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::ivec3(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::ivec3(6))) ? 0 : 1;
		}

		{
			glm::ivec4 x(2);
			glm::ivec4 y(3);
			glm::ivec4 msb(0);
			glm::ivec4 lsb(0);
			glm::imulExtended(x, y, msb, lsb);

			Error += glm::all(glm::equal(msb, glm::ivec4(0))) ? 0 : 1;
			Error += glm::all(glm::equal(lsb, glm::ivec4(6))) ? 0 : 1;
		}

		return Error;
	}
}//namespace imulExtended

namespace bitCount
{
	template <typename genType>
	struct type
	{
		genType		Value;
		genType		Return;
	};

	type<int> const DataI32[] =
	{
		{0x00000001,  1},
		{0x00000003,  2},
		{0x00000002,  1},
		{0x7fffffff, 31},
		{0x00000000,  0}
	};

	template <typename T>
	inline int bitCount_if(T v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		int Count(0);
		for(T i = 0, n = static_cast<T>(sizeof(T) * 8); i < n; ++i)
		{
			if(v & static_cast<T>(1 << i))
				++Count;
		}
		return Count;
	}

	template <typename T>
	inline int bitCount_vec(T v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		int Count(0);
		for(T i = 0, n = static_cast<T>(sizeof(T) * 8); i < n; ++i)
		{
			Count += static_cast<int>((v >> i) & static_cast<T>(1));
		}
		return Count;
	}

	template <typename T>
	inline int bitCount_bits(T v)
	{
		GLM_STATIC_ASSERT(std::numeric_limits<T>::is_integer, "'bitCount' only accept integer values");

		int Count(0);
		for(T i = 0, n = static_cast<T>(sizeof(T) * 8); i < n; ++i)
		{
			Count += static_cast<int>((v >> i) & static_cast<T>(1));
		}
		return Count;
	}

	int perf()
	{
		int Error(0);

		std::size_t Size = 10000000;
		std::vector<int> v;
		v.resize(Size);

		std::vector<glm::ivec4> w;
		w.resize(Size);


		std::clock_t TimestampsA = std::clock();

		// bitCount - TimeIf
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = bitCount_if(i);
		}

		std::clock_t TimestampsB = std::clock();

		// bitCount - TimeVec
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = bitCount_vec(i);
		}

		std::clock_t TimestampsC = std::clock();

		// bitCount - TimeDefault
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				v[i] = glm::bitCount(i);
		}

		std::clock_t TimestampsD = std::clock();

		// bitCount - TimeVec4
		{
			for(std::size_t i = 0, n = v.size(); i < n; ++i)
				w[i] = glm::bitCount(glm::ivec4(static_cast<int>(i)));
		}

		std::clock_t TimestampsE = std::clock();

		std::clock_t TimeIf = TimestampsB - TimestampsA;
		std::clock_t TimeVec = TimestampsC - TimestampsB;
		std::clock_t TimeDefault = TimestampsD - TimestampsC;
		std::clock_t TimeVec4 = TimestampsE - TimestampsD;

		std::printf("bitCount - TimeIf %d\n", static_cast<unsigned int>(TimeIf));
		std::printf("bitCount - TimeVec %d\n", static_cast<unsigned int>(TimeVec));
		std::printf("bitCount - TimeDefault %d\n", static_cast<unsigned int>(TimeDefault));
		std::printf("bitCount - TimeVec4 %d\n", static_cast<unsigned int>(TimeVec4));

		return Error;
	}

	int test()
	{
		int Error(0);

		for(std::size_t i = 0, n = sizeof(DataI32) / sizeof(type<int>); i < n; ++i)
		{
			int Result = glm::bitCount(DataI32[i].Value);
			Error += DataI32[i].Return == Result ? 0 : 1;
			assert(!Error);
		}

		return Error;
	}
}//bitCount

int main()
{
	int Error = 0;

	Error += ::bitfieldReverse::test();
	Error += ::bitfieldReverse::perf();
	Error += ::findMSB::test();
	Error += ::findMSB::perf();
	Error += ::findLSB::test();
	Error += ::umulExtended::test();
	Error += ::imulExtended::test();
	Error += ::uaddCarry::test();
	Error += ::usubBorrow::test();
	Error += ::bitfieldInsert::test();
	Error += ::bitfieldExtract::test();
	Error += ::bitCount::test();
	Error += ::bitCount::perf();

	return Error;
}
