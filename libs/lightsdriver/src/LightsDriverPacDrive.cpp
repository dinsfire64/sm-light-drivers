#include "LightsDriverPacDrive.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverPacDrive::LightsDriverPacDrive()
{
    hidHelper = new HIDHelper(PACDRIVE_VID, PACDRIVE_PID, PACDRIVE_INTERFACE);

    outputLEDs.raw = 0;
};

LightsDriverPacDrive::~LightsDriverPacDrive() {
};

bool LightsDriverPacDrive::Connect()
{
    if (hidHelper->IsOpen())
    {
        // already connected
        return true;
    }

    return hidHelper->Open();
}

void LightsDriverPacDrive::Disconnect()
{
    hidHelper->Close();
}

void LightsDriverPacDrive::PushState()
{
    pacdrive_state_t state;

    state.report_id = PACDRIVE_HIDREPORT_ID;
    state.pad0 = 0;
    state.pad1 = 0;
    state.leds.raw = outputLEDs.raw;

    hidHelper->Write((unsigned char *)&state.raw_state, sizeof(state.raw_state));
}

void LightsDriverPacDrive::Set(const LightsState *ls)
{
    if (!hidHelper->IsOpen())
    {
        // attempt to open if not called already.
        Connect();
    }

    /*
     * OpenITG (LumenAR) Order:
     * http://solid-orange.com/wp-content/uploads/2014/02/ddr_oitg_pacdrive_pins.gif
     * Note: The bit order in OpenITG is byte swapped as PacDrive's vendor library byte
     * swaps the values.
     *
     * This listing mirrors the PHYSICAL pins each of the lights comes out.
     *
     * 01: P1 Left
     * 02: P1 Right
     * 03: P1 Up
     * 04: P1 Down
     * 05: P2 Left
     * 06: P2 Right
     * 07: P2 Up
     * 08: P2 Down
     * 09: Marquee UL
     * 10: Marquee UR
     * 11: Marquee LL
     * 12: Marquee LR
     * 13: P1 Start
     * 14: P2 Start
     * 15: Bass Left
     * 16: Bass Right
     */

    outputLEDs.led01 = ls->p1_left;
    outputLEDs.led02 = ls->p1_right;
    outputLEDs.led03 = ls->p1_up;
    outputLEDs.led04 = ls->p1_down;
    
    outputLEDs.led05 = ls->p2_left;
    outputLEDs.led06 = ls->p2_right;
    outputLEDs.led07 = ls->p2_up;
    outputLEDs.led08 = ls->p2_down;
    
    outputLEDs.led09 = ls->marquee_up_left;
    outputLEDs.led10 = ls->marquee_up_right;
    outputLEDs.led11 = ls->marquee_lr_left;
    outputLEDs.led12 = ls->marquee_lr_right;
    
    outputLEDs.led13 = ls->p1_menu;
    outputLEDs.led14 = ls->p2_menu;
    
    outputLEDs.led15 = ls->bass;
    outputLEDs.led16 = ls->bass;

    if (prevoutputLEDs.raw != outputLEDs.raw)
    {
        PushState();
        prevoutputLEDs.raw = outputLEDs.raw;
    }
}
