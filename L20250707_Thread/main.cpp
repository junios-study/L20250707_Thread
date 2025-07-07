#include <iostream>
#include <Windows.h>
#include <process.h>

CRITICAL_SECTION CS;
volatile LONG BasicSpinlock = 0;
volatile LONG BasicSpinlock2 = 0;

int Bank = 0;


void InitializeSpinlock(volatile LONG* lock)
{
	*lock = 0; // 0 : «Æ∏≤, 1 : ¿·±Ë
}

void EnterSpinlock(volatile LONG* lock)
{
	//not atomic
	//while( *lock == 0)
	//{
	//}
	//*lock = 1;

	//CAS(Compare And Swap)
	while (InterlockedCompareExchange(lock, 1, 0) != 0)
	{
		//Sleep(0);
	}
}

void LeaveSpinlock(volatile LONG* lock)
{
	InterlockedExchange(lock, 0);
}

void DeleteSpinlock(volatile LONG* lock)
{
	*lock = 0;
}



//race condition
unsigned Increase(void* Arg)
{

	for (int i = 0; i < 1000000; ++i)
	{
		EnterSpinlock(&BasicSpinlock2);
		EnterSpinlock(&BasicSpinlock);
		Bank++;
		LeaveSpinlock(&BasicSpinlock);
		LeaveSpinlock(&BasicSpinlock2);
	}

	return 0;
}

//race condition
unsigned Decrease(void* Arg)
{

	for (int i = 0; i < 1000000; ++i)
	{
		EnterSpinlock(&BasicSpinlock);
		EnterSpinlock(&BasicSpinlock2);
		Bank--;
		LeaveSpinlock(&BasicSpinlock);
		LeaveSpinlock(&BasicSpinlock2);
	}

	return 0;
}

int main()
{
	InitializeSpinlock(&BasicSpinlock);
	InitializeSpinlock(&BasicSpinlock2);
	//InitializeCriticalSection(&CS);
	//InitializeCriticalSection(&CS);


	HANDLE ThreadHandles[2];
	ThreadHandles[0] =
			(HANDLE)_beginthreadex(	0, 0, Increase, 0, 0,
				0);
	ThreadHandles[1] =
		(HANDLE)_beginthreadex(0, 0, Decrease, 0, 0,
			0);
	
	WaitForMultipleObjects(2, ThreadHandles, TRUE, INFINITE);

	std::cout << Bank << std::endl;

	//DeleteCriticalSection(&CS);
	DeleteSpinlock(&BasicSpinlock);
	DeleteSpinlock(&BasicSpinlock2);

	return 0;
}
