#pragma once

#include <thread>
#include <functional> // �ݹ��Լ����� ����� std�� ����ϱ� ����

// ǥ�� Thread���� �����ϴ� �Ŵ���
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();


	// �ݹ� �Լ��� �޾��ܰ� ���ÿ� �����带 �����ϴ� �κ�
	// functional �̿��ϸ� ����, �Լ������� �� �°� �Լ� ���� ���� �� �ִ� �����־ ���
	//						intput output
	void Launch(std::function<void(void)> callbackFunc);

	// Thread���� join�ϸ� ���������� ��ٷ��ִ� �װ�
	void Join();

	// �Ŵ����� ���� ������ Thread���� ���� ������ܰ� ���ÿ� 
	// ���ϴ� TLS ������ �ʱ�ȭ �ϴ� �κе��� ����� �ְ� �ͱ� ������
	// TLS�� �ʱ�ȭ �ϰ� ������ �κ� �ʿ�
	static void InitTLS();
	static void DestroyTLS();


private:
	Mutex m_lock;
	vector<thread> m_threads;
};

