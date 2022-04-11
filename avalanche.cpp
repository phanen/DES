//#include<iostream>
//#include<fstream>
//#include<iomanip>
//#include"main_raw.h"
//using namespace std;
//
//// bit mask
//#define LB_IN32   0x00000001U
//#define LB_IN64   0x0000000000000001ULL
//#define L32_IN64    0x00000000ffffffffULL
//#define H32_IN64    0xffffffff00000000ULL
//#define L28_IN64	0x000000000fffffffULL
//#define L28_IN32	0x0fffffffUL
//
//
//#define TURNS 6
//static uint64_t s_op(uint64_t s_i) {
//	uint64_t s_o = 0;
//	uint8_t row, col, low6;
//	for (size_t b_idx = 0, margin = 42; b_idx < 8; ++b_idx, margin -= 6) {
//		low6 = (uint8_t)(s_i >> margin);
//		row = ((low6 >> 4) & 0x02) | (low6 & 0x01);
//		col = (uint8_t)(low6 >> 1) & 0x0f;
//		s_o <<= 4;
//		s_o |= (uint32_t)(S[b_idx][(row << 4) + col] & 0x0f);
//	}
//	return s_o;
//}
//
//// 拓展到 48 位（取代 e扩散） 
//uint64_t pad48(uint64_t input) {
//	uint64_t ret = 0;
//	for (size_t i = 0; i < 8; i++) {
//		ret |= ((input & 0xf) << (i * 6 + 1));
//		input >>= 4;
//	}
//	return ret;
//}
//
//static uint64_t avalanche_base(uint64_t input, uint64_t* sub_key, int turn) {
//	// initial permutation
//	uint64_t ip_o = 0;
//	for (int i = 0; i < 64; i++) {
//		ip_o <<= 1;
//		ip_o |= (input >> (64 - IP[i])) & LB_IN64;
//	}
//	uint32_t L = (uint32_t)(ip_o >> 32) & L32_IN64;
//	uint32_t R = (uint32_t)(ip_o & L32_IN64);
//
//	// f(R,K)
//	for (size_t r = 0; r < turn; r++) {
//		// e extend
//		uint64_t s_i = 0;
//		for (size_t j = 0; j < 48; j++) {
//			s_i <<= 1;
//			s_i |= (uint64_t)((R >> (32 - E[j])) & LB_IN32);
//		}
//
//		uint32_t s_o = s_op(s_i ^ sub_key[r]);
//
//		// p  
//		uint32_t f_o = 0;
//		for (size_t i = 0; i < 32; i++) {
//			f_o <<= 1;
//			f_o |= (s_o >> (32 - P[i])) & LB_IN32;
//		}
//
//		L ^= f_o;
//		L ^= R;
//		R ^= L;
//		L ^= R;
//	}
//
//	uint64_t pre_o = (uint64_t)R | ((uint64_t)L << 32);
//	uint64_t ret = 0;
//	for (int i = 0; i < 64; i++) {
//		ret <<= 1;
//		ret |= (pre_o >> (64 - PI[i])) & LB_IN64;
//	}
//	return ret;
//}
//
//static uint64_t avalanche_no_e_no_p(uint64_t input, uint64_t* sub_key, int turn) {
//	// initial permutation
//	uint64_t ip_o = 0;
//	for (int i = 0; i < 64; i++) {
//		ip_o <<= 1;
//		ip_o |= (input >> (64 - IP[i])) & LB_IN64;
//	}
//	uint32_t L = (uint32_t)(ip_o >> 32) & L32_IN64;
//	uint32_t R = (uint32_t)(ip_o & L32_IN64);
//
//	// f(R,K)
//	for (size_t r = 0; r < turn; r++) {
//		// 此处没有 e 扩散 通过填充到 48位再异或进S盒
//		uint64_t s_i = pad48(R) ^ sub_key[r];
//		uint32_t f_o = s_op(s_i);
//		// 此处没有 p 置换
//		L ^= f_o;
//		L ^= R;
//		R ^= L;
//		L ^= R;
//	}
//
//	uint64_t pre_o = (uint64_t)R | ((uint64_t)L << 32);
//	uint64_t ret = 0;
//	for (int i = 0; i < 64; i++) {
//		ret <<= 1;
//		ret |= (pre_o >> (64 - PI[i])) & LB_IN64;
//	}
//	return ret;
//}
//
//
//static uint64_t avalanche_no_e_no_s(uint64_t input, uint64_t* sub_key, int turn) {
//	// initial permutation
//	uint64_t ip_o = 0;
//	for (int i = 0; i < 64; i++) {
//		ip_o <<= 1;
//		ip_o |= (input >> (64 - IP[i])) & LB_IN64;
//	}
//	uint32_t L = (uint32_t)(ip_o >> 32) & L32_IN64;
//	uint32_t R = (uint32_t)(ip_o & L32_IN64);
//
//	// f(R,K)
//	for (size_t r = 0; r < turn; r++) {
//		// 此处没有 e 扩散
//		uint32_t p_i = R ^ (sub_key[r] & 0xffffffff);
//		// 此处没有 s 混淆
//
//		// p 置换
//		uint32_t f_o = 0;
//		for (size_t i = 0; i < 32; i++) {
//			f_o <<= 1;
//			f_o |= (p_i >> (32 - P[i])) & LB_IN32;
//		}
//
//		L ^= f_o;
//		L ^= R;
//		R ^= L;
//		L ^= R;
//
//	}
//	uint64_t pre_o = (uint64_t)R | ((uint64_t)L << 32);
//	uint64_t ret = 0;
//	for (int i = 0; i < 64; i++) {
//		ret <<= 1;
//		ret |= (pre_o >> (64 - PI[i])) & LB_IN64;
//	}
//	return ret;
//}
//
//
//// 生成轮密钥
//static void key_gen(uint64_t key, uint64_t* sub_key) {
//
//	uint64_t pc1_o = 0;	//64->56
//	uint64_t pc2_o = 0; //56->48
//
//	// PC1 64->56
//	for (int i = 0; i < 56; i++) {
//		// (i+1)_th bit from left <- (PC[i])_th bit from left
//		// <- (64-PC[i]+1)_th bit from right
//		pc1_o <<= 1;
//		pc1_o |= (key >> (64 - PC1[i])) & LB_IN64;
//		// another methods
//		//pc1_o |= (
//		//	LB_IN64 & (key >> (64 - PC1[i]))
//		//	) << (56 - i);
//	}
//	//左右 28 bits
//	uint32_t C = (uint32_t)((pc1_o >> 28) & L28_IN64);;
//	uint32_t D = (uint32_t)(pc1_o & L28_IN64);
//
//	// shift
//	for (size_t i = 0; i < 16; i++) {
//		int s = iteration_shift[i];
//		for (int j = 0; j < s; j++) {
//			C = L28_IN32 & (C << 1) | LB_IN32 & (C >> 27);
//			D = L28_IN32 & (D << 1) | LB_IN32 & (D >> 27);
//		}
//		pc2_o = (((uint64_t)C) << 28) | (uint64_t)D;
//		// PC2
//		sub_key[i] = 0;
//		for (int j = 0; j < 48; j++) {
//			sub_key[i] <<= 1;
//			sub_key[i] |= (pc2_o >> (56 - PC2[j])) & LB_IN64;
//		}
//	}
//}
//
//
//// 对 64bits块 的低 8 位分别翻转
//void change_low8_bits(uint64_t input, uint64_t* ret) {
//	uint64_t bit_mask = 1;
//	for (size_t i = 0; i < 8; ++i, bit_mask <<= 1)
//		ret[i] = input ^ bit_mask;
//}
//
//// 计数 1
//int count_one(uint64_t num) {
//	int cnt = 0;
//	while (num) {
//		cnt += num & 1;
//		num >>= 1;
//	}
//	return cnt;
//}
//
//int main() {
//	uint64_t input_origin = 0x02468aceeca86420;
//	uint64_t key_origin = 0xff14717847d8e859;
//	uint64_t output_origin = 0;
//	uint64_t sub_key[16]{ 0 };
//
//	uint64_t input_test[8]{ 0 };
//	uint64_t key_test[8]{ 0 };
//	uint64_t output_test[8]{ 0 };
//	int one_cnt_test[8]{ 0 };
//
//	change_low8_bits(input_origin, input_test);
//	change_low8_bits(key_origin, key_test);
//
//
//
//
//	for (size_t turn = 1; turn <= 6; turn++) {
//		cout << turn << " 轮*************************************" << endl;
//		cout << "原函数改明文" << endl;
//		key_gen(key_origin, sub_key);
//		output_origin = avalanche_base(input_origin, sub_key,turn);
//		for (size_t t = 0; t < 8; t++) {
//			output_test[t] = avalanche_base(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//		cout << "原函数改密钥" << endl;
//		for (size_t t = 0; t < 8; t++) {
//			key_gen(key_test[t], sub_key);
//			output_test[t] = avalanche_base(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//		cout << "没有 e扩散 和 p置换 改明文" << endl;
//		key_gen(key_origin, sub_key);
//		output_origin = avalanche_no_e_no_p(input_origin, sub_key, turn);
//		for (size_t t = 0; t < 8; t++) {
//			output_test[t] = avalanche_no_e_no_p(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//		cout << "没有 e扩散 和 p置换 改密钥" << endl;
//		for (size_t t = 0; t < 8; t++) {
//			key_gen(key_test[t], sub_key);
//			output_test[t] = avalanche_no_e_no_p(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//		cout << "没有 e扩散 和 s混淆 改明文" << endl;
//		key_gen(key_origin, sub_key);
//		output_origin = avalanche_no_e_no_s(input_origin, sub_key, turn);
//		for (size_t t = 0; t < 8; t++) {
//			output_test[t] = avalanche_no_e_no_s(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//		cout << "没有 e扩散 和 s混淆 改密钥" << endl;
//		for (size_t t = 0; t < 8; t++) {
//			key_gen(key_test[t], sub_key);
//			output_test[t] = avalanche_no_e_no_s(input_test[t], sub_key, turn);
//			one_cnt_test[t] = count_one(output_origin ^ output_test[t]);
//			cout << one_cnt_test[t] << ((t == 7) ? "\n" : " ");
//		}
//
//	}
//}
