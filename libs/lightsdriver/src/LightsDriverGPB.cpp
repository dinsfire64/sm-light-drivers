#include "LightsDriverGPB.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverGPB::LightsDriverGPB() {
  hidHelper = new HIDHelper(GPB_VID, GPB_PID, GPB_LIGHTING_INTERFACENUM);

  memset(outputBuffer, 0, sizeof(outputBuffer));
};

LightsDriverGPB::~LightsDriverGPB() {};

bool LightsDriverGPB::Connect() {
  if (hidHelper->IsOpen()) {
    // already connected
    return true;
  }

  return hidHelper->Open();
}

void LightsDriverGPB::Disconnect() { hidHelper->Close(); }

void LightsDriverGPB::Set(const LightsState *ls) {
  if (!hidHelper->IsOpen()) {
    // attempt to open if not called already.
    Connect();
  }

  outputBuffer[GPB_LIGHTINDEX_REPORT_ID] = GPB_REPORT_ID;

  // btn1 is p1 red.
  outputBuffer[GPB_LIGHTINDEX_BTN02] = ls->p1_menu;
  outputBuffer[GPB_LIGHTINDEX_BTN03] = ls->p1_menu;
  outputBuffer[GPB_LIGHTINDEX_BTN04] = ls->p1_menu;
  // btn2 is p2 red
  outputBuffer[GPB_LIGHTINDEX_BTN06] = ls->p2_menu;
  outputBuffer[GPB_LIGHTINDEX_BTN07] = ls->p2_menu;
  outputBuffer[GPB_LIGHTINDEX_BTN08] = ls->p2_menu;

  if (memcmp(outputBuffer, prevOutputBuffer, sizeof(outputBuffer)) != 0) {
    hidHelper->Write(outputBuffer, sizeof(outputBuffer));
    memcpy(prevOutputBuffer, outputBuffer, sizeof(prevOutputBuffer));
  }
}
