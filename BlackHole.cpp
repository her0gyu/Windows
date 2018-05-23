/**

Purpose of Production:

 For Understanding concept of DESKTOP on Windows Operating Sytem.
 For Understanding concept of EVENT OBJECT on Windows Operating System.

Program Introduction:

 When you in a Black Screen Actually you are in another Desktop 
 which I name "MyDesktop".
 The Thread made in "MyDesktop" make a message, and after the time you input
 The Main Thread set the event which makes Thread end and after Thread end
 The Waiting Works of Main thread also ended. And the Desktop Goes to Originally

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <tchar.h>

#define MY_DESKTOP TEXT("MyDesktop")
#define MY_EVENT TEXT("My Event")
#define VERSION TEXT("1.0")

HANDLE hEvent = NULL;
int bthread = 0;

unsigned int _stdcall HiddenFunc(LPVOID lpParam)
{
	/**
		This is Working Thread which makes Message in BlackHole
	*/

	BOOL bRet = FALSE;
	HDESK* hNewDesk = (HDESK*)lpParam;
	bRet = SetThreadDesktop(*hNewDesk);

	if (FALSE == bRet)
	{
		perror("SetThreadDesktop on Thread Error\n");
		exit(0);
	}

	while (WAIT_TIMEOUT == WaitForSingleObject(hEvent, 0))
	{
		MessageBox(NULL, TEXT("You are in BlackHole..."), TEXT("BlackHole"), MB_OK);
		Sleep(1000);
	}

	return 0;

}

int _tmain()
{

	HDESK hNewDesk = NULL;
	HDESK hOrgDesk = NULL;
	HDESK hOrgInputDesk = NULL;
	HANDLE hThread = NULL;
	BOOL bRet = FALSE;
	DWORD black_hole_time = 0;
	
	
	_tprintf(L" *** This is the BlackHole Program ***\n");
	_tprintf(L" VERSION %s mady by YoungJoon\n", VERSION);
	_tprintf(L" This is Eductation For Windows Desktop Concept with Windows Event Object\n");
	_tprintf(L" Enjoy :p\n");

	_tprintf(L"Input the Blackhole Time\n");
	_tprintf(L"DO NOT SO MUCH TIME YOU'LL BE IN TROUBLE)\n");
	_tprintf(L"==>");
	_tscanf(L"%d", &black_hole_time);
	black_hole_time = black_hole_time * 1000;


	/**

	Remember!

	Signalled -> Green Light
	Non-signalled -> Reg Light

	You can pass the WaitForSingleObject API on Signalled State, becuase it is green light

	This is the Not Manually Reset Event and Default Signalled Event
	*/

	hEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		MY_EVENT
	);

	if (NULL == hEvent)
	{
		perror("CreateEvent Error\n");
		exit(0);
	}

	// My Real Desktop 
	hOrgInputDesk = OpenInputDesktop(
			0, 
			FALSE, 
			DESKTOP_SWITCHDESKTOP);

	if (NULL == hOrgInputDesk)
	{
		perror("OpenInputDesktop Error\n");
		exit(0);
	}

	// This is the New Desktop
	hNewDesk = CreateDesktop(
		MY_DESKTOP,
		NULL,
		NULL,
		0,
		GENERIC_ALL,
		NULL);
	
	if(NULL == hNewDesk)
	{
		perror("CreateDesktop Failed\n");
		exit(0);
	}

	// Real Thread Desktop
	hOrgDesk = GetThreadDesktop(GetCurrentThreadId());

	if (NULL == hOrgDesk)
	{
		perror("GetThreadDesktop Faild\n");
		_tprintf(L"error : %d", GetLastError());
		exit(0);
	}

	// Start the Thread
	hThread = (HANDLE)_beginthreadex(NULL, 0, HiddenFunc, &hNewDesk, CREATE_SUSPENDED, NULL);

	if (NULL == hThread)
	{
		perror("Thread Made Error");
		exit(0);
	}

	// Resume it
	ResumeThread(hThread);
	if (!SetThreadDesktop(hNewDesk))
	{
		perror("SetThreadDesktop Error\n");
		exit(0);
	}

	// Switch into New Desktop (= BlackHole)
	bRet = SwitchDesktop(hNewDesk);

	if (FALSE == bRet)
	{
		perror("Switch to New Desktop Failed\n");
		_tprintf(L" error : %d", GetLastError());
		exit(0);
	}
	
	// Sleep (you are in BlackHole)
	Sleep(black_hole_time);

	// Stop Thread Event
	SetEvent(hEvent);

	// Thread stopped and Wait also stop
	WaitForSingleObject(hThread, INFINITE);

	if (FALSE == bRet)
	{
		perror("Switch to Original Desktop Failed\n");
		_tprintf(L"error : %d", GetLastError());
		exit(0);
	}

	// Go to Original Desktop
	bRet = SwitchDesktop(hOrgDesk);
	SetThreadDesktop(hOrgDesk);

	CloseDesktop(hOrgDesk);
	CloseDesktop(hNewDesk);

	_tprintf(L" I am alive :)\n\n");
	return 0;
}