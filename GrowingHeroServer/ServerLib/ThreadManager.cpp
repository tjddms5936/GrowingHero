#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobalVariable.h"

ThreadManager::ThreadManager()
{
	// Main Thread�� TLS ����ϱ� �ҰŴϱ� Main Thread������ TLS �ʱ�ȭ
	InitTLS();

}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callbackFunc)
{
	// ���� �ٹ������� �������� Ȥ�� �����Ҽ� �����ϱ� 
	LockGuard guard(m_lock);

	m_threads.push_back(std::thread([=]()
		{	
			InitTLS(); // thread�� ��������� InitTLS ���ְ�
			callbackFunc(); // callback �Լ� �������ְ�
			DestroyTLS(); // TLS ����
		}));
}

void ThreadManager::Join()
{
	for (thread& t : m_threads)
	{
		if (t.joinable())
			t.join();
	}

	m_threads.clear();
}

void ThreadManager::InitTLS()
{
	// TLS_ThreadID 1�� ������Ű�鼭 �����ϱ� ����
	static Atomic<int32> SThreadID = 1;
	TLS_ThreadID = SThreadID.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}
