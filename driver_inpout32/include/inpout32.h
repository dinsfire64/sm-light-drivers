#pragma once

#include <windows.h>

#define EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

    BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

    EXPORT int WINAPI Inp32(short Port);
    EXPORT void WINAPI Out32(short Port, short Data);

    EXPORT short WINAPI IsDriverInstalled();

#ifdef __cplusplus
}
#endif
