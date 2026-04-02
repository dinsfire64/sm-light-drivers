#pragma once

#include "HIDHelper.h"
#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

#define FUSION_VID 0x0547
#define FUSION_PID 0x1337

#define FUSION_INTERFACENUM -1

#define LIGHTING_REPORTID 0x02

// five player, five player, neon, four marquee, coin, led
#define TOTAL_LIGHTS (5 + 5 + 1 + 4 + 1 + 1)

// number of lights plus report id.
#define HID_LIGHTS_REPORT_SIZE (1 + TOTAL_LIGHTS)

enum FusionReportIndex {
  FUSION_REPORT_ID = 0,

  FUSION_P1_UL,
  FUSION_P1_UR,
  FUSION_P1_CN,
  FUSION_P1_LL,
  FUSION_P1_LR,

  FUSION_P2_UL,
  FUSION_P2_UR,
  FUSION_P2_CN,
  FUSION_P2_LL,
  FUSION_P2_LR,

  FUSION_NEON,

  FUSION_MAR_UL,
  FUSION_MAR_UR,
  FUSION_MAR_LL,
  FUSION_MAR_LR,

  FUSION_COIN_PULSE,
  FUSION_LED,

  FUISON_REPORT_MAX
};

static_assert(FUISON_REPORT_MAX == HID_LIGHTS_REPORT_SIZE,
              "Incorrect FusionReportIndex");

class LightsDriverFusionGamepad : public LightsDriver {
public:
  LightsDriverFusionGamepad();
  ~LightsDriverFusionGamepad() override;

  bool Connect() override;

  void Set(const LightsState *ls) override;

  void Disconnect() override;

private:
  uint8_t outputBuffer[FUISON_REPORT_MAX];
  uint8_t prevOutputBuffer[FUISON_REPORT_MAX];

  HIDHelper *hidHelper;
};