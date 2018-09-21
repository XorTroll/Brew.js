
/*

    Brew.js link

*/

#include "Net.h"

#include <switch.h>
#include <iostream>
#include <string>
using namespace std;

// Using Brew.js API (ovbiously)
#include <Brew.js/Brew.hpp>

static u16 BrewPort = 42760;
static string Spaces = "   ";

int main()
{
    gfxInitDefault();
    consoleInit(NULL);
    Brew::API::NativeContext ctx = Brew::API::createContext();
    Brew::Environment env(ctx);
    Net_init(BrewPort);
    cout << endl << endl << Spaces << Spaces << "Brew.js link - Brew.js interpreter via PC input" << endl << endl << endl;
    cout << Spaces << "Waiting for PC client..." << endl << Spaces << "Open Brew.js link for PC and set this console's IP to start." << endl;
    gfxFlushBuffers();
    gfxSwapBuffers();
    Net_waitForPC();
    int times = 0;
    cout << endl << Spaces << "Connected to PC client!" << endl << Spaces << "Send JavaScript sources from the PC client to evaluate them here." << endl << endl;
    startup:
    while(appletMainLoop() && Net_ok())
    {
        if(times == 0)
        {
            gfxFlushBuffers();
            gfxSwapBuffers();
        }
        const char *rec = Net_waitRecieve();
        if(rec != NULL)
        {
            if(times == 0) gfxExit();
            times++;
            string srec = string(rec);
            Brew::ExecutionResult res = env.evaluateSourceCode(srec);
            if(!res.isOK())
            {
                gfxExit();
                gfxInitDefault();
                consoleInit(NULL);
                cout << endl << endl << Spaces << "Evaluation threw an error:" << endl << Spaces << "- " << res.resultString() << endl;
                cout << endl << "Press A to restart the execution.";
                while(true)
                {
                    gfxFlushBuffers();
                    gfxSwapBuffers();
                    hidScanInput();
                    if(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) goto startup;
                }
            }
        }
    }
    cout << "The PC client was disconnected. Press A to exit Brew.js link.";
    while(appletMainLoop())
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        hidScanInput();
        if(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) break;
    }
    Net_exit();
    gfxExit();
    return 0;
}