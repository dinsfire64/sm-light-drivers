#include <windows.h>
#include <iostream>
#include <thread>

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278

typedef void(WINAPI *Out32Func)(short int, char);
typedef short int(WINAPI *IsDriverInstalledFunc)();

int main()
{
    const HMODULE hModule = LoadLibrary("inpout32.dll");
    if (!hModule)
    {
        const DWORD error = GetLastError(); // Get the last error code
        std::cerr << "Failed to load DLL. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        return 1;
    }

    const auto Out32 = reinterpret_cast<Out32Func>(GetProcAddress(hModule, "Out32"));
    const auto IsDriverInstalled = reinterpret_cast<IsDriverInstalledFunc>(GetProcAddress(hModule, "IsDriverInstalled"));

    if (!Out32 || !IsDriverInstalled)
    {
        const DWORD error = GetLastError(); // Get the last error code for GetProcAddress failure
        std::cerr << "Failed to get function addresses. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    const short int initResult = IsDriverInstalled();
    std::cout << "IsDriverInstalled result: " << initResult << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        Out32(PORT_LPT1, static_cast<char>(0xFF));
        Out32(PORT_LPT2, static_cast<char>(0xFF));
        std::this_thread::sleep_for(std::chrono::seconds(3));
        Out32(PORT_LPT1, 0);
        Out32(PORT_LPT2, 0);
    }

    FreeLibrary(hModule);
    return 0;
}