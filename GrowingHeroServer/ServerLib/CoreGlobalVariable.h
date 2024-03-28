#pragma once

// 전역으로 사용할 매니저류의 클래스를 넣어서 관리

extern class ThreadManager* GThreadMGR;

class CoreGlobalVariable
{
public:
	CoreGlobalVariable();
	~CoreGlobalVariable();
	CoreGlobalVariable(const CoreGlobalVariable&) = delete;
	CoreGlobalVariable& operator =(const CoreGlobalVariable&) = delete;


};