#include "FS.hpp"

Brew::API::Module fs("fs");

Brew::API::Function Brew::BuiltIn::FS::readFile(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    string data;
    if(handler.checkArgc(1))
    {
        string path = handler.getString(0);
        ifstream ifs(path);
        if(ifs.good())
        {
            stringstream strm;
            strm << ifs.rdbuf();
            data = strm.str();
        }
        else handler.throwError(Brew::API::Error::CommonError, "File \'" + path + "\' was not found");
        ifs.close();
    }
    handler.pushString(data);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::writeFile(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(2))
    {
        string path = handler.getString(0);
        string data = handler.getString(1);
        ofstream ofs(path);
        if(ofs.good()) ofs << data;
        else handler.throwError(Brew::API::Error::CommonError, "File \'" + path + "\' was not found");
        ofs.close();
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::FS::exists(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        string path = handler.getString(0);
        ifstream ifs(path);
        bool ex = ifs.good();
        ifs.close();
        handler.pushBoolean(ex);
    }
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::mkdir(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        string dir = handler.getString(0);
		::mkdir(dir.c_str(), 777);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::FS::unlink(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        string file = handler.getString(0);
		remove(file.c_str());
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::FS::rmdir(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        string dir = handler.getString(0);
		::rmdir(dir.c_str());
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::FS::readdir(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(1))
    {
        string path = handler.getString(0);
        struct dirent *de;
        DIR *dir = opendir(path.c_str());
        if(dir)
        {
            Brew::API::Array arr(Context);
            handler.pushArray(arr);
            while(true)
            {
                de = readdir(dir);
                if(de == NULL) break;
                arr.addString(string(de->d_name));
            }
        }
    }
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::rename(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(handler.checkArgc(2))
    {
        string file = handler.getString(0);
        string newfile = handler.getString(1);
        ::rename(file.c_str(), newfile.c_str());
    }
    return Brew::API::Return::Void;
}

Brew::API::Function ctor(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    handler.setPropertyString("Test", "no");
    return Brew::API::Return::Void;
}

Brew::API::Function testfunc(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    handler.setPropertyString("Test", "NewValue");
    return Brew::API::Return::Void;
}

Brew::API::Module Brew::BuiltIn::FS::initModule()
{
    Brew::API::Class test("TestClass", ctor);
    test.addString("Name", "Value");
    test.addFunction("testFunc", testfunc);
    fs.pushClass(test);
    fs.pushFunction("readFile", readFile);
    fs.pushFunction("writeFile", writeFile);
    fs.pushFunction("exists", exists);
    fs.pushFunction("mkdir", mkdir);
    fs.pushFunction("unlink", unlink);
    fs.pushFunction("rmdir", rmdir);
    fs.pushFunction("readdir", readdir);
    fs.pushFunction("rename", rename);
    return fs;
}