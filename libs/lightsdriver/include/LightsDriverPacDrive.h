#pragma once

#include "HIDHelper.h"
#include "LightsDriver.h"
#include "LightsManager.h"

#include <cstdint>
#include <string.h>

// static information about the device in question.
#define PACDRIVE_VID 0xD209

// PacDrive PIDs range from 0x1500->0x1507
// but most of them are this.
#define PACDRIVE_PID 0x1500

#define PACDRIVE_INTERFACE 0

// the first byte of the buffer is a static report id.
// and I have no idea why ultimarc's report is 5 bytes wide...
#define PACDRIVE_HIDREPORT_SIZE 5
#define PACDRIVE_HIDREPORT_ID 0x00

#pragma pack(push, 1)

typedef union {
  struct {
    // NOTE: this is intentionally byte swapped
    // as Ultimarc's library does this, the firmware expects this order.
    // this matches the physical location of each output with the variable name.
    // see this code snippet:
    // https://github.com/itgmania/itgmania/issues/921#issuecomment-3008263137
    bool led09 : 1;
    bool led10 : 1;
    bool led11 : 1;
    bool led12 : 1;
    bool led13 : 1;
    bool led14 : 1;
    bool led15 : 1;
    bool led16 : 1;

    bool led01 : 1;
    bool led02 : 1;
    bool led03 : 1;
    bool led04 : 1;
    bool led05 : 1;
    bool led06 : 1;
    bool led07 : 1;
    bool led08 : 1;
  };
  uint16_t raw;
} pacdrive_leds_t;

#pragma pack(pop)

typedef union {
  struct {
    uint8_t report_id;
    uint8_t pad0;
    uint8_t pad1;
    pacdrive_leds_t leds;
  };
  uint8_t raw_state[PACDRIVE_HIDREPORT_SIZE];
} pacdrive_state_t;

class LightsDriverPacDrive : public LightsDriver {
public:
  LightsDriverPacDrive();
  ~LightsDriverPacDrive() override;

  bool Connect() override;

  void Set(const LightsState *ls) override;

  void Disconnect() override;

private:
  pacdrive_leds_t outputLEDs;
  pacdrive_leds_t prevoutputLEDs;

  HIDHelper *hidHelper;

  void PushState();
};