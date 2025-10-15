#ifndef LightsDriver_H
#define LightsDriver_H

#include <vector>

struct LightsState
{
    bool marquee_up_left : 1;
    bool marquee_up_right : 1;
    bool marquee_lr_left : 1;
    bool marquee_lr_right : 1;
    bool bass : 1;

    bool p1_menu : 1;
    bool p2_menu : 1;

    bool p1_up : 1;
    bool p1_down : 1;
    bool p1_left : 1;
    bool p1_right : 1;

    bool p2_up : 1;
    bool p2_down : 1;
    bool p2_left : 1;
    bool p2_right : 1;
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