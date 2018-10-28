#include <Brew.js/Modules/NX/Console.hpp>

namespace Brew::BuiltIn::Console
{
    bool cinit = false;

    API::Function init(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(!cinit)
        {
            consoleInit(NULL);
            cinit = true;
        }
        return API::Return::Void;
    }

    API::Function print(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1) && cinit)
        {
            u32 arg = handler.GetArgc();
            for(u32 i = 0; i < arg; i++)
            {
                if(handler.CheckArgumentType(i, API::Type::String))
                {
                    string arg = handler.GetString(i);
                    cout << arg;
                }
                else if(handler.CheckArgumentType(i, API::Type::Number))
                {
                    double arg = handler.GetDouble(i);
                    stringstream strm;
                    strm << arg;
                    cout << strm.str();
                }
                else if(handler.CheckArgumentType(i, API::Type::Boolean))
                {
                    bool arg = handler.GetBoolean(i);
                    string sbool = (arg ? "true" : "false");
                    cout << sbool;
                }
                else if(handler.CheckArgumentType(i, API::Type::ObjectArray))
                {
                    duk_json_encode(Context, i);
                    cout << duk_to_string(Context, -1);
                }
                else if(handler.CheckArgumentType(i, API::Type::Undefined)) cout << "undefined";
                else if(handler.CheckArgumentType(i, API::Type::Null)) cout << "null";
            }
        }
        return API::Return::Void;
    }

    API::Function trace(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1) && cinit)
        {
            u32 arg = handler.GetArgc();
            cout << "Trace: ";
            for(u32 i = 0; i < arg; i++)
            {
                if(handler.CheckArgumentType(i, API::Type::String))
                {
                    string arg = handler.GetString(i);
                    cout << arg;
                }
                else if(handler.CheckArgumentType(i, API::Type::Number))
                {
                    double arg = handler.GetDouble(i);
                    stringstream strm;
                    strm << arg;
                    cout << strm.str();
                }
                else if(handler.CheckArgumentType(i, API::Type::Boolean))
                {
                    bool arg = handler.GetBoolean(i);
                    string sbool = (arg ? "true" : "false");
                    cout << sbool;
                }
                else if(handler.CheckArgumentType(i, API::Type::ObjectArray))
                {
                    duk_json_encode(Context, i);
                    cout << duk_to_string(Context, -1);
                }
                else if(handler.CheckArgumentType(i, API::Type::Undefined)) cout << "undefined";
                else if(handler.CheckArgumentType(i, API::Type::Null)) cout << "null";
            }
        }
        return API::Return::Void;
    }

    API::Function clear(API::NativeContext Context)
    {
        if(cinit) cout << "\x1b[2J";
        return API::Return::Void;
    }

    API::Function refresh(API::NativeContext Context)
    {
        if(cinit) consoleUpdate(NULL);
        return API::Return::Void;
    }

    API::Function exit(API::NativeContext Context)
    {
        if(cinit)
        {
            consoleExit(NULL);
            cinit = false;
        }
        return API::Return::Void;
    }

    API::Module Initialize()
    {
        API::Module console("console");
        console.PushFunction("init", init);
        console.PushFunction("log", print);
        console.PushFunction("warn", print);
        console.PushFunction("error", print);
        console.PushFunction("info", print);
        console.PushFunction("debug", print);
        console.PushFunction("trace", trace);
        console.PushFunction("clear", clear);
        console.PushFunction("refresh", refresh);
        console.PushFunction("exit", exit);
        return console;
    }
}