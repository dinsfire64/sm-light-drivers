#define WIN32_LEAN_AND_MEAN

#include <LightsManager.h>

#include "mmmagic.h"

LightsManager lm;
LightsState light_state;

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason)
  {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  default:
    break;
  }
  return TRUE;
}

bool mm_sendDDRMiniMaidUpdate()
{
  lm.SetAll(&light_state);
  return true;
}

void mm_init()
{
}

void mm_setDDRPad1Light(int a, int b)
{
  if (a == DDR_DOUBLE_PAD_UP)
  {
    light_state.p1_up = b;
  }
  else if (a == DDR_DOUBLE_PAD_DOWN)
  {
    light_state.p1_down = b;
  }
  else if (a == DDR_DOUBLE_PAD_LEFT)
  {
    light_state.p1_left = b;
  }
  else if (a == DDR_DOUBLE_PAD_RIGHT)
  {
    light_state.p1_right = b;
  }
  else if (a == DDR_DOUBLE_PAD_RESET)
  {
    light_state.p1_up = false;
    light_state.p1_down = false;
    light_state.p1_left = false;
    light_state.p1_right = false;
  }
}

void mm_setDDRPad2Light(int a, int b)
{
  if (a == DDR_DOUBLE_PAD_UP)
  {
    light_state.p2_up = b;
  }
  else if (a == DDR_DOUBLE_PAD_DOWN)
  {
    light_state.p2_down = b;
  }
  else if (a == DDR_DOUBLE_PAD_LEFT)
  {
    light_state.p2_left = b;
  }
  else if (a == DDR_DOUBLE_PAD_RIGHT)
  {
    light_state.p2_right = b;
  }
  else if (a == DDR_DOUBLE_PAD_RESET)
  {
    light_state.p2_up = false;
    light_state.p2_down = false;
    light_state.p2_left = false;
    light_state.p2_right = false;
  }
}

void mm_setDDRCabinetLight(int a, int b)
{
  if (a == DDR_DOUBLE_PLAYER1_PANEL)
  {
    light_state.p1_menu = b;
  }
  else if (a == DDR_DOUBLE_PLAYER2_PANEL)
  {
    light_state.p2_menu = b;
  }
  else if (a == DDR_DOUBLE_MARQUEE_LOWER_RIGHT)
  {
    light_state.marquee_lr_right = b;
  }
  else if (a == DDR_DOUBLE_MARQUEE_UPPER_RIGHT)
  {
    light_state.marquee_up_right = b;
  }
  else if (a == DDR_DOUBLE_MARQUEE_LOWER_LEFT)
  {
    light_state.marquee_lr_left = b;
  }
  else if (a == DDR_DOUBLE_MARQUEE_UPPER_LEFT)
  {
    light_state.marquee_up_left = b;
  }
}

void mm_setDDRBassLight(int a, int b)
{
  light_state.bass = b;
}

void setAllLocal(bool val)
{
  light_state.marquee_up_left = val;
  light_state.marquee_up_right = val;
  light_state.marquee_lr_left = val;
  light_state.marquee_lr_right = val;
  light_state.bass = val;

  light_state.p1_menu = val;
  light_state.p2_menu = val;

  light_state.p1_up = val;
  light_state.p1_down = val;
  light_state.p1_left = val;
  light_state.p1_right = val;

  light_state.p2_up = val;
  light_state.p2_down = val;
  light_state.p2_left = val;
  light_state.p2_right = val;
}

void mm_setDDRAllOn()
{
  setAllLocal(true);
}

void mm_setDDRAllOff()
{
  setAllLocal(false);
}

bool mm_connect_minimaid()
{
  lm.Initialize();
  return true;
}

bool mm_setKB(bool val)
{
  return true;
}

void mm_setBlueLED(unsigned char value)
{
}

void mm_setMMOutputReports(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
}

unsigned char mm_turnON(unsigned char set, int bit)
{
  return 0;
}

unsigned char mm_turnOFF(unsigned char set, int bit)
{
  return 0;
}
