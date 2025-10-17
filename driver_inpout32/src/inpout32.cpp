#define WIN32_LEAN_AND_MEAN

#include <LightsManager.h>

#include "inpout32.h"

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

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

void SetCabLights(char Data)
{
  light_state.marquee_up_left = IS_BIT_SET(Data, 0);
  light_state.marquee_up_right = IS_BIT_SET(Data, 1);
  light_state.marquee_lr_left = IS_BIT_SET(Data, 2);
  light_state.marquee_lr_right = IS_BIT_SET(Data, 3);
  light_state.p1_menu = IS_BIT_SET(Data, 4);
  light_state.p2_menu = IS_BIT_SET(Data, 5);
  light_state.bass = IS_BIT_SET(Data, 6);
}

void SetPadLights(char Data)
{
  light_state.p1_left = IS_BIT_SET(Data, 0);
  light_state.p1_right = IS_BIT_SET(Data, 1);
  light_state.p1_up = IS_BIT_SET(Data, 2);
  light_state.p1_down = IS_BIT_SET(Data, 3);

  light_state.p2_left = IS_BIT_SET(Data, 4);
  light_state.p2_right = IS_BIT_SET(Data, 5);
  light_state.p2_up = IS_BIT_SET(Data, 6);
  light_state.p2_down = IS_BIT_SET(Data, 7);
}

short IsDriverInstalled()
{
  return 1;
}

void Out32(short Port, short Data)
{
  if (!lm.IsConnected())
  {
    lm.Initialize();
  }

  switch (Port)
  {
  case SHRT_MAX:
  case PORT_LPT1:
    SetCabLights(Data);
    break;
  case PORT_LPT2:
    SetPadLights(Data);

    // Write only after everything is set.
    lm.SetAll(&light_state);
    break;
  default:
    break;
  }
}

int PortIn(short Port)
{
  return 1;
}

int Inp32(short Port)
{
  return 1;
}