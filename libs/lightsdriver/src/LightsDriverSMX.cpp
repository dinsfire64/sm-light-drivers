#include "LightsDriverSMX.h"

#include "LightsDriver.h"

#include <hidapi.h>
#include <iostream>

smx_pad_light_map smx_light_map[] = {

    // P1 R, L, D, U
    {P1_MIDDLE_RGT, 0x00, 0x00, 0xFF, false, true},
    {P1_MIDDLE_LFT, 0x00, 0x00, 0xFF, false, true},
    {P1_LOWER_MID, 0xFF, 0x00, 0x00, false, true},
    {P1_UPPER_MID, 0xFF, 0x00, 0x00, false, true},

    // P2 R, L, D, U
    {P2_MIDDLE_RGT, 0x00, 0x00, 0xFF, false, true},
    {P2_MIDDLE_LFT, 0x00, 0x00, 0xFF, false, true},
    {P2_LOWER_MID, 0xFF, 0x00, 0x00, false, true},
    {P2_UPPER_MID, 0xFF, 0x00, 0x00, false, true},

    // Red/Blue marquee lights
    // UL, LL, UR, LR
    {P1_UPPER_LFT, 0xFF, 0x00, 0x00, true, false},
    {P1_LOWER_LFT, 0x00, 0x00, 0xFF, true, false},
    {P1_UPPER_RGT, 0xFF, 0x00, 0x00, true, false},
    {P1_LOWER_RGT, 0x00, 0x00, 0xFF, true, false},

    // P1, P2 button
    {P1_MIDDLE_MID, 0x00, 0xFF, 0x00, false, false},
    {P2_MIDDLE_MID, 0x00, 0xFF, 0x00, false, false},

};

smx_pad_light_map bass_overlay = {P1_UPPER_LFT, 0xff, 0xff, 0xff, false, false};

LightsDriverSMX::LightsDriverSMX() {
  smx_i = SMX_Interface();
  smx_i.SetOverlayColor(bass_overlay);
  smx_i.SetFadeOn(IniHelper::SMX.FadeOnStep);
  smx_i.SetFadeOff(IniHelper::SMX.FadeOffStep);
};

LightsDriverSMX::~LightsDriverSMX() {

};

bool LightsDriverSMX::Connect() {
  if (smx_i.attemptDllLoad()) {
    LoadColorMap();

    smx_i.StartThread();

    return true;
  }

  return false;
}

void LightsDriverSMX::LoadColorMap() {
  // P1 R, L, D, U
  smx_light_map[0].r = IniHelper::SMX.P1_LeftRight.r;
  smx_light_map[0].g = IniHelper::SMX.P1_LeftRight.g;
  smx_light_map[0].b = IniHelper::SMX.P1_LeftRight.b;

  smx_light_map[1].r = IniHelper::SMX.P1_LeftRight.r;
  smx_light_map[1].g = IniHelper::SMX.P1_LeftRight.g;
  smx_light_map[1].b = IniHelper::SMX.P1_LeftRight.b;

  smx_light_map[2].r = IniHelper::SMX.P1_UpDown.r;
  smx_light_map[2].g = IniHelper::SMX.P1_UpDown.g;
  smx_light_map[2].b = IniHelper::SMX.P1_UpDown.b;

  smx_light_map[3].r = IniHelper::SMX.P1_UpDown.r;
  smx_light_map[3].g = IniHelper::SMX.P1_UpDown.g;
  smx_light_map[3].b = IniHelper::SMX.P1_UpDown.b;

  // P2 R, L, D, U
  smx_light_map[4].r = IniHelper::SMX.P2_LeftRight.r;
  smx_light_map[4].g = IniHelper::SMX.P2_LeftRight.g;
  smx_light_map[4].b = IniHelper::SMX.P2_LeftRight.b;

  smx_light_map[5].r = IniHelper::SMX.P2_LeftRight.r;
  smx_light_map[5].g = IniHelper::SMX.P2_LeftRight.g;
  smx_light_map[5].b = IniHelper::SMX.P2_LeftRight.b;

  smx_light_map[6].r = IniHelper::SMX.P2_UpDown.r;
  smx_light_map[6].g = IniHelper::SMX.P2_UpDown.g;
  smx_light_map[6].b = IniHelper::SMX.P2_UpDown.b;

  smx_light_map[7].r = IniHelper::SMX.P2_UpDown.r;
  smx_light_map[7].g = IniHelper::SMX.P2_UpDown.g;
  smx_light_map[7].b = IniHelper::SMX.P2_UpDown.b;

  // Marquee UL, LL, UR, LR
  smx_light_map[8].r = IniHelper::SMX.MarUL.r;
  smx_light_map[8].g = IniHelper::SMX.MarUL.g;
  smx_light_map[8].b = IniHelper::SMX.MarUL.b;

  smx_light_map[9].r = IniHelper::SMX.MarLL.r;
  smx_light_map[9].g = IniHelper::SMX.MarLL.g;
  smx_light_map[9].b = IniHelper::SMX.MarLL.b;

  smx_light_map[10].r = IniHelper::SMX.MarUR.r;
  smx_light_map[10].g = IniHelper::SMX.MarUR.g;
  smx_light_map[10].b = IniHelper::SMX.MarUR.b;

  smx_light_map[11].r = IniHelper::SMX.MarLR.r;
  smx_light_map[11].g = IniHelper::SMX.MarLR.g;
  smx_light_map[11].b = IniHelper::SMX.MarLR.b;

  // P1, P2 button
  smx_light_map[12].r = IniHelper::SMX.P1_Start.r;
  smx_light_map[12].g = IniHelper::SMX.P1_Start.g;
  smx_light_map[12].b = IniHelper::SMX.P1_Start.b;

  smx_light_map[13].r = IniHelper::SMX.P2_Start.r;
  smx_light_map[13].g = IniHelper::SMX.P2_Start.g;
  smx_light_map[13].b = IniHelper::SMX.P2_Start.b;

  // TODO: fix bass mixing logic.
  // bass
  // bass_overlay.r = IniHelper::SMX.Bass.r;
  // bass_overlay.g = IniHelper::SMX.Bass.r;
  // bass_overlay.b = IniHelper::SMX.Bass.r;
  // smx_i.SetOverlayColor(bass_overlay);
}

void LightsDriverSMX::Disconnect() {}

void LightsDriverSMX::Set(const LightsState *ls) {
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
