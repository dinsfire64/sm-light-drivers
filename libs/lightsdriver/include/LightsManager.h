#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

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

    // --- Threading members ---
    std::thread m_worker;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<LightsState> m_queue;
    std::atomic<bool> m_running{false};

    void WorkerThread();

public:
    // Registration system
    static void Register(CreateLightsDriverFn fn);
    static std::vector<CreateLightsDriverFn> &GetRegistry();
};
