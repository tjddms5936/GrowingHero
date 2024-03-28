#pragma once
#include "Types.h"


// MMO에서는 서로 잠시만 데이터를 참조했다가 놔주는 식으로
// 동작하는 경우가 많기 때문에 스핀락 채택

/*
32비트를 사용할 것인데
상위 16비트 : WriteFlag. 즉, Lock을 휙득하고 있는 Thread의 아이디
하위 16비트 : ReadFlag. 즉, 공유해서 사용중인 Thread들의 갯수
*/
class RW_SpinLock
{
public:
	RW_SpinLock() = default;
	~RW_SpinLock() = default;
	RW_SpinLock(const RW_SpinLock&) = delete;
	RW_SpinLock& operator=(const RW_SpinLock&) = delete;

public:
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000, // 최대로 기다려줄 틱
		MAX_SPIN_COUNT = 5000, // 최대 몇 번 돌것인가
		WRITE_THREAD_MASK = 0xFFFF0000, // 상위 16비트 비트마스킹 하기 위함
		READ_COUNT_MASK = 0x0000FFFF, // 하위 16비트 비트마스킹 하기 위함
		EMPTY_FLAG = 0x00000000 // 처음엔 모든 애들이 0으로 시작
	};

public:
	void WriteLock(); 
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();


private:
	Atomic<uint32> m_LockFlag = EMPTY_FLAG;
	uint32 m_WriteCnt;
};


// LockGuard
class ReadLockGuard
{
public:
	ReadLockGuard(RW_SpinLock& Lock) : m_Lock(Lock)
	{
		m_Lock.ReadLock();
	}

	~ReadLockGuard()
	{
		m_Lock.ReadUnLock();
	}

private:
	RW_SpinLock& m_Lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(RW_SpinLock& Lock) : m_Lock(Lock)
	{
		m_Lock.WriteLock();
	}

	~WriteLockGuard()
	{
		m_Lock.WriteUnLock();
	}

private:
	RW_SpinLock& m_Lock;
};