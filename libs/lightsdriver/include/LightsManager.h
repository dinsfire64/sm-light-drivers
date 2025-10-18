#pragma once
#include <vector>
#include <memory>
#include "LightsDriver.h"

// Forward declare a factory type
typedef LightsDriver *(*CreateLightsDriverFn)();

class LightsManager
{
public:
    LightsManager();
    ~LightsManager();

    void Initialize();
    void SetAll(const LightsState *ls);

    bool IsConnected();

private:
    std::vector<std::unique_ptr<LightsDriver>> m_allDrivers;
    std::vector<LightsDriver *> m_connectedDrivers;
    bool connected = false;

public:
    // Registration system
    static void Register(CreateLightsDriverFn fn);
    static std::vector<CreateLightsDriverFn> &GetRegistry();
};
