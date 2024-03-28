#pragma once
#include <mutex>

class User
{

};


class UserMGR
{
public:
	static UserMGR* instance()
	{
		static UserMGR instance;
		return &instance;
	}

	User* GetUser(int32 id)
	{
		std:lock_guard<std::mutex> lg(_mutex);
		//���� �����
		return nullptr;
	}

	void ProcessSave();

private:
	mutex _mutex;
};

