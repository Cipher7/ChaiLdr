#include <Windows.h>

#define TMPFILE	L"delays.tmp"

BOOL ApiHammering(DWORD Stress)
{
	WCHAR		szPath[MAX_PATH * 2];
	WCHAR		szTmpPath[MAX_PATH];
	HANDLE		hRFile = INVALID_HANDLE_VALUE;
	HANDLE		hWFile = INVALID_HANDLE_VALUE;
	DWORD		dwNumberOfBytesRead = NULL;
	DWORD		dwNumberOfBytesWritten = NULL;
	PBYTE		pRandBuffer = NULL;
	SIZE_T		sBufferSize = 0xFFFFF;
	INT			Random = 0;

	// Fetch tmp folder
	if (!GetTempPathW(MAX_PATH, szTmpPath)) {
		goto _Cleanup;
	}

	// Constructing the full file path 
	wsprintfW(szPath, L"%s%s", szTmpPath, TMPFILE);

	for (SIZE_T i = 0; i < Stress; i++)
	{
		// Creating the file in write mode
		if ((hWFile = CreateFileW(szPath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL)) == INVALID_HANDLE_VALUE) {
			goto _Cleanup;
		}

		// Allocating a buffer
		pRandBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sBufferSize);
		srand(time(NULL));
		Random = rand() % 0xFF;
		memset(pRandBuffer, Random, sBufferSize);

		// Writing the random data into the file
		if (!WriteFile(hWFile, pRandBuffer, sBufferSize, &dwNumberOfBytesWritten, NULL) || dwNumberOfBytesWritten != sBufferSize) {
			printf("[*] Written %d Bytes of %d \n", dwNumberOfBytesWritten, sBufferSize);
			goto _Cleanup;
		}

		// Clearing the buffer & closing the handle of the file
		RtlZeroMemory(pRandBuffer, sBufferSize);
		CloseHandle(hWFile);

		// Opening the file in read mode & delete when closed
		if ((hRFile = CreateFileW(szPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL)) == INVALID_HANDLE_VALUE) {
			goto _Cleanup;
		}

		// Reading the random data written before 	
		if (!ReadFile(hRFile, pRandBuffer, sBufferSize, &dwNumberOfBytesRead, NULL) || dwNumberOfBytesRead != sBufferSize) {
			printf("[*] Read %d Bytes of %d \n", dwNumberOfBytesRead, sBufferSize);
			goto _Cleanup;
		}

		RtlZeroMemory(pRandBuffer, sBufferSize);
		HeapFree(GetProcessHeap(), NULL, pRandBuffer);

		// Closing the handle of the file which will delete it
		CloseHandle(hRFile);
	}
	return TRUE;

_Cleanup:
	return FALSE;
}