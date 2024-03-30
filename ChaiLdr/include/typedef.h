#pragma once

#include <Windows.h>
#include <wininet.h>

typedef HINTERNET(WINAPI* fnInternetOpenA)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);

typedef ULONGLONG(WINAPI* fnGetTickCount64)();

typedef HINTERNET(WINAPI* fnInternetConnectA)(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUsername, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);

typedef HINTERNET(WINAPI* fnHttpOpenRequestA)(HINTERNET hConnect, LPCSTR lpszVerb, LPCSTR lpszObjectName, LPCSTR lpszVersion, LPCSTR lpszReferrer, LPCSTR* lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);

typedef BOOL(WINAPI* fnInternetSetOptionA)(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);

typedef BOOL(WINAPI* fnHttpSendRequestA)(HINTERNET hRequest, LPCSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);

typedef BOOL(WINAPI* fnInternetReadFile)(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);

typedef BOOL(WINAPI* fnInternetCloseHandle)(HINTERNET hInternet);

typedef HMODULE(WINAPI* fnLoadLibraryA)(LPCSTR lpLibFileName);