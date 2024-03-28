#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

#include "UserMGR.h"
#include "AccountMGR.h"

void f1()
{
	for (int32 i = 0; i < 1; i++)
	{
		UserMGR::instance()->ProcessSave();
	}
}

void f2()
{
	for (int32 i = 0; i < 1; i++)
	{
		AccountMGR::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(f1);
	std::thread t2(f2);



	t1.join();
	t2.join();

	cout << "jobs done " << "\n";
}
