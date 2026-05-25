#pragma once

#include <windows.h>

#include "glue.h"

enum ddr_light_bit {

  DDR_LIGHT_P1_RIGHT = 0x8000000,
  DDR_LIGHT_P1_LEFT = 0x10000000,
  DDR_LIGHT_P1_UP = 0x40000000,
  DDR_LIGHT_P1_DOWN = 0x20000000,

  DDR_LIGHT_P2_RIGHT = 0x80000,
  DDR_LIGHT_P2_LEFT = 0x100000,
  DDR_LIGHT_P2_UP = 0x400000,
  DDR_LIGHT_P2_DOWN = 0x200000,

  // HD lights
  DDR_TOP_SPOT_RED = 0x4,
  DDR_TOP_SPOT_BLUE = 0x8,

  DDR_SAT_SPOT_RED = 0x1,
  DDR_SAT_SPOT_BLUE = 0x2,

  // SD lights
  DDR_P1_HALOGEN_UPPER = 0x80,
  DDR_P1_HALOGEN_LOWER = 0x40,
  DDR_P2_HALOGEN_UPPER = 0x20,
  DDR_P2_HALOGEN_LOWER = 0x10,

  DDR_P1_MENU = 0x1,
  DDR_P2_MENU = 0x2,

  DDR_NEON = 0x4000,
};

#define EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

EXPORT void ddr_io_set_loggers(log_formatter_t misc, log_formatter_t info,
                               log_formatter_t warning, log_formatter_t fatal);
EXPORT int ddr_io_fini();
EXPORT bool ddr_io_init(thread_create_t thread_create,
                        thread_join_t thread_join,
                        thread_destroy_t thread_destroy);

EXPORT int ddr_io_read_pad();

EXPORT void ddr_io_set_lights_extio(uint32_t lights);
EXPORT void ddr_io_set_lights_p3io(uint32_t lights);
EXPORT void ddr_io_set_lights_hdxs_panel(uint32_t lights);
EXPORT void ddr_io_set_lights_hdxs_rgb(uint8_t idx, uint8_t r, uint8_t g,
                                       uint8_t b);

#ifdef __cplusplus
}
#endif
