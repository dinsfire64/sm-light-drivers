#include "LightsDriverMinimaid.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverMinimaid::LightsDriverMinimaid() {
  hidHelper = new HIDHelper(MM_VID, MM_PID, MM_LIGHTS_INTERFACE_NUM);
};

LightsDriverMinimaid::~LightsDriverMinimaid() {};

bool LightsDriverMinimaid::Connect() {
  if (hidHelper->IsOpen()) {
    // already connected
    return true;
  }

  return hidHelper->Open();
}

void LightsDriverMinimaid::Disconnect() { hidHelper->Close(); }

void LightsDriverMinimaid::PushOutputReport() {
  mm_output_report_t newReport;
  memset(newReport.raw, 0, sizeof(newReport));

  // static report id.
  newReport.report_id = MM_REPORT_ID;

  // always ensure the keyboard mode is on.
  newReport.kb_enable = 1;

  // copy the lights in
  newReport.lights.raw = outputLightState.raw;

  // always ensure the pads are turned on by overring these.
  // (this is FL5 on the schematics)
  newReport.lights.p1_pad_en = true;
  newReport.lights.p2_pad_en = true;

  // tie the neons to the blue led for fun and debugging.
  newReport.blueled = newReport.lights.neons ? 0xFF : 0x00;

  hidHelper->Write((unsigned char *)&newReport.raw, MM_OUTPUTREPORT_SIZE);
}

void LightsDriverMinimaid::Set(const LightsState *ls) {
  if (!hidHelper->IsOpen()) {
    // attempt to open if not called already.
    Connect();
  }

  outputLightState.mar_p1_lower = ls->marquee_lr_left;
  outputLightState.mar_p1_upper = ls->marquee_up_left;
  outputLightState.mar_p2_lower = ls->marquee_lr_right;
  outputLightState.mar_p2_upper = ls->marquee_up_right;

  outputLightState.neons = ls->bass;

  outputLightState.p1_menu = ls->p1_menu;
  outputLightState.p2_menu = ls->p2_menu;

  outputLightState.p1_up = ls->p1_up;
  outputLightState.p1_down = ls->p1_down;
  outputLightState.p1_left = ls->p1_left;
  outputLightState.p1_right = ls->p1_right;

  outputLightState.p2_up = ls->p2_up;
  outputLightState.p2_down = ls->p2_down;
  outputLightState.p2_left = ls->p2_left;
  outputLightState.p2_right = ls->p2_right;

  if (outputLightState.raw != prevOutputLightState.raw) {
    PushOutputReport();
    prevOutputLightState.raw = outputLightState.raw;
  }
}
