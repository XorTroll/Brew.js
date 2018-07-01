// Brew.js includes libnx plus some C and C++ basic libs. we don't need anything else
#include "Brew.js/Brew.js.h"

int main()
{
    // JS Libs located at RomFS
    Brew::Init("romfs:/BrewLibs");
    // Result will be "os.arch()"
    string res = Brew::Evaluate("var os = require('os');os.arch();");
    // Brew.js blocks console output (SDL2), so we must close it to enable console
    Brew::Exit();
    // Now we can test out var in console, we init graphics and console
    gfxInitDefault();
    consoleInit(NULL);
    // and print the result (should be "aarch64")
    cout << "Brew.js:" << endl << res;
    // We enter the main loop
    while(appletMainLoop())
    {
        // Refresh graphic stuff
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
        // Check for keys
        hidScanInput();
        int key = hidKeysDown(CONTROLLER_P1_AUTO);
        // if + is pressed exit main loop --> exit app
        if(key & KEY_PLUS) break;
    }
    // exit graphics
    gfxExit();
    // Exit the code
    return 0;
}