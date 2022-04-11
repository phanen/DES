//#include <iostream>
//#include<fstream>
//#include<iomanip>
//#include "des_bitset.h"
//#include <chrono>
//
//using namespace std;
//
//int main()
//{
//	//B64 plain = 0x0123456789abcdef;
//	//B64 key = 0x133457799bbcdff1;
//	B64 input_buf = 0x02468aceeca86420;
//	B64 key = 0xff14717847d8e859;
//
//	B64 output_buf = des(input_buf, key, ENCRYPT);
//	B64 decrypt_buf = des(output_buf, key, DECRYPT);
//
//	// formatted output
//	cout << hex;
//	cout << setfill('0');
//	//cout << setw(16) << input_buf.to_ullong() << endl
//	//	<< setw(16) << output_buf.to_ullong() << endl
//	//	<< setw(16) << decrypt_buf.to_ullong() << endl;
//
//	uint64_t lst[8] = {
//	0x5a60a81235789abd,
//	0x5a60a91235789abd,
//	0x5a61a81235789abd,
//	0x5a61a91235789abd,
//	0x5b60a81235789abd,
//	0x5b60a91235789abd,
//	0x5b61a81235789abd,
//	0x5b61a91235789abd
//	};
//	for (size_t i = 0; i < 8; i++)
//	{
//		key = lst[i];
//		cout << des(0x1ull, key, ENCRYPT).to_ullong() << endl;
//	}
//
//
//
//	//#define TIME
//#ifdef TIME // time measure 
//	uint64_t tmp;
//	ifstream fin("testbench.txt");
//	//pre_save keys
//	B48* keys = new B48[16];
//	key_gen(reverse(key), keys);
//
//	auto begin = std::chrono::high_resolution_clock::now();
//	size_t cnt = 0;
//	while (fin >> tmp) {
//		//des(plain, key, ENCRYPT);
//		input_buf = tmp;
//		output_buf = reverse(des_(reverse(input_buf), keys, false));
//	}
//	auto end = std::chrono::high_resolution_clock::now();
//	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//	std::printf("Time measured: %.9f seconds.\n", elapsed.count() * 1e-9);
//#endif // T2
//
//}