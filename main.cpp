#include <iostream>
using namespace std;
#include "des.h"

using namespace std;
int main()
{
	B64 plain = 0x0123456789abcdef;
	B64 key = 0x133457799bbcdff1;

	B64 cipher = des(plain, key, ENCRYPT);
	B64 re_plain = des(cipher, key, DECRYPT);

	cout << hex;
	cout << plain.to_ullong() << endl
		<< cipher.to_ullong() << endl
		<< re_plain.to_ullong() << endl;
}