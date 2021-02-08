#include "windows.h"
#include "iostream"

int main() 
{
	char* str = (char*)calloc(256, sizeof(char*));
	strcpy_s(str, 256, "totally oblivious");
	while(true)
	{
		std::cout << str << std::endl;
		Sleep(500);
	}
}