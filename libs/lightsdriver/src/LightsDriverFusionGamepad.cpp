#include "LightsDriverFusionGamepad.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverFusionGamepad::LightsDriverFusionGamepad()
{
    hidHelper = new HIDHelper(FUSION_VID, FUSION_PID, FUSION_INTERFACENUM);

    memset(outputBuffer, 0, sizeof(outputBuffer));
};

LightsDriverFusionGamepad::~LightsDriverFusionGamepad() {
};

bool LightsDriverFusionGamepad::Connect()
{
    if (hidHelper->IsOpen())
    {
        // already connected
        return true;
    }

    return hidHelper->Open();
}

void LightsDriverFusionGamepad::Disconnect()
{
    hidHelper->Close();
}

void LightsDriverFusionGamepad::Set(const LightsState *ls)
{
    if (!hidHelper->IsOpen())
    {
        // attempt to open if not called already.
        Connect();
    }

    outputBuffer[FUSION_REPORT_ID] = LIGHTING_REPORTID;

    outputBuffer[FUSION_MAR_LR] = ls->marquee_lr_right ? 0xFF : 0x00;
    outputBuffer[FUSION_MAR_UR] = ls->marquee_up_right ? 0xFF : 0x00;
    outputBuffer[FUSION_MAR_LL] = ls->marquee_lr_left ? 0xFF : 0x00;
    outputBuffer[FUSION_MAR_UL] = ls->marquee_up_left ? 0xFF : 0x00;

    outputBuffer[FUSION_NEON] = ls->bass ? 0xFF : 0x00;

    // fusion does not have player button lights...

    outputBuffer[FUSION_P1_UL] = ls->p1_up ? 0xFF : 0x00;
    outputBuffer[FUSION_P1_UR] = ls->p1_down ? 0xFF : 0x00;
    outputBuffer[FUSION_P1_CN] = ls->p1_left ? 0xFF : 0x00;
    outputBuffer[FUSION_P1_LL] = ls->p1_right ? 0xFF : 0x00;

    outputBuffer[FUSION_P2_UL] = ls->p2_up ? 0xFF : 0x00;
    outputBuffer[FUSION_P2_UR] = ls->p2_down ? 0xFF : 0x00;
    outputBuffer[FUSION_P2_CN] = ls->p2_left ? 0xFF : 0x00;
    outputBuffer[FUSION_P2_LL] = ls->p2_right ? 0xFF : 0x00;

    // tying bass to led for debugging is fun!
    outputBuffer[FUSION_LED] = ls->bass ? 0xFF : 0x00;

    if (memcmp(outputBuffer, prevOutputBuffer, sizeof(outputBuffer)) != 0)
    {
        hidHelper->Write(outputBuffer, sizeof(outputBuffer));
        memcpy(prevOutputBuffer, outputBuffer, sizeof(prevOutputBuffer));
    }
}
