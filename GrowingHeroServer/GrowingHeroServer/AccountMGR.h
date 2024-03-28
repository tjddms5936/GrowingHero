#pragma once
#include <mutex>

class Account
{


};

class AccountMGR
{
public:
	static AccountMGR* Instance()
	{
		static AccountMGR instance;
		return &instance;
	}

	Account* GetAccount(int32 id)
	{
		lock_guard<std::mutex> lg(_mutex);
		// ¹º°¡¸¦ °®°í¿È
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;
};

