#pragma once

#include <windows.h>

#define EXPORT __declspec(dllexport)

/*Minimaid bit identifiers*/
// CABINET_LIGHTS
#define DDR_DOUBLE_PLAYER1_PANEL 2
#define DDR_DOUBLE_PLAYER2_PANEL 3
#define DDR_DOUBLE_MARQUEE_LOWER_RIGHT 4
#define DDR_DOUBLE_MARQUEE_UPPER_RIGHT 5
#define DDR_DOUBLE_MARQUEE_LOWER_LEFT 6
#define DDR_DOUBLE_MARQUEE_UPPER_LEFT 7

// PADX_LIGHTS
#define DDR_DOUBLE_PAD_UP 0
#define DDR_DOUBLE_PAD_DOWN 1
#define DDR_DOUBLE_PAD_LEFT 2
#define DDR_DOUBLE_PAD_RIGHT 3
#define DDR_DOUBLE_PAD_RESET 4

#ifdef __cplusplus
extern "C"
{
#endif

    EXPORT void mm_setDDRPad1Light(int, int);
    EXPORT void mm_setDDRPad2Light(int a, int b);
    EXPORT void mm_setDDRCabinetLight(int a, int b);
    EXPORT void mm_setDDRBassLight(int a, int b);

    EXPORT bool mm_connect_minimaid();
    EXPORT bool mm_setKB(bool val);

    EXPORT void mm_setDDRAllOn();
    EXPORT void mm_setDDRAllOff();

    EXPORT void mm_setBlueLED(unsigned char value);
    EXPORT void mm_setMMOutputReports(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
    EXPORT bool mm_sendDDRMiniMaidUpdate();
    EXPORT void mm_init();
    EXPORT unsigned char mm_turnON(unsigned char set, int bit);
    EXPORT unsigned char mm_turnOFF(unsigned char set, int bit);

#ifdef __cplusplus
}
#endif
