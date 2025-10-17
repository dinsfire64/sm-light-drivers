#define WIN32_LEAN_AND_MEAN

#include <LightsManager.h>

#include "parallel_lights_io.h"

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

LightsManager lm;
LightsState light_state;

bool useOITGmapping = false;

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

void SetPad1Lights(char Data)
{
  light_state.p1_left = IS_BIT_SET(Data, 0);
  light_state.p1_right = IS_BIT_SET(Data, 1);
  light_state.p1_up = IS_BIT_SET(Data, 2);
  light_state.p1_down = IS_BIT_SET(Data, 3);

  // sm3.9 puts the p2 lights here.
  if (!useOITGmapping)
  {
    light_state.p2_left = IS_BIT_SET(Data, 4);
    light_state.p2_right = IS_BIT_SET(Data, 5);
    light_state.p2_up = IS_BIT_SET(Data, 6);
    light_state.p2_down = IS_BIT_SET(Data, 7);
  }
}

void SetPad2Lights(char Data)
{
  // if any of these lights are true, use the oITG mapping.
  if ((Data & 0x0F) > 0)
  {
    useOITGmapping = true;
  }

  if (useOITGmapping)
  {
    light_state.p2_left = IS_BIT_SET(Data, 0);
    light_state.p2_right = IS_BIT_SET(Data, 1);
    light_state.p2_up = IS_BIT_SET(Data, 2);
    light_state.p2_down = IS_BIT_SET(Data, 3);
  }
}

short IsDriverInstalled()
{
  return 1;
}

void PortOut(short Port, short Data)
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
    SetPad1Lights(Data);
    break;
  case PORT_LPT3:
    SetPad2Lights(Data);

    // Write only after everything is set.
    lm.SetAll(&light_state);
    break;
  default:
    break;
  }
}