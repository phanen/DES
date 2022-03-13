#include <iostream>
#include<fstream>
#include<iomanip>
#include "des.h"

using namespace std;


int main()
{
	////B64 plain = 0x0123456789abcdef;
	////B64 key = 0x133457799bbcdff1;
	//B64 plain = 0x02468aceeca86420;
	//B64 key = 0xff14717847d8e859;

	//B64 cipher = des(plain, key, ENCRYPT);
	//B64 re_plain = des(cipher, key, DECRYPT);

	////格式化输出
	//cout << hex;
	//cout << setfill('0');
	//cout << setw(16) << plain.to_ullong() << endl
	//	<< setw(16) << cipher.to_ullong() << endl
	//	<< setw(16) << re_plain.to_ullong() << endl;
	

	B64 plain;
	B64 key = 0x133457799bbcdff1;
	
	long long tmp;
	ifstream fin("testbench.txt.txt");
	if (!fin.good()) {
		cout << "fail to open file" <<endl;
	}
	while (fin >> tmp) {
		plain = tmp;
		cout << plain;
	}
		
	//cout << plain;


}