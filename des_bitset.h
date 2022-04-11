#pragma once
#include <bitset>
// 加解密模式
#define ENCRYPT false
#define DECRYPT true 

// 按位长定义块类型
typedef std::bitset<64> B64;
typedef std::bitset<56> B56;
typedef std::bitset<48> B48;
typedef std::bitset<32> B32;
//typedef std::bitset<28> B28;
//typedef std::bitset<24> B24;

B48 e_box(const B48& in);   //E 拓展
B32 s_box(const B48& in);	//S 盒
B32 p_box(const B32& in);	//P 置换

void key_gen(const B64& k64, B48* keys);	//��Կ����
B64 des_(const B64& block, B48* keys, bool mode = ENCRYPT);	//des 轮密钥加密，底层
B64 des(const B64& block, const B64& key, bool mode = ENCRYPT);	// des 密钥加密

// 逆序化
B64 reverse(const B64& in);
B48 reverse(const B48& in);
