//#include "des_bitset.h"
//
//const static unsigned char IP[64] = {
//	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 , 60 , 52 , 44 , 36 , 28 , 20 , 12 , 4 ,
//	62 , 54 , 46 , 38 , 30 , 22 , 14 , 6 , 64 , 56 , 48 , 40 , 32 , 24 , 16 , 8 ,
//	57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 , 59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
//	61 , 53 , 45 , 37 , 29 , 21 , 13 , 5 , 63 , 55 , 47 , 39 , 31 , 23 , 15 , 7
//};
////IP 逆置换
//const static unsigned char IP_R[64] = {
//	40 , 8 , 48 , 16 , 56 , 24 , 64 , 32 , 39 , 7 , 47 , 15 , 55 , 23 , 63 , 31 ,
//	38 , 6 , 46 , 14 , 54 , 22 , 62 , 30 , 37 , 5 , 45 , 13 , 53 , 21 , 61 , 29 ,
//	36 , 4 , 44 , 12 , 52 , 20 , 60 , 28 , 35 , 3 , 43 , 11 , 51 , 19 , 59 , 27 ,
//	34 , 2 , 42 , 10 , 50 , 18 , 58 , 26 , 33 , 1 , 41 , 9 , 49 , 17 , 57 , 25
//};
//
////E 拓展
//static const unsigned char E_TABLE[48] = {
//	32 , 1 , 2 , 3 , 4 , 5 , 4 , 5 , 6 , 7 , 8 , 9 , 8 , 9 , 10 , 11 ,
//	12 , 13 , 12 , 13 , 14 , 15 , 16 , 17 , 16 , 17 , 18 , 19 , 20 , 21 , 20 , 21 ,
//	22 , 23 , 24 , 25 , 24 , 25 , 26 , 27 , 28 , 29 , 28 , 29 , 30 , 31 , 32 , 1
//};
//
////S 盒
//const static unsigned char S_TABLE[8][64] = {
//	{//S1
//		14 , 4 , 13 , 1 , 2 , 15 , 11 , 8 , 3 , 10 , 6 , 12 , 5 , 9 , 0 , 7 ,
//		0 , 15 , 7 , 4 , 14 , 2 , 13 , 1 , 10 , 6 , 12 , 11 , 9 , 5 , 3 , 8 ,
//		4 , 1 , 14 , 8 , 13 , 6 , 2 , 11 , 15 , 12 , 9 , 7 , 3 , 10 , 5 , 0 ,
//		15 , 12 , 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11 , 3 , 14 , 10 , 0 , 6 , 13
//} ,
//	{//S2
//		15 , 1 , 8 , 14 , 6 , 11 , 3 , 4 , 9 , 7 , 2 , 13 , 12 , 0 , 5 , 10 ,
//		3 , 13 , 4 , 7 , 15 , 2 , 8 , 14 , 12 , 0 , 1 , 10 , 6 , 9 , 11 , 5 ,
//		0 , 14 , 7 , 11 , 10 , 4 , 13 , 1 , 5 , 8 , 12 , 6 , 9 , 3 , 2 , 15 ,
//		13 , 8 , 10 , 1 , 3 , 15 , 4 , 2 , 11 , 6 , 7 , 12 , 0 , 5 , 14 , 9
//} ,
//	{//S3
//		10 , 0 , 9 , 14 , 6 , 3 , 15 , 5 , 1 , 13 , 12 , 7 , 11 , 4 , 2 , 8 ,
//		13 , 7 , 0 , 9 , 3 , 4 , 6 , 10 , 2 , 8 , 5 , 14 , 12 , 11 , 15 , 1 ,
//		13 , 6 , 4 , 9 , 8 , 15 , 3 , 0 , 11 , 1 , 2 , 12 , 5 , 10 , 14 , 7 ,
//		1 , 10 , 13 , 0 , 6 , 9 , 8 , 7 , 4 , 15 , 14 , 3 , 11 , 5 , 2 , 12
//} ,
//	{//S4
//		7 , 13 , 14 , 3 , 0 , 6 , 9 , 10 , 1 , 2 , 8 , 5 , 11 , 12 , 4 , 15 ,
//		13 , 8 , 11 , 5 , 6 , 15 , 0 , 3 , 4 , 7 , 2 , 12 , 1 , 10 , 14 , 9 ,
//		10 , 6 , 9 , 0 , 12 , 11 , 7 , 13 , 15 , 1 , 3 , 14 , 5 , 2 , 8 , 4 ,
//		3 , 15 , 0 , 6 , 10 , 1 , 13 , 8 , 9 , 4 , 5 , 11 , 12 , 7 , 2 , 14
//} ,
//		{//S5
//	2 , 12 , 4 , 1 , 7 , 10 , 11 , 6 , 8 , 5 , 3 , 15 , 13 , 0 , 14 , 9 ,
//		14 , 11 , 2 , 12 , 4 , 7 , 13 , 1 , 5 , 0 , 15 , 10 , 3 , 9 , 8 , 6 ,
//		4 , 2 , 1 , 11 , 10 , 13 , 7 , 8 , 15 , 9 , 12 , 5 , 6 , 3 , 0 , 14 ,
//		11 , 8 , 12 , 7 , 1 , 14 , 2 , 13 , 6 , 15 , 0 , 9 , 10 , 4 , 5 , 3
//} ,
//		{//S6
//		12 , 1 , 10 , 15 , 9 , 2 , 6 , 8 , 0 , 13 , 3 , 4 , 14 , 7 , 5 , 11 ,
//		10 , 15 , 4 , 2 , 7 , 12 , 9 , 5 , 6 , 1 , 13 , 14 , 0 , 11 , 3 , 8 ,
//		9 , 14 , 15 , 5 , 2 , 8 , 12 , 3 , 7 , 0 , 4 , 10 , 1 , 13 , 11 , 6 ,
//		4 , 3 , 2 , 12 , 9 , 5 , 15 , 10 , 11 , 14 , 1 , 7 , 6 , 0 , 8 , 13
//} ,
//	{//S7
//		4 , 11 , 2 , 14 , 15 , 0 , 8 , 13 , 3 , 12 , 9 , 7 , 5 , 10 , 6 , 1 ,
//		13 , 0 , 11 , 7 , 4 , 9 , 1 , 10 , 14 , 3 , 5 , 12 , 2 , 15 , 8 , 6 ,
//		1 , 4 , 11 , 13 , 12 , 3 , 7 , 14 , 10 , 15 , 6 , 8 , 0 , 5 , 9 , 2 ,
//		6 , 11 , 13 , 8 , 1 , 4 , 10 , 7 , 9 , 5 , 0 , 15 , 14 , 2 , 3 , 12
//} ,
//	{//S8
//		13 , 2 , 8 , 4 , 6 , 15 , 11 , 1 , 10 , 9 , 3 , 14 , 5 , 0 , 12 , 7 ,
//		1 , 15 , 13 , 8 , 10 , 3 , 7 , 4 , 12 , 5 , 6 , 11 , 0 , 14 , 9 , 2 ,
//		7 , 11 , 4 , 1 , 9 , 12 , 14 , 2 , 0 , 6 , 10 , 13 , 15 , 3 , 5 , 8 ,
//		2 , 1 , 14 , 7 , 4 , 10 , 8 , 13 , 15 , 12 , 9 , 0 , 3 , 5 , 6 , 11
//	}
//};
//
////P 置换
//const static unsigned char P_TABLE[32] = {
//	16 , 7 , 20 , 21 , 29 , 12 , 28 , 17 , 1 , 15 , 23 , 26 , 5 , 18 , 31 , 10 ,
//	2 , 8 , 24 , 14 , 32 , 27 , 3 , 9 , 19 , 13 , 30 , 6 , 22 , 11 , 4 , 25
//};
//
//
////压缩置换 56 --> 48
//const static unsigned char PC1[56] = {
//	57 , 49 , 41 , 33 , 25 , 17 , 9 , 1 ,
//	58 , 50 , 42 , 34 , 26 , 18 , 10 , 2 ,
//	59 , 51 , 43 , 35 , 27 , 19 , 11 , 3 ,
//	60 , 52 , 44 , 36 , 63 , 55 , 47 , 39 ,
//	31 , 23 , 15 , 7 , 62 , 54 , 46 , 38 ,
//	30 , 22 , 14 , 6 , 61 , 53 , 45 , 37 ,
//	29 , 21 , 13 , 5 , 28 , 20 , 12 , 4
//};
//
////移位表
//const static unsigned char BIT_SHIFT[16] = {
//	1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1
//};
//
////压缩置换2 56-->48
//const static unsigned char PC2[48] = {
//	14 , 17 , 11 , 24 , 1 , 5 , 3 , 28 ,
//	15 , 6 , 21 , 10 , 23 , 19 , 12 , 4 ,
//	26 , 8 , 16 , 7 , 27 , 20 , 13 , 2 ,
//	41 , 52 , 31 , 37 , 47 , 55 , 30 , 40 ,
//	51 , 45 , 33 , 48 , 44 , 49 , 39 , 56 ,
//	34 , 53 , 46 , 42 , 50 , 36 , 29 , 32
//};
//
//
//
//B48 e_box(const B32& in) {
//	B48 ret;
//	for (size_t i = 0; i < 48; i++)
//	{
//		ret[i] = in[E_TABLE[i] - 1];
//	}
//	return ret;
//}
//
//B32 s_box(const B48& in) {
//	B32 ret;
//	std::bitset<4> col_index;//列索引
//	std::bitset<2> row_index;//行索引
//	size_t cnt = 0;
//	for (size_t box_index = 0; box_index < 8; box_index++)
//	{
//		row_index[1] = in[cnt++];
//		col_index[3] = in[cnt++];
//		col_index[2] = in[cnt++];
//		col_index[1] = in[cnt++];
//		col_index[0] = in[cnt++];
//		row_index[0] = in[cnt++];
//
//		std::bitset<4> tmp(S_TABLE[box_index][(row_index.to_ulong() << 4) + col_index.to_ulong()]);
//		for (size_t i = 0; i < 4; i++)
//		{
//			ret[i + (box_index << 2)] = tmp[3 - i];
//		}
//	}
//	return ret;
//}
//
//B32 p_box(const B32& in) {
//	B32 ret;
//	for (size_t i = 0; i < 32; i++)
//	{
//		ret[i] = in[P_TABLE[i] - 1];
//	}
//	return ret;
//}
//
//
//void key_gen(const B64& k64, B48* ret)
//{	//64 -->  56 --> 48
//	B56 k56;
//	//permuted choice 1  64 --> 56 
//	for (size_t i = 0; i < 56; ++i)
//		k56[i] = k64[PC1[i] - 1];
//
//	//移位 56 --> 48
//	for (size_t r = 0; r < 16; ++r)
//	{
//		for (size_t i = 0; i < BIT_SHIFT[r]; i++)
//		{// 入队列
//			ret[r][i] = k56[i];
//			ret[r][BIT_SHIFT[r] + i] = k56[28 + i];
//		}
//		k56 >>= BIT_SHIFT[r];
//		for (size_t i = 0; i < BIT_SHIFT[r]; i++)
//		{// 出队列
//			k56[28 - BIT_SHIFT[r] + i] = ret[r][i];
//			k56[56 - BIT_SHIFT[r] + i] = ret[r][i + BIT_SHIFT[r]];
//		}
//		//PC2 56 --> 48
//		for (size_t i = 0; i < 48; i++)
//			ret[r][i] = k56[PC2[i] - 1];
//	}
//}
//
//// 输入轮密钥 进行加解密
//B64 des_(const B64& block, B48* keys, bool mode)
//{
//	B64 ret;
//	B32 L, R, f;
//	B48 ex_R;
//	//IP置换
//	for (size_t i = 0; i < 32; ++i)
//		L[i] = block[IP[i] - 1];
//	for (size_t i = 0; i < 32; ++i)
//		R[i] = block[IP[i + 32] - 1];
//
//	for (size_t r = 0; r < 16; r++)
//	{
//		if (mode == ENCRYPT) {
//			L ^= p_box(
//				s_box(
//					e_box(R) ^ keys[r]
//				)
//			);
//		}
//		else {
//			L ^= p_box(
//				s_box(
//					e_box(R) ^ keys[15 - r]
//				)
//			);
//		}
//
//
//		if (r != 15) {
//			L ^= R;
//			R ^= L;
//			L ^= R;
//		}
//
//	}
//
//	//IP���û�
//	int tmp;
//	for (size_t i = 0; i < 64; i++)
//		if ((tmp = IP_R[i] - 1) < 32) {
//			ret[i] = L[tmp];
//		}
//		else {
//			ret[i] = R[tmp - 32];
//		}
//	return ret;
//}
//
//B64 reverse(const B64& in) {
//	B64 ret;
//	for (size_t i = 0; i < 64; i++)
//	{
//		ret[i] = in[63 - i];
//	}
//	return ret;
//}
//
//B48 reverse(const B48& in) {
//	B48 ret;
//	for (size_t i = 0; i < 48; i++)
//	{
//		ret[i] = in[47 - i];
//	}
//	return ret;
//}
//
//B64 des(const B64& block, const B64& key, bool mode) {
//	// 由于 bitset 类型是 小端存储
//	B64 block_ = reverse(block);
//	B64 key_ = reverse(key);
//
//	B48* keys = new B48[16];
//	key_gen(key_, keys);
//	B64 ret = reverse(des_(block_, keys, mode));
//
//	delete[] keys;
//	return ret;
//}