#pragma once

#include "HIDHelper.h"
#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

// static information about the device(s) in question.
#define STAC2_VID 0x2E8A
#define STAC2_PID_P1 0x10D9
#define STAC2_PID_P2 0x10E9
#define STAC2_NUMOF_LIGHTS 8

// the first byte of the buffer is a static report id.
#define STAC2_HIDREPORT_SIZE (STAC2_NUMOF_LIGHTS + 1)
#define STAC2_REPORT_ID 0x01
#define STAC2_LIGHTING_INTERFACE 0x02

// total number of supported devices.
#define STAC2_MAX_NUMBER 2

enum Stac2LightIndex {
  STAC2_LIGHTINDEX_REPORT = 0,

  STAC2_LIGHTINDEX_BTN1,
  STAC2_LIGHTINDEX_BTN2,
  STAC2_LIGHTINDEX_BTN3,
  STAC2_LIGHTINDEX_BTN4,
  STAC2_LIGHTINDEX_BTN5,
  STAC2_LIGHTINDEX_BTN6,
  STAC2_LIGHTINDEX_BTN7,
  STAC2_LIGHTINDEX_BTN8,

  STAC2_LIGHTINDEX_MAX
};

static_assert(STAC2_LIGHTINDEX_MAX == STAC2_HIDREPORT_SIZE,
              "Incorrect STAC2 StacLightIndex");

class LightsDriverStac2 : public LightsDriver {
public:
  LightsDriverStac2();
  ~LightsDriverStac2() override;

  bool Connect() override;

  void Set(const LightsState *ls) override;

  void Disconnect() override;

private:
  uint8_t outputBuffer[STAC2_MAX_NUMBER][STAC2_HIDREPORT_SIZE];
  uint8_t prevOutputBuffer[STAC2_MAX_NUMBER][STAC2_HIDREPORT_SIZE];

  HIDHelper *hidHelper[STAC2_MAX_NUMBER];
};