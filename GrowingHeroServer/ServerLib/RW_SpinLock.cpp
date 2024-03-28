#include "pch.h"
#include "RW_SpinLock.h"

void RW_SpinLock::WriteLock()
{
	// � ��쿡 ���Ⱑ ����ɱ�?
	// case 1. ������ �����忡�� ��������� �Լ� ȣ������ ��
	// case 2. Read�� Write�� �ƹ��� ����ϰ� ���� ������

	// case 1 
	const uint32 CurLockThreadID = (m_LockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (CurLockThreadID == TLS_ThreadID)
	{
		// ���� ������ ID�� ���� ������� �������� ���
		// ���° �������� m_WriteCnt�� ����
		m_WriteCnt++;
		return;
	}

	// case 2. 5000���� ������ ���鼭 ��ٸ����� SpinLock���� ����
	// �ʹ� �ð� �����ɸ��� Crash���� ����
	const int64 beginTic = ::GetTickCount64();
	const uint32 desired = ((TLS_ThreadID << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 SpinCnt = 0; SpinCnt < MAX_SPIN_COUNT; SpinCnt++)
		{
			uint32 expected = EMPTY_FLAG;
			if (m_LockFlag.compare_exchange_strong(expected, desired) == true)
			{
				// m_LockFlag�� expected�� ���ٸ� m_LockFlag�� desired�� �ٲ��ֱ�
				// ��, ����־��� ������ ���� ������ ���̵�� Flag �����ذ�
				m_WriteCnt++;
				return;
			}
		}

		const int64 EndTic = ::GetTickCount64();
		if (EndTic - beginTic >= ACQUIRE_TIMEOUT_TICK)
			CRASH("Too Much Lock Wait");

		// �ð��ƿ��� �ƴϰ� �ʹ� ���� ���Ƽ� ������ �������� ��� ������°�
		this_thread::yield(); 
	}
}

void RW_SpinLock::WriteUnLock()
{
	// ReadLock �� Ǯ�� ������ WriteUnLock �Ұ���. ����ó��
	if ((m_LockFlag.load() & READ_COUNT_MASK) != 0)
		CRASH("WriteUnLock ERROR");

	const int32 LockCnt = --m_WriteCnt;
	if (LockCnt == 0)
		m_LockFlag.store(EMPTY_FLAG);
	
}

void RW_SpinLock::ReadLock()
{
	// ������ �����尡 ���� ������ ������ ����
	const uint32 CurLockThreadID = (m_LockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (CurLockThreadID == TLS_ThreadID)
	{
		// ���� ������ ID�� ���� ������� �������� ���
		// ���° �������� m_WriteCnt�� ����
		m_LockFlag.fetch_add(1); // W->R->W�� �Ұ���
		return;
	}

	// �ƹ��� �������� ���� �� �����ؼ� ���� ī��Ʈ �ø���
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

		// �ð��ƿ��� �ƴϰ� �ʹ� ���� ���Ƽ� ������ �������� ��� ������°�
		this_thread::yield();
	}
}

void RW_SpinLock::ReadUnLock()
{
	// fetch �ø���� ����� ������ return���ش�.
	// ��, ���� ���̶� READ_COUNT_MASK ��Ʈ����ŷ �ؼ� 0�̶�°�? �̹� 0�̶�°�
	if((m_LockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("ReadUnLock ERROR");

}
