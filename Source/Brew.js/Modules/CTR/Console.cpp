#include "Console.hpp"

Brew::API::Module console("console");

bool cinit = false;
PrintConsole topc;
PrintConsole subc;

Brew::API::Function Brew::BuiltIn::Console::init(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(!cinit)
    {
        gfxInitDefault();
        consoleInit(GFX_TOP, &topc);
        consoleInit(GFX_BOTTOM, &subc);
        u32 scr = 0;
        if(handler.checkArgc(1)) scr = handler.getUInt(0);
        if(scr == 0) consoleSelect(&topc);
        else consoleSelect(&subc);
        cinit = true;
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::print(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1) && cinit)
    {
        u32 arg = handler.getArgc();
        for(int i = 0; i < arg; i++)
        {
            if(handler.checkArgType(i, Brew::API::Type::String))
            {
                string arg = handler.getString(i);
                cout << arg;
            }
            else if(handler.checkArgType(i, Brew::API::Type::Number))
            {
                double arg = handler.getDouble(i);
                stringstream strm;
                strm << arg;
                cout << strm.str();
            }
            else if(handler.checkArgType(i, Brew::API::Type::Boolean))
            {
                bool arg = handler.getBoolean(i);
                string sbool = (arg ? "true" : "false");
                cout << sbool;
            }
            else if(handler.checkArgType(i, Brew::API::Type::Undefined)) cout << "undefined";
            else if(handler.checkArgType(i, Brew::API::Type::Null)) cout << "null";
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::trace(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1) && cinit)
    {
        u32 arg = handler.getArgc();
        cout << "Trace: ";
        for(int i = 0; i < arg; i++)
        {
            if(handler.checkArgType(i, Brew::API::Type::String))
            {
                string arg = handler.getString(i);
                cout << arg;
            }
            else if(handler.checkArgType(i, Brew::API::Type::Null))
            {
                double arg = handler.getDouble(i);
                stringstream strm;
                strm << arg;
                cout << strm.str();
            }
            else if(handler.checkArgType(i, Brew::API::Type::Boolean))
            {
                bool arg = handler.getBoolean(i);
                string sbool = (arg ? "true" : "false");
                cout << sbool;
            }
        }
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::clear(Brew::API::NativeContext Context)
{
    if(cinit) cout << "\x1b[2J";
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::select(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        u32 scr = handler.getUInt(0);
        if(scr == 0) consoleSelect(&topc);
        else consoleSelect(&subc);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::refresh(Brew::API::NativeContext Context)
{
    if(cinit)
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::Console::exit(Brew::API::NativeContext Context)
{
    if(cinit)
    {
        gfxExit();
        cinit = false;
    }
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::Console::initModule()
{
    console.pushUInt("Top", 0);
    console.pushUInt("Bottom", 1);
    console.pushFunction("init", init);
    console.pushFunction("log", print);
    console.pushFunction("warn", print);
    console.pushFunction("error", print);
    console.pushFunction("info", print);
    console.pushFunction("debug", print);
    console.pushFunction("trace", trace);
    console.pushFunction("clear", clear);
    console.pushFunction("select", select);
    console.pushFunction("refresh", refresh);
    console.pushFunction("exit", Brew::BuiltIn::Console::exit);
    return console;
}