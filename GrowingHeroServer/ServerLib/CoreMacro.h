#pragma once

// ���������� crash�� ���� ���� ��Ȳ���� �۵��ϰԲ� �Ѵ�.
// ���� ������. �����Ϸ����� crash�� nullptr�� �ƴ϶�� ������ �ϰ�
// crash�� �� �־��ָ� ���α׷��� ���� �Ǿ��ִ�.
// ��, �������� crash�� ������� �� ����Ѵ�.
#define  CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

// ���� true���� �ƴ����� üũ�� ������ ���Ǻη� crash ������� �ǵ�
#define ASSERT_CRASH(expr)				\
{										\
	if (!(expr))						\
	{									\
		CRASH("ASSERT_CRASH");			\
		__analysis_assume(expr);		\
	}									\
}

// Custom Lock���� �� ���ϰ� ���� ���� �۾�
// ##�Ű����� �ϸ� ��������. 
#define USE_MANY_LOCKS(count)	RW_SpinLock Lock[count];
#define USE_LOCK				USE_MANY_LOCKS[1];
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(Lock[idx]);
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(Lock[idx]);
#define WRITED_LOCK				WRITE_LOCK_IDX(0);