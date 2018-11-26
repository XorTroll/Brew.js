#include <bjs/node/console.hpp>

namespace bjs::node::console
{
    js::Module CreateModule()
    {
        js::Module console("console");
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