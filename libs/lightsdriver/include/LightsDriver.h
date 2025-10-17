#ifndef LightsDriver_H
#define LightsDriver_H

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