#include "socket.h"

#include <iostream>
#include <map>

#include <LightsManager.h>

using namespace std;

#define DEBUG false

LightsManager *lm = new LightsManager();
LightsState light_state;

BOOL WINAPI ConsoleHandler(DWORD signal)
{
    switch (signal)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:

        if (lm != nullptr)
        {
            lm->Shutdown();
        }

        // lets the default handler exit
        return FALSE;
    default:
        // other signals, default behavior
        return FALSE;
    }
}

const map<string, bool LightsState::*> light_map = {
    {"body left high", &LightsState::marquee_up_left},
    {"body right high", &LightsState::marquee_up_right},
    {"body left low", &LightsState::marquee_lr_left},
    {"body right low", &LightsState::marquee_lr_right},

    {"speaker", &LightsState::bass},

    {"lamp0", &LightsState::p1_menu},
    {"lamp1", &LightsState::p2_menu},

    {"foot 1p up", &LightsState::p1_up},
    {"foot 1p down", &LightsState::p1_down},
    {"foot 1p left", &LightsState::p1_left},
    {"foot 1p right", &LightsState::p1_right},

    {"foot 2p up", &LightsState::p2_up},
    {"foot 2p down", &LightsState::p2_down},
    {"foot 2p left", &LightsState::p2_left},
    {"foot 2p right", &LightsState::p2_right},
};

// Helper to set a field dynamically
bool setLightField(LightsState &state, const string &name, bool value)
{
    auto it = light_map.find(name);
    if (it != light_map.end())
    {
        state.*(it->second) = value;
        return true;
    }
    return false; // field not found
}

void interp_mame_light(string input)
{
    size_t pos = 0;
    string token;
    string delimiter = " = ";

    while ((pos = input.find(delimiter)) != string::npos)
    {
        token = input.substr(0, pos);
        input.erase(0, pos + delimiter.length());
    }

    // ignore all other strings that aren't a light update.
    // AKA the init connection string.
    if (input.length() == 1)
    {
#if DEBUG
        printf("%s: %s\r\n", token.c_str(), stoi(input) ? "on" : "off");
#endif

        if (light_map.count(token) == 1)
        {
            setLightField(light_state, token, stoi(input));
        }
    }
}

int main()
{
    // set a catch for the ctrl+c/exits to cleanly disconnect.
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    printf("Starting mame_feeder %s\r\n", _WIN32 ? "32bit" : "64bit");

    bool connected = false;

    Socket s = Socket();
    lm->Initialize();

    // if (connected)
    if (lm->IsConnected())
    {
        printf("Connected to LightingManager.\r\n\r\n");

        // send lighting state forever
        while (1)
        {
            // turn off lights between connections.
            light_state.AllOff();
            lm->SetAll(&light_state);

            printf("Connecting to MAME at %s.", IniHelper::Mame.IPAddress.c_str());

            // connect to mame
            while (!s.Init(IniHelper::Mame.IPAddress.c_str()))
            {
                Sleep(1000);
                printf(".");
            }

            connected = true;
            printf("\r\nConnected to MAME.");

            while (connected)
            {
                string newLight = s.WaitForNewData();

                if (newLight == ERROR_CHAR)
                {
                    printf("\r\n\r\nDisconnected from MAME\r\n");
                    connected = false;
                }
                else if (newLight.length() > 0)
                {
                    // only light deltas are sent to us by MAME, so we can push on every change.
                    interp_mame_light(newLight);
                    lm->SetAll(&light_state);
                }
            }
        }
    }
    else
    {
        printf("Could not connect to LightsManager\r\n.");
        printf("Press Enter to exit.\r\n");
        cin.get();
    }

    lm->Shutdown();

    return 0;
}