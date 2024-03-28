#include "pch.h"
#include "CoreGlobalVariable.h"
#include "ThreadManager.h"

ThreadManager* GThreadMGR = nullptr;

CoreGlobalVariable::CoreGlobalVariable()
{
	GThreadMGR = new ThreadManager();
}

CoreGlobalVariable::~CoreGlobalVariable()
{
	delete GThreadMGR;
}
