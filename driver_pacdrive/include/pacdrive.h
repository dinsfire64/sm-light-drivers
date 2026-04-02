#ifndef PACDRIVE_H_
#define PACDRIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

// ---- Initialization and general ----
void EXPORT PacSetCallbacks(void * /*connectCb*/, void * /*disconnectCb*/) {}
bool EXPORT PacInitialize(void);
void EXPORT PacShutdown(void) {}

// ---- LED control ----
bool EXPORT PacSetLEDState(DWORD /*id*/, DWORD /*port*/, bool /*state*/) {
  return true;
}
bool EXPORT PacSetLEDStates(int deviceId, short int data);

bool EXPORT Pac64SetLEDStates(DWORD /*id*/, const bool * /*states*/,
                              DWORD /*length*/) {
  return true;
}
bool EXPORT Pac64SetLEDState(DWORD /*id*/, DWORD /*port*/, bool /*state*/) {
  return true;
}
bool EXPORT Pac64SetLEDStatesRandom(DWORD /*id*/, const bool * /*states*/,
                                    DWORD /*length*/) {
  return true;
}
bool EXPORT Pac64SetLEDIntensities(DWORD /*id*/, const BYTE * /*intensities*/,
                                   DWORD /*length*/) {
  return true;
}
bool EXPORT Pac64SetLEDIntensity(DWORD /*id*/, DWORD /*port*/,
                                 BYTE /*intensity*/) {
  return true;
}
bool EXPORT Pac64SetLEDFadeTime(DWORD /*id*/, DWORD /*fadeTime*/) {
  return true;
}
bool EXPORT Pac64SetLEDFlashSpeeds(DWORD /*id*/, const BYTE * /*speeds*/,
                                   DWORD /*length*/) {
  return true;
}
bool EXPORT Pac64SetLEDFlashSpeed(DWORD /*id*/, DWORD /*port*/,
                                  BYTE /*speed*/) {
  return true;
}

// ---- Scripting ----
bool EXPORT Pac64StartScriptRecording(DWORD /*id*/) { return true; }
bool EXPORT Pac64StopScriptRecording(DWORD /*id*/) { return true; }
bool EXPORT Pac64SetScriptStepDelay(DWORD /*id*/, DWORD /*delay*/) {
  return true;
}
bool EXPORT Pac64RunScript(DWORD /*id*/) { return true; }
bool EXPORT Pac64ClearFlash(DWORD /*id*/) { return true; }

// ---- Device information ----
bool EXPORT Pac64SetDeviceId(DWORD /*id*/, DWORD /*newId*/) { return true; }
DWORD EXPORT PacGetDeviceType(DWORD /*id*/) { return 0; }
DWORD EXPORT PacGetVendorId(DWORD /*id*/) { return 0; }
DWORD EXPORT PacGetProductId(DWORD /*id*/) { return 0; }
DWORD EXPORT PacGetVersionNumber(DWORD /*id*/) { return 0; }

const char *EXPORT PacGetVendorName(DWORD /*id*/) { return "Vendor"; }
const char *EXPORT PacGetProductName(DWORD /*id*/) { return "Product"; }
const char *EXPORT PacGetSerialNumber(DWORD /*id*/) { return "00000000"; }
const char *EXPORT PacGetDevicePath(DWORD /*id*/) { return "C:\\Path"; }

// ---- Programming / configuration ----
bool EXPORT PacProgramUHid(DWORD /*id*/, const void * /*data*/,
                           DWORD /*length*/) {
  return true;
}
bool EXPORT PacSetServoStik4Way(DWORD /*id*/) { return true; }
bool EXPORT PacSetServoStik8Way(DWORD /*id*/) { return true; }

bool EXPORT USBButtonConfigurePermanent(DWORD /*id*/, DWORD /*color*/) {
  return true;
}
bool EXPORT USBButtonConfigureTemporary(DWORD /*id*/, DWORD /*color*/) {
  return true;
}
bool EXPORT USBButtonConfigureColor(DWORD /*id*/, DWORD /*color*/) {
  return true;
}

// ---- Firmware update (C++ mangled symbol) ----
int EXPORT Pac64UpdateFirmware(int /*param*/) { return 0; }

#ifdef __cplusplus
}
#endif

#endif // PACDRIVE_H_
