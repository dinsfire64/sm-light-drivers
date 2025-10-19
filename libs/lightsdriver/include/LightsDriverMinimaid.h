#pragma once

#include "LightsDriver.h"
#include "LightsManager.h"
#include "HIDHelper.h"

#include <cstdint>
#include <string.h>

// static information about the device in question.
#define MM_VID 0xbeef
#define MM_PID 0x5730

// input isn't used here, we are using the keyboard on the firmware
// we are only talking to the lighting interface.
#define MM_INPUT_INTERFACE_NUM 0
#define MM_LIGHTS_INTERFACE_NUM 1

#define MM_REPORT_ID 0

#define MM_OUTPUTREPORT_SIZE 9

#pragma pack(push, 1)

typedef union
{
    struct
    {
        // byte 0
        bool b0_unused0 : 1;   // bit 0
        bool b0_unused1 : 1;   // bit 1
        bool p1_menu : 1;      // bit 2
        bool p2_menu : 1;      // bit 3
        bool mar_p2_lower : 1; // bit 4
        bool mar_p2_upper : 1; // bit 5
        bool mar_p1_lower : 1; // bit 6
        bool mar_p1_upper : 1; // bit 7

        // byte 1
        bool p1_up : 1;      // bit 8
        bool p1_down : 1;    // bit 9
        bool p1_left : 1;    // bit 10
        bool p1_right : 1;   // bit 11
        bool p1_pad_en : 1;  // bit 12
        bool b1_unused5 : 1; // bit 13
        bool b1_unused6 : 1; // bit 14
        bool b1_unused7 : 1; // bit 15

        // byte 2
        bool p2_up : 1;      // bit 16
        bool p2_down : 1;    // bit 17
        bool p2_left : 1;    // bit 18
        bool p2_right : 1;   // bit 19
        bool p2_pad_en : 1;  // bit 20
        bool b2_unused5 : 1; // bit 21
        bool b2_unused6 : 1; // bit 22
        bool b2_unused7 : 1; // bit 23

        // byte 3
        bool neons : 1;      // bit 24
        bool b3_unused1 : 1; // bit 25
        bool b3_unused2 : 1; // bit 26
        bool b3_unused3 : 1; // bit 27
        bool b3_unused4 : 1; // bit 28
        bool b3_unused5 : 1; // bit 29
        bool b3_unused6 : 1; // bit 30
        bool b3_unused7 : 1; // bit 31
    };
    uint32_t raw;
} mm_lighting_state_ddr_t;

typedef union
{
    struct
    {
        uint8_t report_id;
        uint8_t extout;
        mm_lighting_state_ddr_t lights;
        uint8_t blueled;
        uint8_t kb_enable;
        uint8_t aux_flags;
    };
    uint8_t raw[MM_OUTPUTREPORT_SIZE];
} mm_output_report_t;

#pragma pack(pop)

class LightsDriverMinimaid : public LightsDriver
{
public:
    LightsDriverMinimaid();
    ~LightsDriverMinimaid() override;

    bool Connect() override;

    void Set(const LightsState *ls) override;

    void Disconnect() override;

private:
    mm_lighting_state_ddr_t outputLightState;
    mm_lighting_state_ddr_t prevOutputLightState;

    HIDHelper *hidHelper;

    void PushOutputReport();
};