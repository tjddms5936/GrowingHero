#pragma once

// 표준에서도 threadID 얻어 올 수 있지만, 그거는 중구난방 숫자라서 1,2,3 ...처럼 관리하기 위해
extern thread_local uint32 TLS_ThreadID;
