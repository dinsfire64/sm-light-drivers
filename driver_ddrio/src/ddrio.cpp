#define WIN32_LEAN_AND_MEAN

#include <LightsManager.h>

#include "ddrio.h"

#ifndef IS_SET
#define IS_SET(var, mask) (((var) & ((mask))) == mask ? 1 : 0)
#endif

LightsManager *lm = new LightsManager();
LightsState light_state;

bool is_hd;

uint32_t p3io, extio;

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

void update_device(uint32_t p3io, uint32_t extio) {
  // Check to see if we have SD or HD lighting by checking for NEON bit.
  // is_hd starts out as true, and will flip false if we see the NEON bit.
  if (is_hd && IS_SET(extio, DDR_NEON)) {
    is_hd = false;
  }

  if (is_hd) {
    light_state.bass =
        IS_SET(p3io, DDR_SAT_SPOT_BLUE) && IS_SET(p3io, DDR_SAT_SPOT_RED);

    light_state.marquee_up_left = IS_SET(p3io, DDR_SAT_SPOT_BLUE);
    light_state.marquee_lr_left = IS_SET(p3io, DDR_SAT_SPOT_RED);
    light_state.marquee_up_right = IS_SET(p3io, DDR_TOP_SPOT_BLUE);
    light_state.marquee_lr_right = IS_SET(p3io, DDR_TOP_SPOT_RED);
  } else {
    light_state.bass = IS_SET(extio, DDR_NEON);

    light_state.marquee_up_left = IS_SET(p3io, DDR_P1_HALOGEN_UPPER);
    light_state.marquee_lr_left = IS_SET(p3io, DDR_P1_HALOGEN_LOWER);
    light_state.marquee_up_right = IS_SET(p3io, DDR_P2_HALOGEN_UPPER);
    light_state.marquee_lr_right = IS_SET(p3io, DDR_P2_HALOGEN_LOWER);
  }

  light_state.p1_menu = IS_SET(p3io, DDR_P1_MENU);
  light_state.p2_menu = IS_SET(p3io, DDR_P2_MENU);

  light_state.p1_up = IS_SET(extio, DDR_LIGHT_P1_UP);
  light_state.p1_down = IS_SET(extio, DDR_LIGHT_P1_DOWN);
  light_state.p1_left = IS_SET(extio, DDR_LIGHT_P1_LEFT);
  light_state.p1_right = IS_SET(extio, DDR_LIGHT_P1_RIGHT);

  light_state.p2_up = IS_SET(extio, DDR_LIGHT_P2_UP);
  light_state.p2_down = IS_SET(extio, DDR_LIGHT_P2_DOWN);
  light_state.p2_left = IS_SET(extio, DDR_LIGHT_P2_LEFT);
  light_state.p2_right = IS_SET(extio, DDR_LIGHT_P2_RIGHT);

  lm->SetAll(&light_state);
}

void ddr_io_set_loggers(log_formatter_t misc, log_formatter_t info,
                        log_formatter_t warning, log_formatter_t fatal) {}

int ddr_io_fini() {
  // cleanup is done by the detach.
  return 0;
}

bool ddr_io_init(thread_create_t thread_create, thread_join_t thread_join,
                 thread_destroy_t thread_destroy) {

  // default to true as sd mode doesn't output neon
  is_hd = true;

  lm->Initialize();
  return true;
}

int ddr_io_read_pad() { return 0; }

void ddr_io_set_lights_p3io(uint32_t lights) {

  p3io = lights;

  update_device(p3io, extio);
}

void ddr_io_set_lights_extio(uint32_t lights) {
  extio = lights;

  update_device(p3io, extio);
}

// unused.
void ddr_io_set_lights_hdxs_panel(uint32_t lights) {}
void ddr_io_set_lights_hdxs_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {}
