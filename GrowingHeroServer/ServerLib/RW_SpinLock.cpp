#include "pch.h"
#include "RW_SpinLock.h"

void RW_SpinLock::WriteLock()
{
	// 어떤 경우에 쓰기가 허락될까?
	// case 1. 동일한 쓰레드에서 재귀적으로 함수 호출했을 때
	// case 2. Read도 Write도 아무도 사용하고 있지 않을때

	// case 1 
	const uint32 CurLockThreadID = (m_LockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (CurLockThreadID == TLS_ThreadID)
	{
		// 현재 쓰레드 ID가 기존 사용중인 쓰레드라면 통과
		// 몇번째 쓰는지는 m_WriteCnt로 관리
		m_WriteCnt++;
		return;
	}

	// case 2. 5000번은 뺑뺑이 돌면서 기다리도록 SpinLock으로 구현
	// 너무 시간 오래걸리면 Crash나게 하자
	const int64 beginTic = ::GetTickCount64();
	const uint32 desired = ((TLS_ThreadID << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 SpinCnt = 0; SpinCnt < MAX_SPIN_COUNT; SpinCnt++)
		{
			uint32 expected = EMPTY_FLAG;
			if (m_LockFlag.compare_exchange_strong(expected, desired) == true)
			{
				// m_LockFlag가 expected랑 같다면 m_LockFlag를 desired로 바꿔주기
				// 즉, 비어있었기 때문에 현재 쓰레드 아이디로 Flag 맞춰준거
				m_WriteCnt++;
				return;
			}
		}

		const int64 EndTic = ::GetTickCount64();
		if (EndTic - beginTic >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Too Much Lock Wait");

		// 시간아웃은 아니고 너무 많이 돌아서 소유권 내려놓고 잠시 쉬고오는거
		this_thread::yield(); 
	}
}

void RW_SpinLock::WriteUnLock()
{
	// ReadLock 다 풀기 전에는 WriteUnLock 불가능. 예외처리
	if ((m_LockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("WriteUnLock ERROR");

	const int32 LockCnt = --m_WriteCnt;
	if (LockCnt == 0)
		m_LockFlag.store(EMPTY_FLAG);
	
}

void RW_SpinLock::ReadLock()
{
	// 동일한 쓰레드가 쓰고 있으면 무조건 성공
	const uint32 CurLockThreadID = (m_LockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (CurLockThreadID == TLS_ThreadID)
	{
		// 현재 쓰레드 ID가 기존 사용중인 쓰레드라면 통과
		// 몇번째 쓰는지는 m_WriteCnt로 관리
		m_LockFlag.fetch_add(1); // W->R->W는 불가능
		return;
	}

	// 아무도 쓰고있지 않을 때 경합해서 공유 카운트 올리기
	const int64 beginTic = ::GetTickCount64();
	while (true)
	{
		for (uint32 SpinCnt = 0; SpinCnt < MAX_SPIN_COUNT; SpinCnt++)
		{
			uint32 expected = (m_LockFlag.load() & READ_COUNT_MASK);
			if (m_LockFlag.compare_exchange_strong(expected, expected+1) == true)
			{
				return;
			}
		}

		const int64 EndTic = ::GetTickCount64();
		if (EndTic - beginTic >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Too Much Lock Wait");

		// 시간아웃은 아니고 너무 많이 돌아서 소유권 내려놓고 잠시 쉬고오는거
		this_thread::yield();
	}
}

void RW_SpinLock::ReadUnLock()
{
	// fetch 시리즈는 결과값 이전을 return해준다.
	// 즉, 빼기 전이랑 READ_COUNT_MASK 비트마스킹 해서 0이라는건? 이미 0이라는거
	if((m_LockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("ReadUnLock ERROR");

}
