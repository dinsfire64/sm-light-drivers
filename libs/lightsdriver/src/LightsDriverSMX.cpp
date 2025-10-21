#include "LightsDriverSMX.h"

#include "LightsDriver.h"

#include <hidapi.h>
#include <iostream>

smx_pad_light_map smx_light_map[] = {

    // P1 R, L, D, U
    {5, 0x00, 0x00, 0xFF, false, false},
    {3, 0x00, 0x00, 0xFF, false, false},
    {7, 0xFF, 0x00, 0x00, false, false},
    {1, 0xFF, 0x00, 0x00, false, false},

    // P2 R, L, D, U
    {14, 0x00, 0x00, 0xFF, false, false},
    {12, 0x00, 0x00, 0xFF, false, false},
    {16, 0xFF, 0x00, 0x00, false, false},
    {10, 0xFF, 0x00, 0x00, false, false},

    // Red/Blue marquee lights
    // UL, LL, UR, LR
    {0, 0xFF, 0x00, 0x00, true, true},
    {6, 0x00, 0x00, 0xFF, true, true},
    {2, 0xFF, 0x00, 0x00, true, true},
    {8, 0x00, 0x00, 0xFF, true, true},

    // P1, P2 button
    {4, 0x00, 0xFF, 0x00, false, true},
    {13, 0x00, 0xFF, 0x00, false, true},

    // Bass is handled in process_buffer
};

LightsDriverSMX::LightsDriverSMX()
{
    smx_i = SMX_Interface();
    smx_i.SetOverlayColor({0, 0xff, 0xff, 0xff, false});
};

LightsDriverSMX::~LightsDriverSMX() {

};

bool LightsDriverSMX::Connect()
{
    if (smx_i.attemptDllLoad())
    {
        smx_i.StartThread();

        return true;
    }

    return false;
}

void LightsDriverSMX::Disconnect()
{
}

void LightsDriverSMX::Set(const LightsState *ls)
{
    smx_i.SetOverlayState(ls->bass);

    smx_i.Update_Light(smx_light_map[0], ls->p1_right);
    smx_i.Update_Light(smx_light_map[1], ls->p1_left);
    smx_i.Update_Light(smx_light_map[2], ls->p1_down);
    smx_i.Update_Light(smx_light_map[3], ls->p1_up);

    smx_i.Update_Light(smx_light_map[4], ls->p2_right);
    smx_i.Update_Light(smx_light_map[5], ls->p2_left);
    smx_i.Update_Light(smx_light_map[6], ls->p2_down);
    smx_i.Update_Light(smx_light_map[7], ls->p2_up);

    smx_i.Update_Light(smx_light_map[8], ls->marquee_up_left);
    smx_i.Update_Light(smx_light_map[9], ls->marquee_lr_left);
    smx_i.Update_Light(smx_light_map[10], ls->marquee_up_right);
    smx_i.Update_Light(smx_light_map[11], ls->marquee_lr_right);

    smx_i.Update_Light(smx_light_map[12], ls->p1_menu);
    smx_i.Update_Light(smx_light_map[13], ls->p2_menu);
}
