/*

	Made by Young Joon 2018/05/21

	This is Educational Program getting to know of 'Sharing Violation'

	You can know which second CreateFile Call acceptable
	
	Before Testing Plese Read the site below 

	https:\/\/msdn.microsoft.com\/en-us\/library\/windows\/desktop\/aa363874(v=vs.85).aspx

	After you read this site, You naturally know of "how to use this program"

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <tchar.h>

#define TEST_FILE L"C:\\test.txt"
#define MAX_STRING_LEN 50
#define VERSION L"1.0"

typedef struct _FileOpenFlags {
	int desiredFlag;
	int sharedFlag;
	TCHAR* dStrings;
	TCHAR* sStrings;

} FileOpenFlags, *PFileOpenFlags;

int DesiredFlags[9] = { GENERIC_READ , GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE};
TCHAR* DesiredFlagsStr[9] = { L"GENERIC_READ", L"GENERIC_WRITE" ,L"GENERIC_READ | GENERIC_WRITE" };
int SharedFlags[9] = { FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE};
TCHAR* SharedFlagsStr[9] = { L"FILE_SHARE_READ", L"FILE_SHARE_WRITE" ,L"FILE_SHARE_READ | FILE_SHARE_WRITE" };
int g_programResult = ERROR_SUCCESS;	// there is no possibilty of multi thread accessing

unsigned int _stdcall threadFunc(LPVOID lpParam)
{
	PFileOpenFlags flags = (PFileOpenFlags)lpParam;
	HANDLE handle = 0;
	Sleep(1000);
	handle = CreateFile(
		TEST_FILE,
		flags->desiredFlag,
		flags->sharedFlag,
		NULL,
		OPEN_ALWAYS,
		NULL,
		NULL);

	_tprintf(L"second file opend occured\n");
	_tprintf(L"handle : %d\n", handle);
	_tprintf(L"error: %d\n", GetLastError());
	Sleep(1000);
	if (INVALID_HANDLE_VALUE != handle)
	{
		CloseHandle(handle);
	}

	if (INVALID_HANDLE_VALUE == handle)
	{
		if (ERROR_SHARING_VIOLATION == GetLastError())
		{
			g_programResult =  ERROR_SHARING_VIOLATION;
			return 0;
		}
		else
		{
			g_programResult = ERROR_INTERNAL_ERROR;
			return 0;
		}
	}

	g_programResult = ERROR_SUCCESS;
	return 0;

}

void Explanation();
void PrintOptions();
void Intro();
void PrintResult(PFileOpenFlags first_flags, PFileOpenFlags second_flags, int result);
int GetFlags(int choice, PFileOpenFlags flags);

int _tmain()
{
	HANDLE hThread;
	HANDLE fHandle;

	PFileOpenFlags first_call_flags;
	PFileOpenFlags second_call_flags;
	char bMore = 'Y';

	int choice;
	int ret;
	first_call_flags = (PFileOpenFlags)malloc(sizeof(FileOpenFlags));
	second_call_flags = (PFileOpenFlags)malloc(sizeof(FileOpenFlags));

	if (first_call_flags == NULL || 
		second_call_flags == NULL)
	{
		if (first_call_flags != NULL) free(first_call_flags);
		if (second_call_flags != NULL) free(second_call_flags);

		perror("not enough memory");
		exit(0);
	}
	
	first_call_flags->dStrings = (TCHAR*)malloc(MAX_STRING_LEN);
	first_call_flags->sStrings = (TCHAR*)malloc(MAX_STRING_LEN);

	second_call_flags->dStrings = (TCHAR*)malloc(MAX_STRING_LEN);
	second_call_flags->sStrings = (TCHAR*)malloc(MAX_STRING_LEN);

	if (first_call_flags->dStrings == NULL ||
		first_call_flags->sStrings == NULL ||
		second_call_flags->dStrings == NULL ||
		second_call_flags->sStrings == NULL
		)
	{
		if (first_call_flags->dStrings != NULL) free(first_call_flags->dStrings);
		if (first_call_flags->sStrings != NULL) free(first_call_flags->sStrings);
		if (second_call_flags->dStrings != NULL) free(second_call_flags->dStrings);
		if (second_call_flags->sStrings != NULL) free(second_call_flags->sStrings);

		perror("not enough memory");
		exit(0);
	}
	Intro();
	while (bMore == 'Y' || bMore == 'y')
	{
		Explanation();
		PrintOptions();
		printf("choose the First Call Flag");
		printf("=>");
		scanf("%d", &choice);

		if (ERROR_INVALID_PARAMETER == GetFlags(choice, first_call_flags))
		{
			printf("Invliad choice \n");
			printf("=>");
			scanf("%d", &choice);
		}

		printf("choose the Second Call Flag");
		printf("=>");
		scanf("%d", &choice);

		if (ERROR_INVALID_PARAMETER == GetFlags(choice, second_call_flags))
		{
			printf("Invliad choice \n");
			printf("=>");
			scanf("%d", &choice);
		}

		hThread = (HANDLE)_beginthreadex(NULL, 0, threadFunc, second_call_flags, CREATE_SUSPENDED, NULL);

		fHandle = CreateFile(TEST_FILE,
			first_call_flags->desiredFlag,
			first_call_flags->sharedFlag,
			NULL,
			OPEN_ALWAYS,
			NULL,
			NULL
		);
		if (INVALID_HANDLE_VALUE == fHandle)
		{
			perror("File Not Opend\n");
			exit(0);
		}


		_tprintf(L"first file opend occured\n");
		_tprintf(L"handle : %d\n", fHandle);
		_tprintf(L"error: %d\n", GetLastError());

		ResumeThread(hThread);	// Resume the suspended Thread
		WaitForSingleObject(hThread, INFINITE);	// Thread Exit Waiting
		PrintResult(first_call_flags, second_call_flags, g_programResult);
		

		if (fHandle != NULL)
		{
			CloseHandle(fHandle);
		}
		
		_tprintf(L"Do you need more lesson? (Y/N)\n");
		fflush(stdin);
		bMore = getchar();
		bMore = getchar();
	}

	if(NULL != first_call_flags->dStrings) free(first_call_flags->dStrings);
	if(NULL != first_call_flags->sStrings) free(first_call_flags->sStrings);
	if(NULL != second_call_flags->dStrings) free(second_call_flags->dStrings);
	if(NULL != second_call_flags->sStrings) free(second_call_flags->sStrings);
	if (NULL != first_call_flags) free(first_call_flags);
	if (NULL != second_call_flags) free(second_call_flags);

	_tprintf(L"Bye! :p\n");
	return 0;
}
int GetFlags(int choice, PFileOpenFlags flags)
{
	switch (choice)
	{
		case 1:
			flags->desiredFlag = DesiredFlags[0];
			flags->sharedFlag = SharedFlags[0];
			_tcscpy(flags->dStrings, DesiredFlagsStr[0]);
			_tcscpy(flags->sStrings, SharedFlagsStr[0]);
			flags->sStrings;
			break;
		case 2:
			flags->desiredFlag = DesiredFlags[0];
			flags->sharedFlag = SharedFlags[1];
			_tcscpy(flags->dStrings, DesiredFlagsStr[0]);
			_tcscpy(flags->sStrings, SharedFlagsStr[1]);
			break;

		case 3:
			flags->desiredFlag = DesiredFlags[0];
			flags->sharedFlag = SharedFlags[2];
			_tcscpy(flags->dStrings, DesiredFlagsStr[0]);
			_tcscpy(flags->sStrings, SharedFlagsStr[2]);
			break;

		case 4:
			flags->desiredFlag = DesiredFlags[1];
			flags->sharedFlag = SharedFlags[0];
			_tcscpy(flags->dStrings, DesiredFlagsStr[1]);
			_tcscpy(flags->sStrings, SharedFlagsStr[0]);
			break;

		case 5:
			flags->desiredFlag = DesiredFlags[1];
			flags->sharedFlag = SharedFlags[1];
			_tcscpy(flags->dStrings, DesiredFlagsStr[1]);
			_tcscpy(flags->sStrings, SharedFlagsStr[1]);
			break;

		case 6:
			flags->desiredFlag = DesiredFlags[1];
			flags->sharedFlag = SharedFlags[2];
			_tcscpy(flags->dStrings, DesiredFlagsStr[1]);
			_tcscpy(flags->sStrings, SharedFlagsStr[2]);
			break;

		case 7:
			flags->desiredFlag = DesiredFlags[2];
			flags->sharedFlag = SharedFlags[0];
			_tcscpy(flags->dStrings, DesiredFlagsStr[2]);
			_tcscpy(flags->sStrings, SharedFlagsStr[0]);
			break;

		case 8:
			flags->desiredFlag = DesiredFlags[2];
			flags->sharedFlag = SharedFlags[1];
			_tcscpy(flags->dStrings, DesiredFlagsStr[2]);
			_tcscpy(flags->sStrings, SharedFlagsStr[1]);
			break;

		case 9:
			flags->desiredFlag = DesiredFlags[2];
			flags->sharedFlag = SharedFlags[2];
			_tcscpy(flags->dStrings, DesiredFlagsStr[2]);
			_tcscpy(flags->sStrings, SharedFlagsStr[2]);
			break;

		default:
			return ERROR_INVALID_PARAMETER;
	}

	return ERROR_SUCCESS;
}

void Intro()
{
	_tprintf(L"********************************************** Intro **********************************************\n");
	_tprintf(L"Sharing Violation Education V %s\n", VERSION);
	_tprintf(L"This program is made for just educatoin enjoy :P\n");
	_tprintf(L"Referenced by https:\/\/msdn.microsoft.com\/en-us\/library\/windows\/desktop\/aa363874(v=vs.85).aspx\n");
	_tprintf(L"(note) delete flags not included\n");
	_tprintf(L"**************************************************************************************************\n");

}

void Explanation()
{
	_tprintf(L"********************************************** HERE IS THE CHEAT SHEET **********************************************\n");
	_tprintf(L"1. FIRST CALL -> desired access : GENERIC_READ , shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ ,  shared_access : FILE_SRARE_READ\n\n");

	_tprintf(L"2. FIRST CALL -> desired access : GENERIC_READ , shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ,  shared_access : FILE_SRARE_READ\n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"3. FIRST CALL ->  desired access : GENERIC_READ,  shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ \n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL3 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL4 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL5 -> desired access : GENERIC_READ | GENERIC_WRITE , shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL6 -> desired access : GENERIC_READ | GENERIC_WRITE , shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"4. FIRST CALL ->  desired access : GENERIC_WRITE,  shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_WRITE \n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"5. FIRST CALL ->  desired access : GENERIC_WRITE,  shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"6. FIRST CALL ->  desired access : GENERIC_WRITE,  shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_WRITE \n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL3 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL4 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL5 -> desired access : GENERIC_READ | GENERIC_WRITE , shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL6 -> desired access : GENERIC_READ | GENERIC_WRITE , shared_access : , FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"7. FIRST CALL ->  desired access : GENERIC_READ | GENERIC_WRITE,  shared_access : FILE_SHARE_READ\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"8. FIRST CALL ->  desired access : GENERIC_READ | GENERIC_WRITE,  shared_access : FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n\n");

	_tprintf(L"9. FIRST CALL ->  desired access : GENERIC_READ | GENERIC_WRITE,  shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL1 -> desired access : GENERIC_READ, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");
	_tprintf(L"VALID SECOND CALL2 -> desired access : GENERIC_READ | GENERIC_WRITE, shared_access : FILE_SHARE_READ | FILE_SHARE_WRITE");
	_tprintf(L"*************************************************************************************************\n\n");
}

void PrintOptions(){	_tprintf(L"********************************************** MENU **********************************************\n");	_tprintf(L"1: desired access -> GENERIC_READ,  shared access : FILE_SHARE_READ\n");	_tprintf(L"2: desired access -> GENERIC_READ,  shared access : FILE_SHARE_WRITE\n");	_tprintf(L"3: desired access -> GENERIC_READ,  shared access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");	_tprintf(L"4: desired access -> GENERIC_WRITE,  shared access : FILE_SHARE_READ\n");	_tprintf(L"5: desired access -> GENERIC_WRITE,  shared access : FILE_SHARE_WRITE\n");	_tprintf(L"6: desired access -> GENERIC_WRITE,  shared access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");	_tprintf(L"7: desired access -> GENERIC_READ | GENERIC_WRITE,  shared access : FILE_SHARE_READ\n");	_tprintf(L"8: desired access -> GENERIC_READ | GENERIC_WRITE,  shared access : FILE_SHARE_WRITE\n");	_tprintf(L"9: desired access -> GENERIC_READ | GENERIC_WRITE,  shared access : FILE_SHARE_READ | FILE_SHARE_WRITE\n");	_tprintf(L"*************************************************************************************************\n\n");}void PrintResult(PFileOpenFlags first_flags, PFileOpenFlags second_flags, int result)
{
	_tprintf(L"********************************************** RESULT **********************************************\n");
	_tprintf(L"You Choosed Flags Below \n");
	_tprintf(L"FIRST CALL ->  desired access : %s , shared_access : %s\n", first_flags->dStrings, first_flags->sStrings);
	_tprintf(L"SECOND CALL -> desired access : %s ,  shared_access : %s\n", second_flags->dStrings, second_flags->sStrings);

	switch (result)
	{
		case ERROR_SHARING_VIOLATION:
			_tprintf(L"Result : Sharing Violation Occured \n");
			break;

		case ERROR_INTERNAL_ERROR:
			_tprintf(L"Result :  Awkward.. Strange Result Occured \n");
			break;

		case ERROR_SUCCESS:
			_tprintf(L"Result :  Successfully Second File Opend \n");
			break;
	}
	_tprintf(L"*************************************************************************************************\n\n");
	return;
}
