#include <iostream>
#include <Windows.h>
#include <process.h>

CRITICAL_SECTION CS;

int Bank = 0;

//race condition
unsigned Increase(void* Arg)
{
	for (int i = 0; i < 1000000; ++i)
	{
		//atomic º¸Àå
		EnterCriticalSection(&CS);
		Bank++; 
		LeaveCriticalSection(&CS);
	}

	return 0;
}

int main()
{
	InitializeCriticalSection(&CS);

	//Increase(NULL);
	//Increase(NULL);
	HANDLE ThreadHandles[4];
	for (int i = 0; i < 4; ++i)
	{
		ThreadHandles[i] =
			(HANDLE)_beginthreadex(	0, 0, Increase, 0, CREATE_SUSPENDED,
				0);
	}

	ResumeThread(ThreadHandles[0]);

	Sleep(2000);

	std::cout << Bank << std::endl;

	DeleteCriticalSection(&CS);

	return 0;
}
