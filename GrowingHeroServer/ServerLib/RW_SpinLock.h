#pragma once
#include "Types.h"


// MMO������ ���� ��ø� �����͸� �����ߴٰ� ���ִ� ������
// �����ϴ� ��찡 ���� ������ ���ɶ� ä��

/*
32��Ʈ�� ����� ���ε�
���� 16��Ʈ : WriteFlag. ��, Lock�� �׵��ϰ� �ִ� Thread�� ���̵�
���� 16��Ʈ : ReadFlag. ��, �����ؼ� ������� Thread���� ����
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
		ACQUIRE_TIMEOUT_TICK = 10000, // �ִ�� ��ٷ��� ƽ
		MAX_SPIN_COUNT = 5000, // �ִ� �� �� �����ΰ�
		WRITE_THREAD_MASK = 0xFFFF0000, // ���� 16��Ʈ ��Ʈ����ŷ �ϱ� ����
		READ_COUNT_MASK = 0x0000FFFF, // ���� 16��Ʈ ��Ʈ����ŷ �ϱ� ����
		EMPTY_FLAG = 0x00000000 // ó���� ��� �ֵ��� 0���� ����
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