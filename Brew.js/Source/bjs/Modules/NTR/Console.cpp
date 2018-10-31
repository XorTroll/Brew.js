#include <bjs/Modules/NTR/Console.hpp>

namespace bjs::modules::Console
{
    bool cinit = false;
    PrintConsole topc;
    PrintConsole subc;

    js::Function init(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(!cinit)
        {
            videoSetMode(MODE_0_2D);
            videoSetModeSub(MODE_0_2D);
            vramSetBankA(VRAM_A_MAIN_BG);
            vramSetBankC(VRAM_C_SUB_BG);
            consoleInit(&topc, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
            consoleInit(&subc, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
            u32 scr = 0;
            if(handler.CheckArgcEqualOrBigger(1)) scr = handler.GetUInt(0);
            if(scr == 0) consoleSelect(&topc);
            else consoleSelect(&subc);
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

    js::Function select(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            u32 scr = handler.GetUInt(0);
            if(scr == 0) consoleSelect(&topc);
            else consoleSelect(&subc);
        }
        return js::Return::Void;
    }

    js::Function refresh(js::NativeContext Context)
    {
        return js::Return::Void;
    }

    js::Module Initialize()
    {
        js::Module console("console");
        console.PushUInt("Top", 0);
        console.PushUInt("Bottom", 1);
        console.PushFunction("init", init);
        console.PushFunction("log", print);
        console.PushFunction("warn", print);
        console.PushFunction("error", print);
        console.PushFunction("info", print);
        console.PushFunction("debug", print);
        console.PushFunction("trace", trace);
        console.PushFunction("clear", clear);
        console.PushFunction("select", select);
        console.PushFunction("refresh", refresh);
        return console;
    }
}