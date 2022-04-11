#pragma once
#include<iostream>
using namespace std;
#include "des_implementation.hpp"



uint64_t sub_key[16]{ 0 };

// generate the difference distribution table (DDT)
// and possible input table -- given each specific xor input and xor output
size_t ddt_n[8][0x3f + 1][0x0f + 1];
vector<uint8_t> ddt_v[8][0x3f + 1][0x0f + 1];


// policy I - [deprecated!!!]
// keys[i][j] denote key's possible j-th to (j+6)-th bits 
// deducted from the i-th plain-cipher pair
//set<uint8_t> keys_filter[2][8];
//static int attack_idx = 0;

// policy II:
// bitmaps[i] denote the bitmap of Si-Box
// the k-th bit is 1 <--> k is a key in the bitmap
// (count from right, base index is 0)
vector<uint64_t> bitmaps(8, 0xffffffffffffffff);

vector<uint8_t> box_keys[8];
vector<uint64_t> possible_keys;



// show the possible key
static void log_bitmap() {
	for (size_t i = 0; i < 8; i++)
		cout << hex << i << ' '
		<< setw(16) << bitmaps[i] << endl;
	cout << endl;
}

// log the info of ddt
static void log_ddt() {
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 0x3f + 1; j++)
			for (size_t k = 0; k < 0x0f + 1; k++) {
				cout << dec << ddt_n[i][j][k];
				cout << ((k == 0x0f) ? "\n" : ", ");
			}		cout << endl
				<< ",\n";
	}
}



// show possible key for each S-Box
static void log_box_key() {
	cout << hex;
	for (size_t box_idx = 0; box_idx < 8; box_idx++)
	{
		cout << "possible key for S" << box_idx + 1 << " (" << box_keys[box_idx].size() <<")" << endl;
		for (uint8_t key : box_keys[box_idx])
			cout << setw(2)<< int(key) << " ";
		cout << endl;
	}

}


// show all the complete possible keys
static void log_key() {
	cout << "possible 48-bit key for the 3rd round "<< "(" << dec <<possible_keys.size() << ")" << endl;
	cout << hex;
	for (const auto& key : possible_keys)
		cout << key << endl;
}

// log the process to get xor-input and xor-output
static void log_instance() {

	uint64_t p0 = 0x80cb4f7c993b1782;
	uint64_t p1 = 0xebeb4af9993b1782;
	uint64_t c0 = 0x746a0efd1ee535d4;
	uint64_t c1 = 0x626106e0607ec3bd;

	cout << "d plain\t\t\t" << hex << (p0 ^ p1) << endl;
	cout << "d cipher\t\t" << hex << (c0 ^ c1) << endl;
	cout << "before Ex\t\t" << hex << uint32_t((c0 ^ c1) >> 32) << endl;
	cout << "S in\t\t\t" << hex << e_op(uint32_t((c0 ^ c1) >> 32)) << endl;

	uint32_t tmp = (uint32_t((p0 ^ p1) >> 32) ^ uint32_t((c0 ^ c1) & L32_IN64));
	uint32_t ap = 0;
	for (size_t i = 0; i < 32; i++)
	{
		ap <<= 1;
		ap |= (tmp >> (32 - INV_P[i])) & LB_IN32;
	}
	cout << "S out\t\t\t" << hex << ap << endl;
	cout << "after Pm\t\t" << hex << tmp << endl;
}

