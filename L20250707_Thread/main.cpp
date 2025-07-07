#include <iostream>
#include <Windows.h>
#include <process.h>

CRITICAL_SECTION CS;
volatile LONG BasicSpinlock = 0;

int Bank = 0;


void InitializeSpinlock(volatile LONG* lock)
{
	*lock = 0; // 0 : 풀림, 1 : 잠김
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
		Sleep(0);
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
		//atomic 보장
		//lock 객체 -> spinlock, adaptive spinlock, backoff spinlock
		//EnterCriticalSection(&CS);
		EnterSpinlock(&BasicSpinlock);
		Bank++;
		//context swiching, 진짜로 동시 처리 문제
		//Register = Bank(메모리)
		//Register = Register + 1
		//Bank = Register;
		//LeaveCriticalSection(&CS);
		LeaveSpinlock(&BasicSpinlock);

	}


	return 0;
}

int main()
{
	InitializeSpinlock(&BasicSpinlock);
	//InitializeCriticalSection(&CS);


	HANDLE ThreadHandles[4];
	for (int i = 0; i < 4; ++i)
	{
		ThreadHandles[i] =
			(HANDLE)_beginthreadex(	0, 0, Increase, 0, 0,
				0);
	}

	Sleep(2000);

	std::cout << Bank << std::endl;

	//DeleteCriticalSection(&CS);
	DeleteSpinlock(&BasicSpinlock);

	return 0;
}
