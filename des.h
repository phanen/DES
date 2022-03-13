#pragma once
#include <iostream>
#include <bitset>
#define ENCRYPT false //加密
#define DECRYPT true //解密

typedef std::bitset<64> B64;
typedef std::bitset<56> B56;
typedef std::bitset<48> B48;

typedef std::bitset<32> B32;
//typedef std::bitset<28> B28;
//typedef std::bitset<24> B24;



B48 e_box(const B48& in); //E拓展
B32 s_box(const B48& in);	//S混淆
B32 p_box(const B32& in);	//P置换

void key_gen(const B64& k64, B48* keys);	//密钥生成
B64 des_(const B64& block, B48* keys, bool mode = ENCRYPT);	//DES调用  指定密钥组(48 * 16)
B64 des(const B64& block, const B64& key, bool mode = ENCRYPT);	//DES加密 指定密钥(64)

//输出逆序 B64  解决B64 小端存储问题
B64 reverse(const B64& in);
B48 reverse(const B48& in);
