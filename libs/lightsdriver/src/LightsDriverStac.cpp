#include "LightsDriverStac.h"

#include "LightsDriver.h"
#include <unistd.h>

LightsDriverStac::LightsDriverStac()
{
    hidHelper[0] = new HIDHelper(STAC_VID, STAC_PID_P1, STAC_LIGHTING_INTERFACE);
    hidHelper[1] = new HIDHelper(STAC_VID, STAC_PID_P2, STAC_LIGHTING_INTERFACE);

    memset(outputBuffer[0], 0, sizeof(outputBuffer[0]));
    memset(outputBuffer[1], 0, sizeof(outputBuffer[1]));
};

LightsDriverStac::~LightsDriverStac() {
};

bool LightsDriverStac::Connect()
{
    if (hidHelper[0]->IsOpen() || hidHelper[1]->IsOpen())
    {
        // already connected
        return true;
    }

    bool isP1Connected = hidHelper[0]->Open();
    bool isP2Connected = hidHelper[1]->Open();

    return isP1Connected || isP2Connected;
}

void LightsDriverStac::Disconnect()
{
    for (int i = 0; i < STAC_MAX_NUMBER; i++)
    {
        if (hidHelper[i])
        {
            hidHelper[i]->Close();
        }
    }
}

void LightsDriverStac::Set(const LightsState *ls)
{
    if (!(hidHelper[0]->IsOpen() || hidHelper[1]->IsOpen()))
    {
        // attempt to open if not called already.
        Connect();
    }

    outputBuffer[0][STAC_LIGHTINDEX_REPORT] = STAC_REPORT_ID;
    outputBuffer[0][STAC_LIGHTINDEX_BTN1] = ls->p1_up;
    outputBuffer[0][STAC_LIGHTINDEX_BTN2] = ls->p1_down;
    outputBuffer[0][STAC_LIGHTINDEX_BTN3] = ls->p1_left;
    outputBuffer[0][STAC_LIGHTINDEX_BTN4] = ls->p1_right;

    outputBuffer[1][STAC_LIGHTINDEX_REPORT] = STAC_REPORT_ID;
    outputBuffer[1][STAC_LIGHTINDEX_BTN1] = ls->p2_up;
    outputBuffer[1][STAC_LIGHTINDEX_BTN2] = ls->p2_down;
    outputBuffer[1][STAC_LIGHTINDEX_BTN3] = ls->p2_left;
    outputBuffer[1][STAC_LIGHTINDEX_BTN4] = ls->p2_right;

    for (int i = 0; i < STAC_MAX_NUMBER; i++)
    {
        if (hidHelper[i]->IsOpen())
        {
            if (memcmp(outputBuffer[i], prevOutputBuffer[i], sizeof(outputBuffer[i])) != 0)
            {
                hidHelper[i]->Write(outputBuffer[i], sizeof(outputBuffer[i]));
                memcpy(prevOutputBuffer[i], outputBuffer[i], sizeof(prevOutputBuffer[i]));
            }
        }
    }
}
