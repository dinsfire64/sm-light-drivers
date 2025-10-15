#define WIN32_LEAN_AND_MEAN

#include <LightsManager.h>

#include "parallel_lights_io.h"

#define PORT_LPT1 0x378
#define PORT_LPT2 0x278
#define PORT_LPT3 0x3BC

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

LightsManager lm;
LightsState light_state;

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason)
  {
  case DLL_PROCESS_ATTACH:
    if (!lm.IsConnected())
    {
      lm.Initialize();
    }
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
  light_state.p1_up = IS_BIT_SET(Data, 0);
  light_state.p1_down = IS_BIT_SET(Data, 1);
  light_state.p1_left = IS_BIT_SET(Data, 2);
  light_state.p1_right = IS_BIT_SET(Data, 3);
}

void SetPad2Lights(char Data)
{
  light_state.p2_up = IS_BIT_SET(Data, 0);
  light_state.p2_down = IS_BIT_SET(Data, 1);
  light_state.p2_left = IS_BIT_SET(Data, 2);
  light_state.p2_right = IS_BIT_SET(Data, 3);
}

short int WINAPI IsDriverInstalled()
{
  return 1;
}

void WINAPI PortOut(short int port, char data)
{
  switch (port)
  {
  case SHRT_MAX:
  case PORT_LPT1:
    SetCabLights(data);
    break;
  case PORT_LPT2:
    SetPad1Lights(data);
    break;
  case PORT_LPT3:
    SetPad2Lights(data);

    // Write only after everything is set.
    lm.SetAll(&light_state);
    break;
  default:
    break;
  }
}

// --- STUB IMPLEMENTATIONS FOR OTHER EXPORTED FUNCTIONS (with ) ---

char WINAPI PortIn(short int port)
{

  return '\0';
}

void WINAPI ClrPortBit(short int Port, int Bit)
{
}

short int WINAPI GetPortBit(short int Port, int Bit)
{

  return 0;
}

void WINAPI LeftPortShift(short int Port, int ShiftAmount)
{
}

void WINAPI NotPortBit(short int Port, int Bit)
{
}

unsigned long WINAPI PortDWordIn(short int Port)
{

  return 0;
}

void WINAPI PortDWordOut(short int Port, unsigned long Data)
{
}

short int WINAPI PortWordIn(short int Port)
{

  return 0;
}

void WINAPI PortWordOut(short int Port, short int Data)
{
}

void WINAPI ReleasePort(short int Port)
{
}

void WINAPI RightPortShift(short int Port, int ShiftAmount)
{
}

void WINAPI SetPortBit(short int Port, int Bit)
{
}