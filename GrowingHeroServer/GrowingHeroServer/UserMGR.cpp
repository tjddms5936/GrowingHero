#include "pch.h"
#include "UserMGR.h"
#include "AccountMGR.h"

void UserMGR::ProcessSave()
{
	std::lock_guard<std::mutex> lg(_mutex);

	Account* account = AccountMGR::Instance()->GetAccount(10);
}
