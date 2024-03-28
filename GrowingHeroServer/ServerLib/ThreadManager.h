#pragma once

#include <thread>
#include <functional> // 콜백함수에서 사용할 std를 사용하기 위함

// 표준 Thread들을 관리하는 매니저
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();


	// 콜백 함수를 받아줌과 동시에 쓰레드를 실행하는 부분
	// functional 이용하면 람다, 함수포인터 등 온갖 함수 형태 받을 수 있는 장점있어서 사용
	//						intput output
	void Launch(std::function<void(void)> callbackFunc);

	// Thread에서 join하면 끝날때까지 기다려주는 그거
	void Join();

	// 매니저를 만든 이유는 Thread들을 새로 만들어줌과 동시에 
	// 원하는 TLS 영역을 초기화 하는 부분들을 만들어 주고 싶기 때문에
	// TLS를 초기화 하고 날리는 부분 필요
	static void InitTLS();
	static void DestroyTLS();


private:
	Mutex m_lock;
	vector<thread> m_threads;
};

