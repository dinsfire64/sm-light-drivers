#pragma once
#include "LightsDriver.h"
#include "LightsManager.h"
#include <iostream>
#include <sstream>

class LightsDriverPrintf : public LightsDriver
{
public:
    LightsDriverPrintf() = default;
    ~LightsDriverPrintf() override = default;

    bool Connect() override
    {
        std::cout << "LightsDriverPrintf Connected\n";
        return true;
    }

    void Set(const LightsState *ls) override
    {
        std::cout << ToString(*ls) << "\n";
    }

    void Disconnect() override
    {
        std::cout << "LightsDriverPrintf Disconnected\n";
    }

private:
    // Helper to format LightsState for readable output with 0/1
    std::string ToString(const LightsState &ls) const
    {
        std::ostringstream ss;
        ss << "MAR[UL:" << int(ls.marquee_up_left)
           << " UR:" << int(ls.marquee_up_right)
           << " LL:" << int(ls.marquee_lr_left)
           << " LR:" << int(ls.marquee_lr_right) << "]  "
           << "B:" << int(ls.bass) << "  "
           << "P1[M:" << int(ls.p1_menu)
           << " U:" << int(ls.p1_up)
           << " D:" << int(ls.p1_down)
           << " L:" << int(ls.p1_left)
           << " R:" << int(ls.p1_right) << "]  "
           << "P2[M:" << int(ls.p2_menu)
           << " U:" << int(ls.p2_up)
           << " D:" << int(ls.p2_down)
           << " L:" << int(ls.p2_left)
           << " R:" << int(ls.p2_right) << "]";
        return ss.str();
    }
};

// Automatically register this driver at static initialization time
REGISTER_LIGHTS_DRIVER(LightsDriverPrintf)
