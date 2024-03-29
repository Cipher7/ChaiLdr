#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include "include/common.h"

DWORD Download(char** response, PVOID url)
{
	DWORD bytesRead = 0;
	DWORD totalBytesRead = 0;
	const DWORD bufferSize = 1024;
	char buffer[1024];

	HINTERNET hInternet = InternetOpenA(NULL, NULL, NULL, NULL, NULL);

	HINTERNET hInternetFile = InternetOpenUrlA(hInternet, url, NULL, NULL, (INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_UNKNOWN_CA), NULL);

	DWORD dwBytesRead = NULL;
	SIZE_T sSize = 0;

	*response = (char*)malloc(1);
	do {
		if (InternetReadFile(hInternetFile, buffer, bufferSize, &bytesRead)) {
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
	InternetCloseHandle(hInternetFile);
	InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	return totalBytesRead;
}