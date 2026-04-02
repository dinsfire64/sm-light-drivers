#pragma once

#include "HIDHelper.h"
#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

#define GPB_VID 0x04d8
#define GPB_PID 0xea6a
#define GPB_NUMOF_LIGHTS 16

// the first byte of the buffer is a static report id.
#define GPB_REPORT_ID 0x01
#define GPB_LIGHTING_INTERFACENUM 0x01

enum GpbLightIndex {
  GPB_LIGHTINDEX_REPORT_ID = 0,

  GPB_LIGHTINDEX_BTN01,
  GPB_LIGHTINDEX_BTN02,
  GPB_LIGHTINDEX_BTN03,
  GPB_LIGHTINDEX_BTN04,
  GPB_LIGHTINDEX_BTN05,
  GPB_LIGHTINDEX_BTN06,
  GPB_LIGHTINDEX_BTN07,
  GPB_LIGHTINDEX_BTN08,
  GPB_LIGHTINDEX_BTN09,
  GPB_LIGHTINDEX_BTN10,
  GPB_LIGHTINDEX_BTN11,
  GPB_LIGHTINDEX_BTN12,
  GPB_LIGHTINDEX_BTN13,
  GPB_LIGHTINDEX_BTN14,
  GPB_LIGHTINDEX_BTN15,
  GPB_LIGHTINDEX_BTN16,

  GPB_LIGHTINDEX_MAX
};

class LightsDriverGPB : public LightsDriver {
public:
  LightsDriverGPB();
  ~LightsDriverGPB() override;

  bool Connect() override;

  void Set(const LightsState *ls) override;

  void Disconnect() override;

private:
  uint8_t outputBuffer[GPB_LIGHTINDEX_MAX];
  uint8_t prevOutputBuffer[GPB_LIGHTINDEX_MAX];

  HIDHelper *hidHelper;
};