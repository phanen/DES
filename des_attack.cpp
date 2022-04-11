#include <iostream>
//#include <fstream>
#include <iomanip> // format the console output
#include <chrono> //time measure

#include <vector>
//#include <algorithm> // set_intersection
#include <map>	// hashTable
// #include <set>
#include "des_attack_log.hpp"
using namespace std;



void des_bf_attack(uint64_t plain, uint64_t cipher)
{
	cout << "input:\t" << setw(16) << plain << endl;
	cout << "output:\t" << setw(16) << cipher << endl;

	size_t cnt = 0;
	std::printf("possible key:\n");
	auto begin = std::chrono::high_resolution_clock::now();
	uint64_t tmp = 0;
	for (uint64_t key = 0x0000001235789abd; key < 0xffffff1235789abd; key += 0x0000010000000000)
	{
		key_gen(key, sub_key);
		tmp = des(plain, sub_key, 'e');
		if (tmp == cipher)
			cout << "\t" << cnt++ << ":\t" << key << endl;
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::printf("Time measured: %.9f\n\n",
		std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() * 1e-9);
}

void des_tab_lookup_attack(uint64_t plain, uint64_t cipher)
{

	cout << "input:\t" << setw(16) << plain << endl;
	cout << "output:\t" << setw(16) << cipher << endl;

	// offline stage
	map<uint64_t, vector<uint64_t>> tab;
	auto begin = std::chrono::high_resolution_clock::now();
	for (uint64_t key = 0x0000001235789abd; key < 0xffffff1235789abd; key += 0x0000010000000000)
	{
		key_gen(key, sub_key);
		tab[des(plain, sub_key, 'e')].push_back(key);
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() * 1e-9;

	// online stage
	begin = std::chrono::high_resolution_clock::now();
	cout << "size of hash table:\t" << tab.size() << endl;
	vector<uint64_t> tmp = tab[cipher];
	cout << "possible key:" << endl;
	for (const auto& key : tmp)
		cout << "\t" << key << endl;
	end = std::chrono::high_resolution_clock::now();

	std::printf("Time measured[online stage]: %.9f\n", duration);
	std::printf("Time measured[offline stage]: %.9f\n\n",
		std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() * 1e-9);
}

void des2_meet_in_the_middle_attack(uint64_t plain, uint64_t cipher)
{

	cout << "input:\t" << setw(16) << plain << endl;
	cout << "output:\t" << setw(16) << cipher << endl;
	// store the hash table of the first DES
	map<uint64_t, vector<uint64_t>> tab;
	auto begin = std::chrono::high_resolution_clock::now();
	for (uint64_t key = 0x0000001235789abd; key < 0xffffff1235789abd; key += 0x0000010000000000)
	{
		key_gen(key, sub_key);
		tab[des(plain, sub_key, 'e')].push_back(key);
	}

	// decrypt the final cipher
	// to meet the first cipher in the middle
	cout << "size of hash table:\t" << tab.size() << endl;
	cout << "possible key pair:" << endl;
	uint64_t tmp;
	int cnt = 0;
	for (uint64_t key1 = 0x0000001235789abd; key1 < 0xffffff1235789abd; key1 += 0x0000010000000000)
	{
		key_gen(key1, sub_key);
		tmp = des(cipher, sub_key, 'd');
		if (tab.find(tmp) != tab.end())
		{
			cout << "meet value:\t" << tmp << endl;
			for (auto& key0 : tab[tmp])
				cout << "[" << dec << cnt++ << "]\t" << hex << "key0: " << key0 << "\tkey1: " << key1 << endl;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() * 1e-9;
	std::printf("Time measured: %.9f\n\n",
		std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() * 1e-9);
}


// generate the differetial distribution table of S-box
void gen_ddt()
{
	for (int i = 0; i < 8; i++)
	{
		//  for each xor input
		for (int d_in = 0x00; d_in < 0x3f + 1; d_in++)
		{
			// for each input, calc the xor output
			for (int input0 = 0; input0 < 0x3f + 1; input0++)
			{
				int input1 = input0 ^ d_in;
				int o0 = S[i][((input0 & 0x20) | ((input0 & 0x01) << 4)) + ((input0 >> 1) & 0x0f)];
				int o1 = S[i][((input1 & 0x20) | ((input1 & 0x01) << 4)) + ((input1 >> 1) & 0x0f)];
				// cout << int(o0 ^ o1) << ": " << input0 << "," << input1 << " " << o0 << "^" << o1 << endl;
				ddt_v[i][d_in][o1 ^ o0].push_back(input0);
			}

			// store the differential distribution table
			for (uint8_t d_out = 0; d_out < 0x0f + 1; d_out++)
				ddt_n[i][d_in][d_out] = ddt_v[i][d_in][d_out].size();
		}
	}
}


// reduce the possible key, store info of key in the bitmap
// read the global ddt_v, write the global bitmap
void des_3r_differential_reduce_key(uint64_t plain0, uint64_t cipher0, uint64_t plain1, uint64_t cipher1)
{
	// d_ := The differential operator
	uint64_t d_plain = plain0 ^ plain1;		// d_LR0
	uint64_t d_cipher = cipher0 ^ cipher1;	// d_LR3

	// e extension
	uint32_t d_e_in = (d_cipher >> 32);		// d_L3
	uint32_t e_in = (cipher0 >> 32);		// L3

	// get xor input of S-Box and one of the non-xor input
	uint64_t d_s_in = e_op(d_e_in);
	uint64_t s_in = e_op(e_in);


	// get xor output of S-Box (inverse p permutation)
	uint32_t inv_p_in = uint32_t(d_cipher & L32_IN64) ^ (d_plain >> 32);
	uint32_t d_s_out = 0; // xor output of S-Box
	for (size_t i = 0; i < 32; i++)
	{
		d_s_out <<= 1;
		d_s_out |= (inv_p_in >> (32 - INV_P[i])) & LB_IN32;
	}

	// log the xor input and output of S-Box
	// cout << hex << setw(12) << d_s_in << endl << setw(8) << d_s_out << endl;

	// for each S-box (reverse order)
	for (int b_idx = 7; b_idx >= 0; --b_idx)
	{
		uint64_t cur_bitmap = 0x0000000000000000;
		//cout << b_idx << ' ' << int(d_s_in & 0x3f) << ' ' << int(d_s_out & 0x0f) << endl;
		for (const auto& possible_input : ddt_v[b_idx][d_s_in & 0x3f][d_s_out & 0x0f])
		{
			// possible_input is key-xored
			uint8_t key = possible_input ^ (s_in & 0x3f);
			cur_bitmap |= (0x1UI64 << uint64_t(key));
		}
		bitmaps[b_idx] &= cur_bitmap;
		s_in >>= 6;
		d_s_in >>= 6;
		d_s_out >>= 4;
	}
}


// generate each box's possible key from bitmaps
void des_3r_differential_gen_box_key() {
	for (size_t b_idx = 0; b_idx < 8; b_idx++)
	{
		// for each box, use a new mask
		uint64_t mask = 0x1UI64;
		uint64_t cur_bitmap = bitmaps[b_idx];
		// shift the mask to check if the key in the bitmap
		for (uint8_t key = 0; key < 0x3f + 1; key++)
		{
			if (mask & cur_bitmap) {
				box_keys[b_idx].push_back(key);
			}
			mask <<= 1;
		}
	}
}

// generate complete possible key from each box's possible key recursively
// brief: load 0~cur_box_idx 's possible key
void des_3r_differential_gen_key_recursion(int cur_box_idx) {
	if (cur_box_idx != 0)
		des_3r_differential_gen_key_recursion(cur_box_idx - 1);
	else if (cur_box_idx == 0)
		possible_keys.push_back(0x00);

	auto copy_keys = possible_keys;
	uint64_t tmp;
	possible_keys.clear();
	auto length = box_keys[cur_box_idx].size();
	int pad = 42 - 6 * cur_box_idx;

	while (!copy_keys.empty())
	{
		tmp = copy_keys.back();
		copy_keys.pop_back();
		// pad <- [42 - n * 6]
		for (size_t i = 0; i < length; i++)
			possible_keys.push_back(tmp | (uint64_t(box_keys[cur_box_idx][i]) << pad));
	}
}


void des_3r_differential_gen_possible_key_nonrecursion() {

	uint64_t tmp = 0;
	for (size_t b_idx = 0; b_idx < 8; b_idx++)
	{
		// pre store the stack
		possible_keys.push_back(0x00);
		while (!possible_keys.empty()) {
			tmp = possible_keys.back();
			possible_keys.pop_back();
		}
	}
}



int main()
{
	// formatting the log globally
	cout << hex << setfill('0');

	// brute-force attack
	// des_bf_attack(0x0000000000000000UI64, 0xd76346397c9fb03bUI64);
	// des_bf_attack(0x0000000000000001UI64, 0xd2e7f4569792b897UI64);

	// look-up table attack
	// des_tab_lookup_attack(0x0000000000000000UI64, 0xd76346397c9fb03bUI64);
	// des_tab_lookup_attack(0x0000000000000001UI64, 0xd2e7f4569792b897UI64);

	// meet-in-the-middle attack of DES2/DDES
	// des2_meet_in_the_middle_attack(0x0000000000000000UI64, 0x3637232aaab78692UI64);
	// des2_meet_in_the_middle_attack(0x0000000000000001UI64, 0x70de8bbfe1892b7cUI64);



	//明文：0x80cb4f7c993b1782 密文：0x746a0efd1ee535d4
	//明文：0xebeb4af9993b1782 密文：0x626106e0607ec3bd
	//明文：0x54a281efdcd118c9 密文：0xb29d363555820170
	//明文：0x2812ac18dcd118c9 密文：0x6d0dc7ce3844be01

	gen_ddt();
	//log_ddt();
	// show the possible key
	log_bitmap();
	des_3r_differential_reduce_key(0x80cb4f7c993b1782, 0x746a0efd1ee535d4, 0xebeb4af9993b1782, 0x626106e0607ec3bd);
	des_3r_differential_reduce_key(0x54a281efdcd118c9, 0xb29d363555820170, 0x2812ac18dcd118c9, 0x6d0dc7ce3844be01);
	log_bitmap();
	des_3r_differential_gen_box_key();
	des_3r_differential_gen_key_recursion(7);
	log_box_key();
	log_key();

}



