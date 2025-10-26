#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <iostream>

#include <LightsManager.h>

using namespace std;

#define DEBUG false

LightsManager *lm = new LightsManager();

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

int main()
{
    // set a catch for the ctrl+c/exits to cleanly disconnect.
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    printf("Starting light test %s\r\n", _WIN32 ? "32bit" : "64bit");
    printf("\r\nPress any key to enter manual mode.\r\n");

    LightsState state;
    state.AllOff();

    struct
    {
        const char *name;
        bool *field;
    } lights[] = {
        {"marquee up left", &state.marquee_up_left},
        {"marquee up right", &state.marquee_up_right},
        {"marquee lr left", &state.marquee_lr_left},
        {"marquee lr right", &state.marquee_lr_right},
        {"bass", &state.bass},
        {"p1 menu", &state.p1_menu},
        {"p2 menu", &state.p2_menu},
        {"p1 up", &state.p1_up},
        {"p1 down", &state.p1_down},
        {"p1 left", &state.p1_left},
        {"p1 right", &state.p1_right},
        {"p2 up", &state.p2_up},
        {"p2 down", &state.p2_down},
        {"p2 left", &state.p2_left},
        {"p2 right", &state.p2_right},
    };

    const int numLights = sizeof(lights) / sizeof(lights[0]);
    int current = 0;
    bool manualMode = false;

    lm->Initialize();

    if (lm->IsConnected())
    {
        printf("Connected to LightingManager.\r\n\r\n");

        // send lighting state forever
        while (1)
        {
            if (!manualMode)
            {
                // Auto cycle
                state.AllOff();
                *(lights[current].field) = true;

                lm->SetAll(&state);
                printf("turning on: %s\r\n", lights[current].name);
                fflush(stdout);

                Sleep(1000);

                current = (current + 1) % numLights;

                if (_kbhit())
                {
                    manualMode = true;
                    _getch(); // clear first key
                    printf("\nSwitched to manual mode.\n");
                    printf("Use <-/-> to change lights, 'q' to quit.\n");
                }
            }
            else
            {
                // Manual control
                printf("\r\n[%2d/%2d] %s : %s", current + 1, numLights,
                       lights[current].name,
                       *(lights[current].field) ? "ON " : "OFF");
                fflush(stdout);

                int c = _getch();

                if (c == 'q' || c == 'Q' || c == ' ' || c == '\r' || c == '\n')
                {
                    printf("\nExiting light tester.\n");
                    break;
                }

                // windows sends 0 or 224 for special codes.
                if (c == 0 || c == 224)
                {
                    // it then sends the code we want.
                    // Arrow keys, left/right
                    c = _getch();
                    switch (c)
                    {
                    case 75: // left arrow
                        current = (current - 1 + numLights) % numLights;
                        break;
                    case 77: // right arrow
                        current = (current + 1) % numLights;
                        break;
                    }

                    state.AllOff();
                    *(lights[current].field) = !*(lights[current].field);
                    lm->SetAll(&state);
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