#include "LightsManager.h"

#include "LightsDriverPrintf.h"
#include "LightsDriverSnek.h"
#include "LightsDriverSextet.h"
#include "LightsDriverSMX.h"
#include "LightsDriverMinimaid.h"
#include "LightsDriverUnilight.h"
#include "LightsDriverStac.h"
#include "LightsDriverStac2.h"
#include "LightsDriverPacDrive.h"
#include "LightsDriverFusionGamepad.h"

#include <iostream>

// for debugging/testing only.
// REGISTER_LIGHTS_DRIVER(LightsDriverPrintf)

REGISTER_LIGHTS_DRIVER(LightsDriverSnek)
REGISTER_LIGHTS_DRIVER(LightsDriverSextet)
REGISTER_LIGHTS_DRIVER(LightsDriverSMX)
REGISTER_LIGHTS_DRIVER(LightsDriverMinimaid)
REGISTER_LIGHTS_DRIVER(LightsDriverUnilight)
REGISTER_LIGHTS_DRIVER(LightsDriverStac)
REGISTER_LIGHTS_DRIVER(LightsDriverStac2)
REGISTER_LIGHTS_DRIVER(LightsDriverPacDrive)
REGISTER_LIGHTS_DRIVER(LightsDriverFusionGamepad)

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
    // default all lights to off.
    m_latestState.SetAll(false);

    // load the values
    // this santities them, then flush to disk.
    IniHelper::Load();
    IniHelper::Save();

    // Instantiate all registered drivers
    for (auto fn : GetRegistry())
    {
        m_allDrivers.emplace_back(fn());
    }
}

LightsManager::~LightsManager()
{
    // force all off on exit.
    ForceAllTo(true);

    // Stop worker thread
    m_running = false;
    m_cv.notify_all();
    if (m_worker.joinable())
        m_worker.join();

    // Disconnect all drivers
    for (auto *driver : m_connectedDrivers)
    {
        if (driver)
        {
            driver->Disconnect();
        }
    }
}

void LightsManager::ForceAllTo(bool val)
{
    m_latestState.SetAll(val);
    m_stateDirty = true;
    m_cv.notify_one();
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

    // Start background thread
    m_running = true;
    m_worker = std::thread(&LightsManager::WorkerThread, this);

    // turn off all lights on first connection.
    ForceAllTo(false);
}

bool LightsManager::IsConnected()
{
    return connected;
}

void LightsManager::SetAll(const LightsState *ls)
{
    if (!connected)
        return;

    std::unique_lock<std::mutex> lock(m_mutex);
    m_latestState = *ls; // overwrite previous state
    m_stateDirty = true;
    m_cv.notify_one(); // wake up worker
}

void LightsManager::WorkerThread()
{
    while (m_running)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_cv.wait_for(lock, std::chrono::milliseconds(LIGHTSMAN_TIMEOUT_THREAD_MS),
                           [&]
                           { return m_stateDirty || !m_running; }))
        {
            // timeout, no new state
            continue;
        }

        if (!m_running)
            break;

        LightsState ls = m_latestState;
        m_stateDirty = false; // reset dirty flag
        lock.unlock();

        // Send to all drivers
        for (auto *driver : m_connectedDrivers)
        {
            driver->Set(&ls);
        }
    }
}