#include "LightsManager.h"

#include "LightsDriverPrintf.h"
#include "LightsDriverSnek.h"
#include "LightsDriverSextet.h"
#include "LightsDriverSMX.h"
#include "LightsDriverMinimaid.h"
#include "LightsDriverUnilight.h"
#include "LightsDriverStac.h"

#include <iostream>

// for debugging/testing only.
// REGISTER_LIGHTS_DRIVER(LightsDriverPrintf)

REGISTER_LIGHTS_DRIVER(LightsDriverSnek)
REGISTER_LIGHTS_DRIVER(LightsDriverSextet)
REGISTER_LIGHTS_DRIVER(LightsDriverSMX)
REGISTER_LIGHTS_DRIVER(LightsDriverMinimaid)
REGISTER_LIGHTS_DRIVER(LightsDriverUnilight)
REGISTER_LIGHTS_DRIVER(LightsDriverStac)

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
    m_queue.push(*ls); // copy LightsState into queue
    m_cv.notify_one();
}

// --- Worker thread implementation ---
void LightsManager::WorkerThread()
{
    while (m_running)
    {
        LightsState ls;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&]
                      { return !m_queue.empty() || !m_running; });

            if (!m_running)
                break;

            ls = m_queue.back(); // get the most recent one
            std::queue<LightsState> empty;
            std::swap(m_queue, empty); // clear the queue (drop old updates)
        }

        // Send the latest lights state to all connected drivers
        for (auto *driver : m_connectedDrivers)
        {
            driver->Set(&ls);
        }
    }
}
