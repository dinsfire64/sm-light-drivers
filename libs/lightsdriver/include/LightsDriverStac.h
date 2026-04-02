#pragma once

#include "HIDHelper.h"
#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

// static information about the device(s) in question.
#define STAC_VID 0x04d8
#define STAC_PID_P1 0xea4b
#define STAC_PID_P2 0xea4a
#define STAC_NUMOF_LIGHTS 5

// the first byte of the buffer is a static report id.
#define STAC_HIDREPORT_SIZE (STAC_NUMOF_LIGHTS + 1)
#define STAC_REPORT_ID 0x01
#define STAC_LIGHTING_INTERFACE 0x01

// total number of supported devices.
#define STAC_MAX_NUMBER 2

enum StacLightIndex {
  STAC_LIGHTINDEX_REPORT = 0,

  STAC_LIGHTINDEX_BTN1,
  STAC_LIGHTINDEX_BTN2,
  STAC_LIGHTINDEX_BTN3,
  STAC_LIGHTINDEX_BTN4,
  STAC_LIGHTINDEX_BTN5,

  STAC_REPORTINDEX_MAX
};

static_assert(STAC_REPORTINDEX_MAX == STAC_HIDREPORT_SIZE,
              "Incorrect STAC StacLightIndex");

class LightsDriverStac : public LightsDriver {
public:
  LightsDriverStac();
  ~LightsDriverStac() override;

  bool Connect() override;

  void Set(const LightsState *ls) override;

  void Disconnect() override;

private:
  uint8_t outputBuffer[STAC_MAX_NUMBER][STAC_HIDREPORT_SIZE];
  uint8_t prevOutputBuffer[STAC_MAX_NUMBER][STAC_HIDREPORT_SIZE];

  HIDHelper *hidHelper[STAC_MAX_NUMBER];
};