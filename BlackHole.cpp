#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <tchar.h>
#define TEST_FILE L"c:\\test.txt"
//__stdcall
unsigned int _stdcall threadFunc(LPVOID lpParam)
{
	CreateFile(
		
		NULL,
		NULL,
		NULL,
		NULL

	);

	return;

}
void GetFirstCallFlag(int choice, int* flag)
{
	

}
void GetSecondCallFlag(int choice, int* flag)

{


}


int _tmain()
{
	HANDLE hThread;
	HANDLE fHandle;

	int FirstCallFlag;
	int SecondCallFlag;

	int choice;
	Explanation();

	printf("=>");
	scanf("%d", &choice);

	GetFirstCallFlag(choice, &FirstCallFlag);

	printf("choose the SHARED_ACCESS FLAG");
	printf("=>");

	scanf("%d", &choice);

	GetSecondCallFlag(choice, &SecondCallFlag);

	hThread = (HANDLE)_beginthreadex(NULL, 0, threadFunc, NULL, CREATE_SUSPENDED, NULL);
	fHandle = CreateFile(TEST_FILE,
			shareAccessFlag,
			desiredAccessFlag,
			NULL,
			NULL,
			NULL,
			NULL
			);

	ResumeThread(hThread);	// Resume the suspended Thread
	WaitForSingleObject(hThread, INFINITE);	// Thread Exit Waiting


	if (fHandle != NULL)
	{
		CloseHandle(fHandle);
	}
		

	return 0;
}

void Explanation()
{

	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa363874(v=vs.85).aspx
	_tprintf(L"------------------------------------------------------\n");
	_tprintf(L"This is for getting knowledge of the Sharing Violation\n");
	_tprintf(L"------------------------------------------------------\n");

	_tprintf(L"Choose the case of DESIRED ACCESS and SHARED ACCESS \n");

	_tprintf(L"1. FIRST CALL -> desired access : GENERIC_READ , shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL -> desired access : GENERIC_READ ,  shared_access : FILE_SRARE_READ\n");

	_tprintf(L"2. FIRST CALL -> desired access : GENERIC_READ , shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL -> desired access : GENERIC_READ,  shared_access : FILE_SRARE_READ\n");
	_tprintf(L"VALID SECOND CALL2 ->  desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");

	_tprintf(L"3. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	//_tprintf(L"VALID SHARED_ACCESS : GENERIC_WRITE |  FILE_SHARE_READ\n");


/*
	_tprintf(L"4. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");

	_tprintf(L"5. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");

	_tprintf(L"6. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");

	_tprintf(L"7. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");

	_tprintf(L"8. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");
	_tprintf(L"9. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");
	_tprintf(L"10. DESIRED_ACCESS : GENERIC_READ | FILE_SHARE_READ\n");
	_tprintf(L"VALID SHARED_ACCESS : GENERIC_READ | FILE_SRARE_READ\n");
	*/


}