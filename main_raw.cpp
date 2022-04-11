//#include<chrono>	//time measure
//#include<iostream>
//#include<fstream>
//#include<iomanip>
//#include"des.h"
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
//
//static uint64_t des(uint64_t input, uint64_t* sub_key, char mode) {
//
//	// initial permutation
//	uint64_t ip_o = 0;
//	for (int i = 0; i < 64; i++) {
//		ip_o <<= 1;
//		ip_o |= (input >> (64 - IP[i])) & LB_IN64;
//	}
//
//	// 均分
//	uint32_t L = (uint32_t)(ip_o >> 32) & L32_IN64;
//	uint32_t R = (uint32_t)(ip_o & L32_IN64);
//
//	// f(R,K)
//	for (size_t r = 0; r < 16; r++) {
//		// e extend
//		uint64_t s_i = 0;
//		for (size_t j = 0; j < 48; j++) {
//			s_i <<= 1;
//			s_i |= (uint64_t)((R >> (32 - E[j])) & LB_IN32);
//		}
//
//		// xor
//		s_i ^= (mode == 'd' ? sub_key[15 - r] : sub_key[r]);
//
//		// s 
//		uint32_t s_o = s_op(s_i);
//
//		// p  
//		uint32_t f_o = 0;
//		for (size_t i = 0; i < 32; i++) {
//			f_o <<= 1;
//			f_o |= (s_o >> (32 - P[i])) & LB_IN32;
//		}
//
//		L ^= f_o;
//		if (r != 15) {
//			L ^= R;
//			R ^= L;
//			L ^= R;
//		}
//	}
//
//	uint64_t ret = 0;
//	uint64_t pre_o = (uint64_t)R | ((uint64_t)L << 32);
//	// inverse initial permutation
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
//
//int main() {
//
//
//
//	// buf of input
//	uint64_t input_buf = 0xd76346397c9fb03b;
//	//d76346397c9fb03b
//	uint64_t key = 0x133457799bbcdff1;
//
//	// 16 rounds' keys
//	uint64_t sub_key[16]{ 0 };
//	key_gen(key, sub_key);
//
//
//	// 打印轮密钥
//	cout << "轮密钥:" << endl;
//	for (size_t i = 0; i < 16; i++) 
//		cout << "\t" << hex << setw(12) << sub_key[i] << endl;
//
//	// 解密结果
//	cout << setfill('0');
//	uint64_t output_buf = des(input_buf, sub_key, 'e');
//	cout << "加密" << endl;
//	cout << "\t" << hex << setw(16) << output_buf << endl;
//	cout << "解密" << endl;
//	cout << "\t" << hex << setw(16) << des(output_buf, sub_key, 'd') << endl;
//
//#define TIME
//#ifdef TIME
//	ifstream fin("testbench.txt");
//	auto begin = std::chrono::high_resolution_clock::now();
//	while (fin >> input_buf) {
//		output_buf = des(input_buf, sub_key, 'e');
//	}
//	auto end = std::chrono::high_resolution_clock::now();
//	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//	printf("Time measured: %.9f\n", elapsed.count() * 1e-9);
//#endif // TIME
//}