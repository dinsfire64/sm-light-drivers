#include "LightsManager.h"

#include "LightsDriverPrintf.h"
#include "LightsDriverSnek.h"
#include "LightsDriverSextet.h"
#include "LightsDriverSMX.h"
#include "LightsDriverMinimaid.h"
#include "LightsDriverUnilight.h"

#include <iostream>

// for debugging/testing only.
// REGISTER_LIGHTS_DRIVER(LightsDriverPrintf)

REGISTER_LIGHTS_DRIVER(LightsDriverSnek)
REGISTER_LIGHTS_DRIVER(LightsDriverSextet)
REGISTER_LIGHTS_DRIVER(LightsDriverSMX)
REGISTER_LIGHTS_DRIVER(LightsDriverMinimaid)
REGISTER_LIGHTS_DRIVER(LightsDriverUnilight)

// --- Static registry definition ---
std::vector<CreateLightsDriverFn> &LightsManager::GetRegistry()
{
    static std::vector<CreateLightsDriverFn> registry;
    return registry;
}

void LightsManager::Register(CreateLightsDriverFn fn)
{
    GetRegistry().push_back(fn);
}

// --- Instance methods ---
LightsManager::LightsManager()
{
    // Instantiate all registered drivers
    for (auto fn : GetRegistry())
    {
        m_allDrivers.emplace_back(fn());
    }
}

LightsManager::~LightsManager()
{
    // Call Disconnect on all connected drivers
    for (auto *driver : m_connectedDrivers)
    {
        if (driver)
        {
            driver->Disconnect();
        }
    }
}

void LightsManager::Initialize()
{
    m_connectedDrivers.clear();

    for (auto &driver : m_allDrivers)
    {
        if (driver->Connect())
        {
            m_connectedDrivers.push_back(driver.get());
        }
    }

    std::cout << "[LightsManager] " << m_connectedDrivers.size() << " drivers connected.\n";
    connected = true;
}

bool LightsManager::IsConnected()
{
    return connected;
}

void LightsManager::SetAll(const LightsState *ls)
{
    for (auto *driver : m_connectedDrivers)
    {
        driver->Set(ls);
    }
}
