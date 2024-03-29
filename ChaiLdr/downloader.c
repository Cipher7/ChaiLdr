#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include "include/common.h"

DWORD Download(char** response, PVOID url, PVOID endpoint, BOOL ssl)
{
	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;
	const DWORD bufferSize = 1024;
	char buffer[1024];

	HINTERNET hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hInternet == NULL)
		return -1;

	// connect to remote server
	HINTERNET hConnect = NULL;
	if(ssl)
		hConnect = InternetConnectA(hInternet, url, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);
	else
		hConnect = InternetConnectA(hInternet, url, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);

	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return -1;
	}

	HINTERNET hRequest = NULL;
	if(ssl)
		hRequest = HttpOpenRequestA(hConnect, "GET", endpoint , NULL, NULL, NULL, (INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE), 0);
	else
		hRequest = HttpOpenRequestA(hConnect, "GET", endpoint, NULL, NULL, NULL, (INTERNET_FLAG_DONT_CACHE), 0);

	DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

	InternetSetOptionA(hRequest, INTERNET_OPTION_SECURITY_FLAGS,&flags,sizeof(flags));

	BOOL status = HttpSendRequestA(hRequest,NULL,0,NULL,0);
	if (!status)
		printf("ERROR!!!!!! :: %d\n",GetLastError());

	DWORD dwBytesRead = NULL;
	SIZE_T sSize = 0;

	*response = (char*)malloc(1);
	do {
		if (InternetReadFile(hRequest, buffer, bufferSize, &bytesRead)) {
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

	InternetCloseHandle(hInternet);
	InternetCloseHandle(hRequest);
	return totalBytesRead;
}