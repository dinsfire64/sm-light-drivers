#ifndef LightsDriver_H
#define LightsDriver_H

#include "IniHelper.h"

#include <vector>

struct LightsState
{
    bool marquee_up_left;
    bool marquee_up_right;
    bool marquee_lr_left;
    bool marquee_lr_right;
    bool bass;

    bool p1_menu;
    bool p2_menu;

    bool p1_up;
    bool p1_down;
    bool p1_left;
    bool p1_right;

    bool p2_up;
    bool p2_down;
    bool p2_left;
    bool p2_right;

    void SetAll(bool val)
    {
        marquee_up_left = val;
        marquee_up_right = val;
        marquee_lr_left = val;
        marquee_lr_right = val;
        bass = val;

        p1_menu = val;
        p2_menu = val;

        p1_up = val;
        p1_down = val;
        p1_left = val;
        p1_right = val;

        p2_up = val;
        p2_down = val;
        p2_left = val;
        p2_right = val;
    }
    
    void AllOn()
    {
        SetAll(true);
    }
    
    void AllOff()
    {
        SetAll(false);
    }
};

class LightsDriver
{
public:
    LightsDriver() {};
    virtual ~LightsDriver() {};

    virtual bool Connect() = 0;
    virtual void Disconnect() {}

    virtual void Set(const LightsState *ls) = 0;
};

// Macro to register a driver at static initialization time
#define REGISTER_LIGHTS_DRIVER(classname)                                               \
    namespace                                                                           \
    {                                                                                   \
        LightsDriver *Create_##classname() { return new classname(); }                  \
        struct classname##_AutoRegister                                                 \
        {                                                                               \
            classname##_AutoRegister() { LightsManager::Register(Create_##classname); } \
        };                                                                              \
        static classname##_AutoRegister s_##classname##_AutoRegister;                   \
    }

#endif