#include "LightsDriverSnek.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverSnek::LightsDriverSnek() {
  hidHelper = new HIDHelper(SNEK_VID, SNEK_PID, SNEK_LIGHTING_INTERFACENUM);

  memset(outputBuffer, 0, sizeof(outputBuffer));
};

LightsDriverSnek::~LightsDriverSnek() {};

bool LightsDriverSnek::Connect() {
  if (hidHelper->IsOpen()) {
    // already connected
    return true;
  }

  return hidHelper->Open();
}

void LightsDriverSnek::Disconnect() { hidHelper->Close(); }

void LightsDriverSnek::Set(const LightsState *ls) {
  if (!hidHelper->IsOpen()) {
    // attempt to open if not called already.
    Connect();
  }

  outputBuffer[SNEK_LIGHTINDEX_REPORT_ID] = SNEK_REPORT_ID;

  outputBuffer[SNEK_INDEX_DANCE_M_LR] = ls->marquee_lr_right ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_M_UR] = ls->marquee_up_right ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_M_LL] = ls->marquee_lr_left ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_M_UL] = ls->marquee_up_left ? 0xFF : 0x00;

  outputBuffer[SNEK_INDEX_DANCE_NEON] = ls->bass ? 0xFF : 0x00;

  outputBuffer[SNEK_INDEX_DANCE_P1_START] = ls->p1_menu ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P2_START] = ls->p2_menu ? 0xFF : 0x00;

  outputBuffer[SNEK_INDEX_DANCE_P1_UP] = ls->p1_up ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P1_DOWN] = ls->p1_down ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P1_LEFT] = ls->p1_left ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P1_RIGHT] = ls->p1_right ? 0xFF : 0x00;

  outputBuffer[SNEK_INDEX_DANCE_P2_UP] = ls->p2_up ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P2_DOWN] = ls->p2_down ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P2_LEFT] = ls->p2_left ? 0xFF : 0x00;
  outputBuffer[SNEK_INDEX_DANCE_P2_RIGHT] = ls->p2_right ? 0xFF : 0x00;

  if (memcmp(outputBuffer, prevOutputBuffer, sizeof(outputBuffer)) != 0) {
    hidHelper->Write(outputBuffer, sizeof(outputBuffer));
    memcpy(prevOutputBuffer, outputBuffer, sizeof(prevOutputBuffer));
  }
}
