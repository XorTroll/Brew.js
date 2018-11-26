#include <bjs/node/console/ConsolePrint.hpp>
#include <iostream>

namespace bjs::node::console
{
    bool cinit = false;
    #ifdef bjsPlatformLibCTRU
        PrintConsole topc;
        PrintConsole subc;
    #endif

    js::Function init(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(!cinit)
        {
            #ifdef bjsPlatformLibNX
                consoleInit(NULL);
            #elif defined bjsPlatformLibCTRU
                gfxInitDefault();
                consoleInit(GFX_TOP, &topc);
                consoleInit(GFX_BOTTOM, &subc);
                u32 scr = 0;
                if(handler.CheckArgcEqualOrBigger(1)) scr = handler.GetUInt(0);
                if(scr == 0) consoleSelect(&topc);
                else consoleSelect(&subc);
            #endif
            cinit = true;
        }
        return js::Return::Void;
    }

    js::Function print(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1) && cinit)
        {
            u32 arg = handler.GetArgc();
            for(u32 i = 0; i < arg; i++)
            {
                if(handler.CheckArgumentType(i, js::Type::String))
                {
                    std::string arg = handler.GetString(i);
                    std::cout << arg;
                }
                else if(handler.CheckArgumentType(i, js::Type::Number))
                {
                    double arg = handler.GetDouble(i);
                    std::stringstream strm;
                    strm << arg;
                    std::cout << strm.str();
                }
                else if(handler.CheckArgumentType(i, js::Type::Boolean))
                {
                    bool arg = handler.GetBoolean(i);
                    std::string sbool = (arg ? "true" : "false");
                    std::cout << sbool;
                }
                else if(handler.CheckArgumentType(i, js::Type::ObjectArray))
                {
                    duk_json_encode(Context, i);
                    std::cout << duk_to_string(Context, -1);
                }
                else if(handler.CheckArgumentType(i, js::Type::Undefined)) std::cout << "undefined";
                else if(handler.CheckArgumentType(i, js::Type::Null)) std::cout << "null";
            }
        }
        return js::Return::Void;
    }

    js::Function trace(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1) && cinit)
        {
            u32 arg = handler.GetArgc();
            std::cout << "Trace: ";
            for(u32 i = 0; i < arg; i++)
            {
                if(handler.CheckArgumentType(i, js::Type::String))
                {
                    std::string arg = handler.GetString(i);
                    std::cout << arg;
                }
                else if(handler.CheckArgumentType(i, js::Type::Number))
                {
                    double arg = handler.GetDouble(i);
                    std::stringstream strm;
                    strm << arg;
                    std::cout << strm.str();
                }
                else if(handler.CheckArgumentType(i, js::Type::Boolean))
                {
                    bool arg = handler.GetBoolean(i);
                    std::string sbool = (arg ? "true" : "false");
                    std::cout << sbool;
                }
                else if(handler.CheckArgumentType(i, js::Type::ObjectArray))
                {
                    duk_json_encode(Context, i);
                    std::cout << duk_to_string(Context, -1);
                }
                else if(handler.CheckArgumentType(i, js::Type::Undefined)) std::cout << "undefined";
                else if(handler.CheckArgumentType(i, js::Type::Null)) std::cout << "null";
            }
        }
        return js::Return::Void;
    }

    js::Function clear(js::NativeContext Context)
    {
        if(cinit) std::cout << "\x1b[2J";
        return js::Return::Void;
    }

    js::Function refresh(js::NativeContext Context)
    {
        if(cinit)
        {
            #ifdef bjsPlatformLibNX
                consoleUpdate(NULL);
            #elif defined bjsPlatformLibCTRU
                gfxFlushBuffers();
                gfxSwapBuffers();
                gspWaitForVBlank();
            #endif
        }
        return js::Return::Void;
    }

    js::Function exit(js::NativeContext Context)
    {
        if(cinit)
        {
            #ifdef bjsPlatformLibNX
                consoleExit(NULL);
            #elif defined bjsPlatformLibCTRU
                gfxExit();
            #endif
            cinit = false;
        }
        return js::Return::Void;
    }
}