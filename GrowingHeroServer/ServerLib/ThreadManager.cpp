#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobalVariable.h"

ThreadManager::ThreadManager()
{
	// Main Thread도 TLS 사용하긴 할거니까 Main Thread에서도 TLS 초기화
	InitTLS();

}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callbackFunc)
{
	// 동시 다발적으로 여러명이 혹시 실행할수 있으니까 
	LockGuard guard(m_lock);

	m_threads.push_back(std::thread([=]()
		{	
			InitTLS(); // thread가 만들어지면 InitTLS 해주고
			callbackFunc(); // callback 함수 실행해주고
			DestroyTLS(); // TLS 정리
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
	// TLS_ThreadID 1씩 증가시키면서 관리하기 위함
	static Atomic<int32> SThreadID = 1;
	TLS_ThreadID = SThreadID.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}
