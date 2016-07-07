#include <iostream>

using namespace std;

bool is_little_endian()
{
	unsigned short i = 1;//0x01
	return *((char*)(&i)) ? true : false;
}

int main(void)
{
	cout << "Check the local endian:" << endl;
	cout << (is_little_endian() ? "little" : "big") << "_endian" << endl;
	return 0;
}
