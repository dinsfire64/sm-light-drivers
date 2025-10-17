#pragma once

#include <windows.h>

#define EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

    BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

    EXPORT short WINAPI IsDriverInstalled();
    EXPORT void WINAPI PortOut(short Port, short Data);

#ifdef __cplusplus
}
#endif
