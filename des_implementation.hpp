#pragma once

#include "des.h"
// bit mask
#define LB_IN32 0x00000001U
#define LB_IN64 0x0000000000000001UI64
#define L32_IN64 0x00000000ffffffffUI64
#define H32_IN64 0xffffffff00000000UI64
#define L28_IN64 0x000000000fffffffUI64
#define L28_IN32 0x0fffffffUL

static uint32_t s_op(uint64_t s_i)
{
	uint32_t s_o = 0;
	uint8_t row, col, low6;
	for (size_t b_idx = 0, margin = 42; b_idx < 8; ++b_idx, margin -= 6)
	{
		low6 = (uint8_t)(s_i >> margin);
		row = ((low6 >> 4) & 0x02) | (low6 & 0x01);
		col = (uint8_t)(low6 >> 1) & 0x0f;
		s_o <<= 4;
		s_o |= (uint32_t)(S[b_idx][(row << 4) + col] & 0x0f);
	}
	return s_o;
}


static inline uint64_t e_op(uint32_t e_in) {

	uint64_t ret = 0;
	for (size_t j = 0; j < 48; j++)
	{
		ret <<= 1;
		ret |= (uint64_t)((e_in >> (32 - E[j])) & LB_IN32);
	}
	return ret;
}


static uint64_t des(uint64_t input, uint64_t* sub_key, char mode)
{
	// initial permutation
	uint64_t ip_o = 0;
	for (int i = 0; i < 64; i++)
	{
		ip_o <<= 1;
		ip_o |= (input >> (64 - IP[i])) & LB_IN64;
	}

	// 均分
	uint32_t L = (uint32_t)(ip_o >> 32) & L32_IN64;
	uint32_t R = (uint32_t)(ip_o & L32_IN64);

	// f(R,K)
	for (size_t r = 0; r < 16; r++)
	{
		// e extend
		uint64_t s_i = 0;
		for (size_t j = 0; j < 48; j++)
		{
			s_i <<= 1;
			s_i |= (uint64_t)((R >> (32 - E[j])) & LB_IN32);
		}

		// xor
		s_i ^= (mode == 'd' ? sub_key[15 - r] : sub_key[r]);

		// s
		uint32_t s_o = s_op(s_i);

		// p
		uint32_t f_o = 0;
		for (size_t i = 0; i < 32; i++)
		{
			f_o <<= 1;
			f_o |= (s_o >> (32 - P[i])) & LB_IN32;
		}

		L ^= f_o;
		if (r != 15)
		{
			L ^= R;
			R ^= L;
			L ^= R;
		}
	}

	uint64_t ret = 0;
	uint64_t pre_o = (uint64_t)R | ((uint64_t)L << 32);
	// inverse initial permutation
	for (int i = 0; i < 64; i++)
	{
		ret <<= 1;
		ret |= (pre_o >> (64 - PI[i])) & LB_IN64;
	}
	return ret;
}

// 生成轮密钥
static void key_gen(uint64_t key, uint64_t* sub_key)
{

	uint64_t pc1_o = 0; // 64->56
	uint64_t pc2_o = 0; // 56->48

	// PC1 64->56
	for (int i = 0; i < 56; i++)
	{
		// (i+1)_th bit from left <- (PC[i])_th bit from left
		// <- (64-PC[i]+1)_th bit from right
		pc1_o <<= 1;
		pc1_o |= (key >> (64 - PC1[i])) & LB_IN64;
		// another methods
		// pc1_o |= (
		//	LB_IN64 & (key >> (64 - PC1[i]))
		//	) << (56 - i);
	}
	//左右 28 bits
	uint32_t C = (uint32_t)((pc1_o >> 28) & L28_IN64);
	;
	uint32_t D = (uint32_t)(pc1_o & L28_IN64);

	// shift
	for (size_t i = 0; i < 16; i++)
	{
		int s = iteration_shift[i];
		for (int j = 0; j < s; j++)
		{
			C = L28_IN32 & (C << 1) | LB_IN32 & (C >> 27);
			D = L28_IN32 & (D << 1) | LB_IN32 & (D >> 27);
		}
		pc2_o = (((uint64_t)C) << 28) | (uint64_t)D;
		// PC2
		sub_key[i] = 0;
		for (int j = 0; j < 48; j++)
		{
			sub_key[i] <<= 1;
			sub_key[i] |= (pc2_o >> (56 - PC2[j])) & LB_IN64;
		}
	}
}
