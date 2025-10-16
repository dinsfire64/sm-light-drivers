#include "LightsDriverSnek.h"

#include "LightsDriver.h"

#include <hidapi.h>

static hid_device *handle = nullptr;

LightsDriverSnek::LightsDriverSnek()
{
    hid_init();

    memset(outputBuffer, 0, sizeof(outputBuffer));
};

LightsDriverSnek::~LightsDriverSnek() {
    // hid_close()?
};

bool LightsDriverSnek::Connect()
{
    if (handle != nullptr)
    {
        // already connected
        return true;
    }

    bool found = false;
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(SNEK_VID, 0);
    cur_dev = devs;

    if (devs && cur_dev)
    {
        // Look for the desired devices by iterating connected ones
        while (cur_dev)
        {
            if (cur_dev->vendor_id == SNEK_VID && cur_dev->product_id == SNEK_PID)
            {
                if (cur_dev->interface_number == SNEK_LIGHTING_INTERFACENUM)
                {
                    found = true;
                    break;
                }
            }

            if (found)
                break;

            cur_dev = cur_dev->next;
        }
    }

    if (found)
    {
        handle = hid_open_path(cur_dev->path);
    }

    return handle != nullptr;
}

void LightsDriverSnek::Disconnect()
{
}

void LightsDriverSnek::Set(const LightsState *ls)
{
    if (handle == nullptr)
    {
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

    if (handle != nullptr &&
        memcmp(outputBuffer, prevOutputBuffer, sizeof(outputBuffer)) != 0)
    {
        hid_write(handle, outputBuffer, sizeof(outputBuffer));
        memcpy(prevOutputBuffer, outputBuffer, sizeof(prevOutputBuffer));
    }
}
