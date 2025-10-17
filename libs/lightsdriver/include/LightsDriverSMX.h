#pragma once

#include "LightsDriver.h"
#include "LightsManager.h"
#include "SMX_Interface.h"

#include <cstdint>
#include <string.h>

#include <windows.h>

class LightsDriverSMX : public LightsDriver
{
public:
    LightsDriverSMX();
    ~LightsDriverSMX() override;

    bool Connect() override;

    void Set(const LightsState *ls) override;

    void Disconnect() override;

private:
    SMX_Interface smx_i;
};