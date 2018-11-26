#include <Net.hpp>

static u16 BrewPort = 42760;
static string Spaces = "   ";
static bjs::js::NativeContext ctx = bjs::js::CreateContext();
bjs::Environment env(ctx, true);

int main()
{
    consoleInit(NULL);
    bjs::link::Initialize(BrewPort);
    cout << endl << endl << Spaces << Spaces << "Brew.js link - Brew.js interpreter via PC input" << endl << endl << endl;
    cout << Spaces << "Waiting for a PC client..." << endl << Spaces << "Open Brew.js link on Windows (Link.exe) and set this console's IP to start." << endl;
    consoleUpdate(NULL);
    bjs::link::WaitForConnection();
    int times = 0;
    cout << endl << Spaces << "Connected to a PC client!" << endl << Spaces << "Send JavaScript sources from the PC client to evaluate them here." << endl << endl;
    while(appletMainLoop() && bjs::link::IsOK())
    {
        if(times == 0) consoleUpdate(NULL);
        const char *rec = bjs::link::WaitReceive();
        if(rec != NULL)
        {
            if(times == 0) consoleExit(NULL);
            times++;
            string srec = string(rec);
            bjs::EvaluationResult res = env.EvaluateSourceCode(srec);
            bjs::link::SendResult(res);
            if(!res.IsSuccess())
            {
                consoleExit(NULL);
                consoleInit(NULL);
                cout << endl << endl << Spaces << "Evaluation threw an error. (check the client)" << endl;
                cout << endl << "Press any key to exit this application.";
                while(true)
                {
                    consoleUpdate(NULL);
                    hidScanInput();
                    if(hidKeysDown(CONTROLLER_P1_AUTO))
                    {
                        bjs::link::Finalize();
                        consoleExit(NULL);
                        return 0;
                    }
                }
            }
        }
    }
    cout << "The client was disconnected. Press A to this application.";
    while(appletMainLoop())
    {
        consoleUpdate(NULL);
        hidScanInput();
        if(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) break;
    }
    bjs::link::Finalize();
    consoleExit(NULL);
    return 0;
}