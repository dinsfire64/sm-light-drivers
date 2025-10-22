#pragma once

#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

#include <windows.h>

class LightsDriverSextet : public LightsDriver
{
public:
    LightsDriverSextet();
    ~LightsDriverSextet() override;

    bool Connect() override;
    void Set(const LightsState *ls) override;
    void Disconnect() override;

private:
    // number of bytes to contain the full sextet pack and a trailing LF
    static const size_t FULL_SEXTET_COUNT = 14;
    static const uint8_t FILLER_BYTE = 0xC0;

    static const uint8_t SEXTET_END_BYTE = '\n';

    uint8_t outputBuffer[FULL_SEXTET_COUNT];
    uint8_t prevOutputBuffer[FULL_SEXTET_COUNT];

    HANDLE deviceHardware = nullptr;
    bool connectedHardware = false;

    HANDLE devicePipe = nullptr;
    bool connectedPipe = false;

    bool ConnectHardware();
    bool ConnectPipe();

    bool PushTo(HANDLE device);
};