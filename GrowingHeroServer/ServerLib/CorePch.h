#pragma once

// pch파일 자체는 서버 코어 내부에서만 활용하는 파일이라서 
// 클라나 게임서버에서 참조하길 원하는 애들을 모아놓는 용도

#include "Types.h"
#include "CoreTLS.h"
#include "CoreGlobalVariable.h"
#include "CoreMacro.h"

#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#include "Windows.h"
#include "RW_SpinLock.h"