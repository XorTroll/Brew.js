#include <bjs/node/fs.hpp>

namespace bjs::node::fs
{
    js::Module CreateModule()
    {
        js::Module fs("fs");
        js::Class cStats = Stats::CreateClass();
        fs.PushClass(cStats);
        fs.PushFunction("readFile", readFile);
        fs.PushFunction("writeFile", writeFile);
        fs.PushFunction("exists", exists);
        fs.PushFunction("mkdir", mkdir);
        fs.PushFunction("unlink", unlink);
        fs.PushFunction("rmdir", rmdir);
        fs.PushFunction("readdir", readdir);
        fs.PushFunction("rename", rename);
        return fs;
    }
}