#pragma once

// �������� ����� �Ŵ������� Ŭ������ �־ ����

extern class ThreadManager* GThreadMGR;

class CoreGlobalVariable
{
public:
	CoreGlobalVariable();
	~CoreGlobalVariable();
	CoreGlobalVariable(const CoreGlobalVariable&) = delete;
	CoreGlobalVariable& operator =(const CoreGlobalVariable&) = delete;


};