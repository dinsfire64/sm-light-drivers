#include <pacdrive.h>
#include <LightsManager.h>

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

LightsManager lm;

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason)
  {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

int PacInitialize(void)
{
  lm.Initialize();

  return lm.IsConnected() ? 1 : 0;
}

void PacShutdown(void)
{
  // taken care of by LightsMan destructor
}

bool PacSetLEDStates(int deviceId, short int data)
{
  LightsState ls;

  ls.p1_up = IS_BIT_SET(data, 0);
  ls.p1_down = IS_BIT_SET(data, 1);
  ls.p1_left = IS_BIT_SET(data, 2);
  ls.p1_right = IS_BIT_SET(data, 3);
  ls.p2_up = IS_BIT_SET(data, 4);
  ls.p2_down = IS_BIT_SET(data, 5);
  ls.p2_left = IS_BIT_SET(data, 6);
  ls.p2_right = IS_BIT_SET(data, 7);

  ls.marquee_up_left = IS_BIT_SET(data, 8);
  ls.marquee_up_right = IS_BIT_SET(data, 9);
  ls.marquee_lr_left = IS_BIT_SET(data, 10);
  ls.marquee_lr_right = IS_BIT_SET(data, 11);
  ls.p1_menu = IS_BIT_SET(data, 12);
  ls.p2_menu = IS_BIT_SET(data, 13);
  ls.bass = IS_BIT_SET(data, 14);

  lm.SetAll(&ls);

  return true;
}
