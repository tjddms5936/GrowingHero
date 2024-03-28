#pragma once

#define WIN32_LEAN_AND_MEAN           

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerLib.lib")
#else
#pragma comment(lib, "Release\\ServerLib.lib")
#endif

 #include "CorePch.h"