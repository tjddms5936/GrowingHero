#include "pch.h"
#include "AccountMGR.h"
#include "UserMGR.h"

void AccountMGR::ProcessLogin()
{
	std::lock_guard<std::mutex> lg(_mutex);

	User* user = UserMGR::instance()->GetUser(100);
}
