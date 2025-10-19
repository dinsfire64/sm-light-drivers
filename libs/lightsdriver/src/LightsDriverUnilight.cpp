#include "LightsDriverUnilight.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverUnilight::LightsDriverUnilight()
{
    hidHelper = new HIDHelper(BD_UNILIGHT_VID, BD_UNILIGHT_PID, BD_UNILIGHT_INTERFACE);
};

LightsDriverUnilight::~LightsDriverUnilight() {
};

bool LightsDriverUnilight::Connect()
{
    if (hidHelper->IsOpen())
    {
        // already connected
        return true;
    }

    return hidHelper->Open();
}

void LightsDriverUnilight::Disconnect()
{
    hidHelper->Close();
}

void LightsDriverUnilight::pushCabLights()
{
    uint8_t cab_lights_state[3];

    cab_lights_state[0] = 0x00;
    cab_lights_state[1] = 0x01;
    cab_lights_state[2] = cabLights;

    hidHelper->Write(cab_lights_state, sizeof(cab_lights_state));
}

void LightsDriverUnilight::pushPadLights()
{
    uint8_t pad_lights_state[3];

    pad_lights_state[0] = 0x00;
    pad_lights_state[1] = 0x02;
    pad_lights_state[2] = padLights;

    hidHelper->Write(pad_lights_state, sizeof(pad_lights_state));
}

void LightsDriverUnilight::Set(const LightsState *ls)
{
    if (!hidHelper->IsOpen())
    {
        // attempt to open if not called already.
        Connect();
    }

    cabLights = 0;

    cabLights |= ls->marquee_up_left ? BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_LEFT : 0x00;
    cabLights |= ls->marquee_up_right ? BD_UNILIGHT_LIGHT_CAB_MARQUEE_UP_RIGHT : 0x00;
    cabLights |= ls->marquee_lr_left ? BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_LEFT : 0x00;
    cabLights |= ls->marquee_lr_right ? BD_UNILIGHT_LIGHT_CAB_MARQUEE_LR_RIGHT : 0x00;

    cabLights |= ls->p1_menu ? BD_UNILIGHT_LIGHT_CAB_BUTTONS_P1 : 0x00;
    cabLights |= ls->p2_menu ? BD_UNILIGHT_LIGHT_CAB_BUTTONS_P2 : 0x00;

    cabLights |= ls->bass ? BD_UNILIGHT_LIGHT_CAB_BASS : 0x00;

    if (cabLights != prevCabLights)
    {
        pushCabLights();
        prevCabLights = cabLights;
    }

    padLights = 0;

    padLights |= ls->p1_left ? BD_UNILIGHT_LIGHT_PAD_P1_LEFT : 0x00;
    padLights |= ls->p1_right ? BD_UNILIGHT_LIGHT_PAD_P1_RIGHT : 0x00;
    padLights |= ls->p1_up ? BD_UNILIGHT_LIGHT_PAD_P1_UP : 0x00;
    padLights |= ls->p1_down ? BD_UNILIGHT_LIGHT_PAD_P1_DOWN : 0x00;

    padLights |= ls->p2_left ? BD_UNILIGHT_LIGHT_PAD_P2_LEFT : 0x00;
    padLights |= ls->p2_right ? BD_UNILIGHT_LIGHT_PAD_P2_RIGHT : 0x00;
    padLights |= ls->p2_up ? BD_UNILIGHT_LIGHT_PAD_P2_UP : 0x00;
    padLights |= ls->p2_down ? BD_UNILIGHT_LIGHT_PAD_P2_DOWN : 0x00;

    if (padLights != prevPadLights)
    {
        pushPadLights();
        prevPadLights = padLights;
    }
}
