#pragma once

#include "SMX.h"
#include <map>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>
#include <windows.h>

// Function pointers
typedef void (*SMX_Start_t)(SMXUpdateCallback, void *);
typedef void (*SMX_Stop_t)();
typedef void (*SMX_SetLogCallback_t)(SMXLogCallback);
typedef void (*SMX_GetInfo_t)(int, SMXInfo *);
typedef uint16_t (*SMX_GetInputState_t)(int);
typedef void (*SMX_SetLights_t)(const char[864]);
typedef void (*SMX_SetLights2_t)(const char *, int);
typedef void (*SMX_ReenableAutoLights_t)();
typedef bool (*SMX_GetConfig_t)(int, SMXConfig *);
typedef void (*SMX_SetConfig_t)(int, const SMXConfig *);
typedef void (*SMX_FactoryReset_t)(int);
typedef void (*SMX_ForceRecalibration_t)(int);
typedef void (*SMX_SetTestMode_t)(int, SensorTestMode);
typedef bool (*SMX_GetTestData_t)(int, SMXSensorTestModeData *);
typedef void (*SMX_SetPanelTestMode_t)(PanelTestMode);
typedef const char *(*SMX_Version_t)();

using namespace std;

enum SMXLightIndex {
  P1_UPPER_LFT = 0,
  P1_UPPER_MID,
  P1_UPPER_RGT,
  P1_MIDDLE_LFT,
  P1_MIDDLE_MID,
  P1_MIDDLE_RGT,
  P1_LOWER_LFT,
  P1_LOWER_MID,
  P1_LOWER_RGT,

  P2_UPPER_LFT,
  P2_UPPER_MID,
  P2_UPPER_RGT,
  P2_MIDDLE_LFT,
  P2_MIDDLE_MID,
  P2_MIDDLE_RGT,
  P2_LOWER_LFT,
  P2_LOWER_MID,
  P2_LOWER_RGT,
};

struct smx_pad_light_map {
  SMXLightIndex smx_light_panel_offset;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  bool repeat_on_both;
  bool fade_disabled;
};

#define CHANNELS_PER_LED 3
#define LEDS_PER_PANEL 25
#define PANELS_PER_STAGE 9
#define NUM_OF_STAGES 2

#define BYTES_PER_PANEL (CHANNELS_PER_LED * LEDS_PER_PANEL)
#define BYTES_PER_STAGE (BYTES_PER_PANEL * PANELS_PER_STAGE)
#define NUM_OF_RGB_LEDS (NUM_OF_STAGES * BYTES_PER_STAGE)

#define NUM_OF_LEDS (LEDS_PER_PANEL * PANELS_PER_STAGE * NUM_OF_STAGES)
#define NUM_OF_PANELS (PANELS_PER_STAGE * NUM_OF_STAGES)

#define COLUMN_LED 4
#define ROW_LED (LEDS_PER_PANEL / COLUMN_LED)

#define FADE_DEF_STEP 0.25

#define MIN_BRIGHTNESS 0.0
#define MAX_BRIGHTNESS 1.0

#define SMX_THREAD_MS 32

#define SMX_DLL_FILENAME "SMX.dll"

struct padstate {
  uint8_t pattern[NUM_OF_RGB_LEDS];
  bool panel_on[NUM_OF_PANELS];
  float panel_alpha[NUM_OF_PANELS];
};

class SMX_Interface {
private:
  std::mutex *light_state_mutex;

  bool dllLoaded = false;

  padstate *background;
  padstate *foreground;

  bool fg_pattern[NUM_OF_LEDS];

  char finalLightState[NUM_OF_RGB_LEDS];

  std::thread *UpdateThreadObject = nullptr;
  bool run_thread;

  float fadeOn = FADE_DEF_STEP;
  float fadeOff = FADE_DEF_STEP;

  void UpdateThread();

  void MakeOverlayMask();

  void DebugPrintPanel(int panelNum);

  static void SMXStateChangedCallback(int pad, SMXUpdateCallbackReason reason,
                                      void *pUser);
  static void SMXLogCallback(const char *log);
  void SMXStateChanged(int pad, SMXUpdateCallbackReason reason);

  uint8_t MixColors(uint8_t fgColor, float fgAlpha, uint8_t bgColor,
                    float bgAlpha, float targetAlpha);
  void CombineStates();

  void FadeState(padstate *pattern);

  void MakeMaskFeather();

  bool LoadSMXDLL(const char *dllPath);

  // Global pointers
  SMX_Start_t SMX_Start_ptr = nullptr;
  SMX_Stop_t SMX_Stop_ptr = nullptr;
  SMX_SetLogCallback_t SMX_SetLogCallback_ptr = nullptr;
  SMX_GetInfo_t SMX_GetInfo_ptr = nullptr;
  SMX_GetInputState_t SMX_GetInputState_ptr = nullptr;
  SMX_SetLights_t SMX_SetLights_ptr = nullptr;
  SMX_SetLights2_t SMX_SetLights2_ptr = nullptr;
  SMX_ReenableAutoLights_t SMX_ReenableAutoLights_ptr = nullptr;
  SMX_GetConfig_t SMX_GetConfig_ptr = nullptr;
  SMX_SetConfig_t SMX_SetConfig_ptr = nullptr;
  SMX_FactoryReset_t SMX_FactoryReset_ptr = nullptr;
  SMX_ForceRecalibration_t SMX_ForceRecalibration_ptr = nullptr;
  SMX_SetTestMode_t SMX_SetTestMode_ptr = nullptr;
  SMX_GetTestData_t SMX_GetTestData_ptr = nullptr;
  SMX_SetPanelTestMode_t SMX_SetPanelTestMode_ptr = nullptr;
  SMX_Version_t SMX_Version_ptr = nullptr;

public:
  SMX_Interface();
  ~SMX_Interface();

  bool isDllLoaded();
  bool attemptDllLoad();

  void StartThread();
  void StopThread();

  void ClearLights();

  void SetOverlayColor(smx_pad_light_map light);
  void SetOverlayState(bool on);

  void Update_Light(smx_pad_light_map light, bool light_on);
  void SendLightingState();

  void SetFadeOn(float a) { fadeOn = a; }
  void SetFadeOff(float a) { fadeOff = a; }
};