#include <LightsManager.h>
#include <pacdrive.h>

#define IS_BIT_SET(data, bit) ((data & (1 << bit)) != 0)

LightsManager *lm = new LightsManager();

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason,
                       IN LPVOID Reserved) {
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    if (lm != nullptr) {
      lm->Shutdown();
    }
    break;
  default:
    break;
  }
  return TRUE;
}

bool PacInitialize(void) {
  if (!lm->IsConnected()) {
    lm->Initialize();
  }

  return lm->IsConnected();
}

bool PacSetLEDStates(int deviceId, short int data) {
  printf("PacSetLEDStates");

  LightsState ls;

  // this is the correct "openITG" order of lights

  ls.p1_left = IS_BIT_SET(data, 0);
  ls.p1_right = IS_BIT_SET(data, 1);
  ls.p1_up = IS_BIT_SET(data, 2);
  ls.p1_down = IS_BIT_SET(data, 3);
  ls.p2_left = IS_BIT_SET(data, 4);
  ls.p2_right = IS_BIT_SET(data, 5);
  ls.p2_up = IS_BIT_SET(data, 6);
  ls.p2_down = IS_BIT_SET(data, 7);

  ls.marquee_up_left = IS_BIT_SET(data, 8);
  ls.marquee_up_right = IS_BIT_SET(data, 9);
  ls.marquee_lr_left = IS_BIT_SET(data, 10);
  ls.marquee_lr_right = IS_BIT_SET(data, 11);
  ls.p1_menu = IS_BIT_SET(data, 12);
  ls.p2_menu = IS_BIT_SET(data, 13);
  ls.bass = IS_BIT_SET(data, 14) || IS_BIT_SET(data, 15);

  lm->SetAll(&ls);

  return true;
}
