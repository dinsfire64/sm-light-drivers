#include <windows.h>
#include <iostream>
#include <thread>

// minimaid prototypes
typedef void (*MM_SETDDRPAD1LIGHT)(int, int);
typedef void (*MM_SETDDRPAD2LIGHT)(int, int);
typedef void (*MM_SETCABINETLIGHT)(int, int);
typedef void (*MM_SETDDRBASSLIGHT)(int, int);
static MM_SETDDRPAD1LIGHT mm_setDDRPad1Light;
static MM_SETDDRPAD2LIGHT mm_setDDRPad2Light;
static MM_SETCABINETLIGHT mm_setDDRCabinetLight;
static MM_SETDDRBASSLIGHT mm_setDDRBassLight;

typedef bool (*MM_CONNECT_MINIMAID)();
typedef bool (*MM_SETKB)(bool val);
static MM_CONNECT_MINIMAID mm_connect_minimaid;
static MM_SETKB mm_setKB;

typedef void (*MM_SETDDRALLON)();
typedef void (*MM_SETDDRALLOFF)();
static MM_SETDDRALLON mm_setDDRAllOn;
static MM_SETDDRALLOFF mm_setDDRAllOff;

typedef void (*MM_SETBLUELED)(unsigned char);
typedef void (*MM_SETMMOUTPUTREPORTS)(unsigned char, unsigned char, unsigned char, unsigned char);
typedef bool (*MM_SENDDDRMINIMAIDUPDATE)();
static MM_SETBLUELED mm_setBlueLED;
static MM_SETMMOUTPUTREPORTS mm_setMMOutputReports;
static MM_SENDDDRMINIMAIDUPDATE mm_sendDDRMiniMaidUpdate;

typedef void (*MM_INIT)();
typedef void (*MM_TURNON)(unsigned char, int);
typedef bool (*MM_TURNOFF)(unsigned char, int);
static MM_INIT mm_init;
static MM_TURNON mm_turnON;
static MM_TURNOFF mm_turnOFF;

int main()
{
    const HMODULE hModule = LoadLibrary("mmmagic.dll");
    if (!hModule)
    {
        const DWORD error = GetLastError(); // Get the last error code
        std::cerr << "Failed to load DLL. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        return 1;
    }

    // Get the function pointers
    mm_connect_minimaid = (MM_CONNECT_MINIMAID)GetProcAddress(hModule, "mm_connect_minimaid");
    mm_setKB = (MM_SETKB)GetProcAddress(hModule, "mm_setKB");
    mm_setDDRPad1Light = (MM_SETDDRPAD1LIGHT)GetProcAddress(hModule, "mm_setDDRPad1Light");
    mm_setDDRPad2Light = (MM_SETDDRPAD2LIGHT)GetProcAddress(hModule, "mm_setDDRPad2Light");
    mm_setDDRCabinetLight = (MM_SETCABINETLIGHT)GetProcAddress(hModule, "mm_setDDRCabinetLight");
    mm_setDDRBassLight = (MM_SETDDRBASSLIGHT)GetProcAddress(hModule, "mm_setDDRBassLight");
    mm_setDDRAllOn = (MM_SETDDRALLON)GetProcAddress(hModule, "mm_setDDRAllOn");
    mm_setDDRAllOff = (MM_SETDDRALLOFF)GetProcAddress(hModule, "mm_setDDRAllOff");
    mm_setBlueLED = (MM_SETBLUELED)GetProcAddress(hModule, "mm_setBlueLED");
    mm_setMMOutputReports = (MM_SETMMOUTPUTREPORTS)GetProcAddress(hModule, "mm_setMMOutputReports");
    mm_sendDDRMiniMaidUpdate = (MM_SENDDDRMINIMAIDUPDATE)GetProcAddress(hModule, "mm_sendDDRMiniMaidUpdate");

    if (!mm_connect_minimaid || !mm_setKB)
    {
        const DWORD error = GetLastError(); // Get the last error code for GetProcAddress failure
        std::cerr << "Failed to get function addresses. Error code: " << error << " (" << std::system_category().message(error) << ")" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    mm_connect_minimaid();
    mm_setKB(true);

    for (int i = 0; i < 10; ++i)
    {
        mm_setDDRAllOn();
        mm_sendDDRMiniMaidUpdate();
        std::this_thread::sleep_for(std::chrono::seconds(3));

        mm_setDDRAllOff();
        mm_sendDDRMiniMaidUpdate();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    FreeLibrary(hModule);
    return 0;
}