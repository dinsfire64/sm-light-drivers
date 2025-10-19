#pragma once

#include "LightsDriver.h"
#include "LightsManager.h"
#include "HIDHelper.h"

#include <cstdint>
#include <string.h>

#define BD_UNILIGHT_VID 0x04BD
#define BD_UNILIGHT_PID 0x00BD
#define BD_UNILIGHT_INTERFACE -1

#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT 0x01
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT 0x02
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT 0x04
#define BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT 0x08
#define BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1 0x10
#define BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2 0x20
#define BD_UNILIGHT_LIGHT_CAB_BASS 0x40
#define BD_UNILIGHT_LIGHT_CAB_OPTIONAL 0x80

#define BD_UNILIGHT_LIGHT_PAD_P1_LEFT 0x01
#define BD_UNILIGHT_LIGHT_PAD_P1_RIGHT 0x02
#define BD_UNILIGHT_LIGHT_PAD_P1_UP 0x04
#define BD_UNILIGHT_LIGHT_PAD_P1_DOWN 0x08
#define BD_UNILIGHT_LIGHT_PAD_P2_LEFT 0x10
#define BD_UNILIGHT_LIGHT_PAD_P2_RIGHT 0x20
#define BD_UNILIGHT_LIGHT_PAD_P2_UP 0x40
#define BD_UNILIGHT_LIGHT_PAD_P2_DOWN 0x80

class LightsDriverUnilight : public LightsDriver
{
public:
    LightsDriverUnilight();
    ~LightsDriverUnilight() override;

    bool Connect() override;

    void Set(const LightsState *ls) override;

    void Disconnect() override;

private:
    uint8_t cabLights;
    uint8_t prevCabLights;

    uint8_t padLights;
    uint8_t prevPadLights;

    HIDHelper *hidHelper;

    void pushCabLights();
    void pushPadLights();
};