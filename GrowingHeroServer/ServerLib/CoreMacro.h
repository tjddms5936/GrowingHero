#pragma once

// 인위적으로 crash를 내고 싶은 상황에서 작동하게끔 한다.
// 동작 원리는. 컴파일러한테 crash가 nullptr이 아니라고 거짓말 하고
// crash에 값 넣어주면 프로그램이 뻗게 되어있다.
// 즉, 인위적인 crash를 내고싶을 때 사용한다.
#define  CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

// 값이 true인지 아닌지를 체크해 가지고 조건부로 crash 내고싶은 의도
#define ASSERT_CRASH(expr)				\
{										\
	if (!(expr))						\
	{									\
		CRASH("ASSERT_CRASH");			\
		__analysis_assume(expr);		\
	}									\
}

// Custom Lock관련 더 편하게 쓰기 위한 작업
// ##매개변수 하면 맞춰진다. 
#define USE_MANY_LOCKS(count)	RW_SpinLock Lock[count];
#define USE_LOCK				USE_MANY_LOCKS[1];
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(Lock[idx]);
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(Lock[idx]);
#define WRITED_LOCK				WRITE_LOCK_IDX(0);