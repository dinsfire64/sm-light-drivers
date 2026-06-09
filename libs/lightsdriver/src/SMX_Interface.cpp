#include "SMX_Interface.h"

#define SMX_DEBUG false
#define SMX_DISPLAY_DEBUG false

SMX_Interface::SMX_Interface() {
  light_state_mutex = new mutex();

  // clear the lighting state.
  ClearLights();

  background = new padstate();
  foreground = new padstate();

  memset(background, 0x00, sizeof(padstate));
  memset(foreground, 0x00, sizeof(padstate));

  MakeOverlayMask();
}

SMX_Interface::~SMX_Interface() {
  // printf("~SMX_Interface");
  return;

  if (run_thread) {
    StopThread();

    if (dllLoaded) {
      SMX_Stop_ptr();
    }
  }
}

bool SMX_Interface::isDllLoaded() { return dllLoaded; }

bool SMX_Interface::attemptDllLoad() {
  if (LoadSMXDLL(SMX_DLL_FILENAME)) {

#if SMX_DEBUG
    // Set a logging callback.  This can be called before SMX_Start.
    SMX_SetLogCallback_ptr(SMXLogCallback);
#endif

    // Start scanning.  The update callback will be called when devices connect
    // or disconnect or panels are pressed or released.  This callback will be
    // called from a thread.
    SMX_Start_ptr(SMXStateChangedCallback, this);

    dllLoaded = true;
  } else {
    dllLoaded = false;
  }

  return dllLoaded;
}

bool SMX_Interface::LoadSMXDLL(const char *dllPath) {
  HMODULE hModule = LoadLibrary(dllPath);
  if (!hModule) {
    DWORD err = GetLastError();

    // do not print error for "cannot be found" 126
    if (err != 126) {
      printf("Failed to load DLL: %s %lu\n", dllPath, err);
    }

    return false;
  }

  SMX_Start_ptr = (SMX_Start_t)GetProcAddress(hModule, "SMX_Start");
  SMX_Stop_ptr = (SMX_Stop_t)GetProcAddress(hModule, "SMX_Stop");
  SMX_SetLogCallback_ptr =
      (SMX_SetLogCallback_t)GetProcAddress(hModule, "SMX_SetLogCallback");
  SMX_GetInfo_ptr = (SMX_GetInfo_t)GetProcAddress(hModule, "SMX_GetInfo");
  SMX_GetInputState_ptr =
      (SMX_GetInputState_t)GetProcAddress(hModule, "SMX_GetInputState");
  SMX_SetLights_ptr = (SMX_SetLights_t)GetProcAddress(hModule, "SMX_SetLights");
  SMX_SetLights2_ptr =
      (SMX_SetLights2_t)GetProcAddress(hModule, "SMX_SetLights2");
  SMX_ReenableAutoLights_ptr = (SMX_ReenableAutoLights_t)GetProcAddress(
      hModule, "SMX_ReenableAutoLights");
  SMX_GetConfig_ptr = (SMX_GetConfig_t)GetProcAddress(hModule, "SMX_GetConfig");
  SMX_SetConfig_ptr = (SMX_SetConfig_t)GetProcAddress(hModule, "SMX_SetConfig");
  SMX_FactoryReset_ptr =
      (SMX_FactoryReset_t)GetProcAddress(hModule, "SMX_FactoryReset");
  SMX_ForceRecalibration_ptr = (SMX_ForceRecalibration_t)GetProcAddress(
      hModule, "SMX_ForceRecalibration");
  SMX_SetTestMode_ptr =
      (SMX_SetTestMode_t)GetProcAddress(hModule, "SMX_SetTestMode");
  SMX_GetTestData_ptr =
      (SMX_GetTestData_t)GetProcAddress(hModule, "SMX_GetTestData");
  SMX_SetPanelTestMode_ptr =
      (SMX_SetPanelTestMode_t)GetProcAddress(hModule, "SMX_SetPanelTestMode");
  SMX_Version_ptr = (SMX_Version_t)GetProcAddress(hModule, "SMX_Version");

  if (!SMX_Start_ptr || !SMX_Stop_ptr || !SMX_SetLogCallback_ptr ||
      !SMX_GetInfo_ptr || !SMX_GetInputState_ptr || !SMX_SetLights_ptr ||
      !SMX_SetLights2_ptr || !SMX_ReenableAutoLights_ptr ||
      !SMX_GetConfig_ptr || !SMX_SetConfig_ptr || !SMX_FactoryReset_ptr ||
      !SMX_ForceRecalibration_ptr || !SMX_SetTestMode_ptr ||
      !SMX_GetTestData_ptr || !SMX_SetPanelTestMode_ptr || !SMX_Version_ptr) {
    printf("Failed to get one or more function addresses.\n");
    FreeLibrary(hModule);
    return false;
  }

  return true;
}

void SMX_Interface::ClearLights() {
  light_state_mutex->lock();
  // memset(smx_light_state, 0, sizeof(smx_light_state));
  light_state_mutex->unlock();
}

void SMX_Interface::SMXStateChangedCallback(int pad,
                                            SMXUpdateCallbackReason reason,
                                            void *pUser) {
  SMX_Interface *pSelf = (SMX_Interface *)pUser;
  pSelf->SMXStateChanged(pad, reason);
}

void SMX_Interface::SMXLogCallback(const char *log) {
#if SMX_DEBUG
  printf("SMX: %s\n", log);
#endif
}

void SMX_Interface::SMXStateChanged(int pad, SMXUpdateCallbackReason reason) {
#if SMX_DEBUG
  printf("SMX: Device %i state changed: %04x\n", pad,
         SMX_GetInputState_ptr(pad));
#endif
}

void SMX_Interface::MakeMaskFeather() {
  memset(fg_pattern, 0x00, NUM_OF_LEDS * sizeof(bool));

  int cornerPanels[] = {0, 2, 6, 8};

  for (int j = 0; j < sizeof(cornerPanels) / sizeof(int); j++) {
    int offset = (LEDS_PER_PANEL)*cornerPanels[j];

    for (int i = 0; i < LEDS_PER_PANEL; i++) {
      // make a pattern
      fg_pattern[offset + i] = ((i) % 5 == 0) || ((i) % 3 == 0);
    }
  }
}

void SMX_Interface::StartThread() {
  // Make a thread to feed lighting data to the pad to
  // prevent reverting back to auto lights.
  run_thread = true;
  UpdateThreadObject = new std::thread(&SMX_Interface::UpdateThread, this);
}

void SMX_Interface::StopThread() {
  if (run_thread && UpdateThreadObject != nullptr) {
    run_thread = false;
    UpdateThreadObject->join();
  }
}

void SMX_Interface::SetOverlayState(bool on) {
  for (int i = 0; i < NUM_OF_PANELS; i++) {
    foreground->panel_on[i] = on;
  }
}

void SMX_Interface::Update_Light(smx_pad_light_map light, bool light_on) {
  light_state_mutex->lock();

  if (light_on) {
    // set the color in the background.
    int offset = BYTES_PER_PANEL * light.smx_light_panel_offset;

    for (int i = 0; i < BYTES_PER_PANEL; i += 3) {
      background->pattern[offset + (i + 0)] = light.r;
      background->pattern[offset + (i + 1)] = light.g;
      background->pattern[offset + (i + 2)] = light.b;

      // ensure that the light to be duplicated is on p1 side.
      if (light.smx_light_panel_offset < 9 && light.repeat_on_both) {
        // Skip to p2 by dividing the total number of bytes in half.
        background->pattern[((NUM_OF_RGB_LEDS / 2) + offset) + (i + 0)] =
            light.r;
        background->pattern[((NUM_OF_RGB_LEDS / 2) + offset) + (i + 1)] =
            light.g;
        background->pattern[((NUM_OF_RGB_LEDS / 2) + offset) + (i + 2)] =
            light.b;
      }
    }
  }

  // turn on the light.
  background->panel_on[light.smx_light_panel_offset] = light_on;

  // copy the light state to the second side if enabled.
  if (light.repeat_on_both) {
    background->panel_on[light.smx_light_panel_offset + PANELS_PER_STAGE] =
        light_on;
  }

  if (light.fade_disabled) {
    // if fade is disabled, then we need to min/max the brightness here to
    // bypass the thread that changes the brightness.
    float lightState = light_on ? MAX_BRIGHTNESS : MIN_BRIGHTNESS;

    background->panel_alpha[light.smx_light_panel_offset] = lightState;

    if (light.repeat_on_both) {
      background->panel_alpha[light.smx_light_panel_offset + PANELS_PER_STAGE] =
          lightState;
    }
  } else {
    // if fade is enabled, the panel_on state will fade on/off the color
    // accordingly
  }

  light_state_mutex->unlock();
}

void SMX_Interface::UpdateThread() {
  while (run_thread) {
    SendLightingState();

    this_thread::sleep_for(std::chrono::milliseconds(SMX_THREAD_MS));
  }
}

void SMX_Interface::FadeState(padstate *pattern) {
  for (int i = 0; i < NUM_OF_PANELS; i++) {
    if (pattern->panel_on[i]) {
      pattern->panel_alpha[i] =
          std::min(MAX_BRIGHTNESS, pattern->panel_alpha[i] + fadeOn);
    } else {
      pattern->panel_alpha[i] =
          std::max(MIN_BRIGHTNESS, pattern->panel_alpha[i] - fadeOff);
    }
  }
}

void SMX_Interface::SendLightingState() {
  light_state_mutex->lock();

  FadeState(foreground);
  FadeState(background);
  CombineStates();

  SMX_SetLights2_ptr(finalLightState, NUM_OF_RGB_LEDS);

#if SMX_DISPLAY_DEBUG
  system("cls");

  // int panelsOfInterest[] = { 0,1,2,3,4,5,6,7,8 };
  // int panelsOfInterest[] = { 9,10,11,12,13,14,15,16,17 };
  // int panelsOfInterest[] = { 4,13 };

  for (int j = 0; j < sizeof(panelsOfInterest) / sizeof(int); j++) {
    DebugPrintPanel(panelsOfInterest[j]);
  }
#endif

  light_state_mutex->unlock();
}

void SMX_Interface::MakeOverlayMask() {
  memset(fg_pattern, 0x00, NUM_OF_LEDS * sizeof(bool));

  for (int i = 0; i < NUM_OF_STAGES; i++) {
    for (int j = 0; j < PANELS_PER_STAGE; j++) {
      int offset =
          (j * LEDS_PER_PANEL) + (i * (PANELS_PER_STAGE * LEDS_PER_PANEL));

      if (j == 0 || j == 2) {
        // top bar
        fg_pattern[offset + 0] = true;
        fg_pattern[offset + 1] = true;
        fg_pattern[offset + 2] = true;
        fg_pattern[offset + 3] = true;
      } else if (j == 6 || j == 8) {
        // bottom bar
        fg_pattern[offset + 12] = true;
        fg_pattern[offset + 13] = true;
        fg_pattern[offset + 14] = true;
        fg_pattern[offset + 15] = true;
      }

      if (j == 0 || j == 6) {
        // left bar
        fg_pattern[offset + 0] = true;
        fg_pattern[offset + 4] = true;
        fg_pattern[offset + 8] = true;
        fg_pattern[offset + 12] = true;
      } else if (j == 2 || j == 8) {
        // right bar
        fg_pattern[offset + 3] = true;
        fg_pattern[offset + 7] = true;
        fg_pattern[offset + 11] = true;
        fg_pattern[offset + 15] = true;
      }
    }
  }
}

void SMX_Interface::SetOverlayColor(smx_pad_light_map light) {
  for (int j = 0; j < PANELS_PER_STAGE * NUM_OF_STAGES; j++) {
    int offset = BYTES_PER_PANEL * j;

    for (int i = 0; i < BYTES_PER_PANEL; i += 3) {
      foreground->pattern[offset + (i + 0)] = light.r;
      foreground->pattern[offset + (i + 1)] = light.g;
      foreground->pattern[offset + (i + 2)] = light.b;
    }
  }
}

void SMX_Interface::DebugPrintPanel(int panelNum) {
  printf("%d:\r\n", panelNum);
  int panelnumoffset = BYTES_PER_PANEL * panelNum;

  for (int i = 0; i < CHANNELS_PER_LED * 16; i++) {
    printf("%03d", finalLightState[panelnumoffset + i] & 0xFF);

    if ((i + 1) % (COLUMN_LED * CHANNELS_PER_LED) == 0) {
      printf("\r\n");
    } else if ((i + 1) % CHANNELS_PER_LED == 0) {
      printf("-");
    }
  }
}

uint8_t SMX_Interface::MixColors(uint8_t fgColor, float fgAlpha,
                                 uint8_t bgColor, float bgAlpha,
                                 float targetAlpha) {
  return fgColor * (fgAlpha / targetAlpha) +
         bgColor * bgAlpha * ((1 - fgAlpha) / targetAlpha);
}

void SMX_Interface::CombineStates() {
  for (int i = 0; i < NUM_OF_LEDS; i++) {
    int offset = i * CHANNELS_PER_LED;
    int panelnum = i / LEDS_PER_PANEL;

    if (fg_pattern[i]) {
      float res_alpha = 1 - (1 - background->panel_alpha[panelnum]) *
                                (1 - foreground->panel_alpha[panelnum]);

      if (res_alpha < fadeOff) {
        // resulting color is too dim, just make it off.
        finalLightState[offset + 0] = 0;
        finalLightState[offset + 1] = 0;
        finalLightState[offset + 2] = 0;
      } else if (foreground->panel_alpha[panelnum] < fadeOn) {
        // foreground is off, use the background
        finalLightState[offset + 0] =
            background->pattern[offset + 0] * background->panel_alpha[panelnum];
        finalLightState[offset + 1] =
            background->pattern[offset + 1] * background->panel_alpha[panelnum];
        finalLightState[offset + 2] =
            background->pattern[offset + 2] * background->panel_alpha[panelnum];
      } else if (background->panel_alpha[panelnum] < fadeOn) {
        // background is off, use the foreground
        finalLightState[offset + 0] =
            foreground->pattern[offset + 0] * foreground->panel_alpha[panelnum];
        finalLightState[offset + 1] =
            foreground->pattern[offset + 1] * foreground->panel_alpha[panelnum];
        finalLightState[offset + 2] =
            foreground->pattern[offset + 2] * foreground->panel_alpha[panelnum];
      } else {
        // mix the color.
        finalLightState[offset + 0] = MixColors(
            foreground->pattern[offset + 0], foreground->panel_alpha[panelnum],
            background->pattern[offset + 0], background->panel_alpha[panelnum],
            res_alpha);
        finalLightState[offset + 1] = MixColors(
            foreground->pattern[offset + 1], foreground->panel_alpha[panelnum],
            background->pattern[offset + 1], background->panel_alpha[panelnum],
            res_alpha);
        finalLightState[offset + 2] = MixColors(
            foreground->pattern[offset + 2], foreground->panel_alpha[panelnum],
            background->pattern[offset + 2], background->panel_alpha[panelnum],
            res_alpha);
      }
    } else {
      // no foreground to apply.
      finalLightState[offset + 0] =
          background->pattern[offset + 0] * background->panel_alpha[panelnum];
      finalLightState[offset + 1] =
          background->pattern[offset + 1] * background->panel_alpha[panelnum];
      finalLightState[offset + 2] =
          background->pattern[offset + 2] * background->panel_alpha[panelnum];
    }
  }
}
