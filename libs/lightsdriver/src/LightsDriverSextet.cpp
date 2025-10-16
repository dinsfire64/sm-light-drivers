#include "LightsDriverSextet.h"

#include "LightsDriver.h"

#include <hidapi.h>
#include <iostream>

LightsDriverSextet::LightsDriverSextet()
{
    memset(outputBuffer, FILLER_BYTE, sizeof(outputBuffer));
};

LightsDriverSextet::~LightsDriverSextet()
{
    Disconnect();
};

bool LightsDriverSextet::Connect()
{
    // open device
    device = CreateFileA(
        SEXTET_PORT,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    // check device
    if (device == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // get comm state
    DCB dcb;
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(device, &dcb))
    {
        return false;
    }

    // set comm state
    dcb.BaudRate = 115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    if (!SetCommState(device, &dcb))
    {
        return false;
    }

    // success
    is_connected = true;
    return true;
}

void LightsDriverSextet::Disconnect()
{
    if (is_connected)
    {
        CloseHandle(device);
    }

    is_connected = false;
}

void LightsDriverSextet::Set(const LightsState *ls)
{
    memset(outputBuffer, FILLER_BYTE, sizeof(outputBuffer));

    /*
     * Refer to this document for more info
     * https://github.com/stepmania/stepmania/blob/master/src/arch/Lights/LightsDriver_SextetStream.md#bit-meanings
     */

    outputBuffer[0] |= (uint8_t)((ls->bass << 5) |
                                 (ls->bass << 4) |
                                 (ls->marquee_lr_right << 3) |
                                 (ls->marquee_lr_left << 2) |
                                 (ls->marquee_up_right << 1) |
                                 (ls->marquee_up_left << 0));

    outputBuffer[1] |= (uint8_t)((ls->p1_menu << 5) |
                                 (ls->p1_menu << 4));

    outputBuffer[3] |= (uint8_t)((ls->p1_down << 3) |
                                 (ls->p1_up << 2) |
                                 (ls->p1_right << 1) |
                                 (ls->p1_left << 0));

    outputBuffer[7] |= (uint8_t)((ls->p2_menu << 5) |
                                 (ls->p2_menu << 4));

    outputBuffer[9] |= (uint8_t)((ls->p2_down << 3) |
                                 (ls->p2_up << 2) |
                                 (ls->p2_right << 1) |
                                 (ls->p2_left << 0));

    // terminate with LF
    outputBuffer[13] = SEXTET_END_BYTE;

    if (memcmp(outputBuffer, prevOutputBuffer, sizeof(outputBuffer)) != 0)
    {
        // write data to device
        DWORD bytes_written;
        WriteFile(
            device,
            outputBuffer,
            FULL_SEXTET_COUNT,
            &bytes_written,
            NULL);

        memcpy(prevOutputBuffer, outputBuffer, sizeof(prevOutputBuffer));
    }
}
