#include <bjs/node/fs/FileSystem.hpp>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>

namespace bjs::node::fs
{
    js::Function readFile(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        std::string data;
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string path = handler.GetString(0);
            std::ifstream ifs(path);
            bool ok = true;
            if(ifs.good())
            {
                std::stringstream strm;
                strm << ifs.rdbuf();
                data = strm.str();
            }
            else ok = false;
            ifs.close();
            js::Callback cb = handler.GetCallback(1);
            if(ok) cb.AddNull();
            else cb.AddString("File \'" + path + "\' was not found");
            cb.AddString(data);
            cb.Call();
        }
        handler.ReturnString(data);
        return js::Return::Void;
    }

    js::Function writeFile(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            std::string path = handler.GetString(0);
            std::string data = handler.GetString(1);
            std::ofstream ofs(path);
            if(ofs.good()) ofs << data;
            else js::ThrowError(Context, js::Error::CommonError, "File \'" + path + "\' was not found");
            ofs.close();
        }
        return js::Return::Void;
    }

    js::Function exists(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string path = handler.GetString(0);
            std::ifstream ifs(path);
            bool ex = ifs.good();
            ifs.close();
            handler.ReturnBoolean(ex);
        }
        return js::Return::Variable;
    }

    js::Function mkdir(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string dir = handler.GetString(0);
            ::mkdir(dir.c_str(), 777);
        }
        return js::Return::Void;
    }

    js::Function unlink(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string file = handler.GetString(0);
            ::remove(file.c_str());
        }
        return js::Return::Void;
    }

    js::Function rmdir(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string dir = handler.GetString(0);
            ::rmdir(dir.c_str());
        }
        return js::Return::Void;
    }

    js::Function readdir(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            std::string path = handler.GetString(0);
            struct dirent *de;
            DIR *dir = opendir(path.c_str());
            if(dir)
            {
                js::Array arr(Context);
                handler.ReturnArray(arr);
                while(true)
                {
                    de = readdir(dir);
                    if(de == NULL) break;
                    arr.AddString(std::string(de->d_name));
                }
            }
            closedir(dir);
        }
        return js::Return::Variable;
    }

    js::Function rename(js::NativeContext Context)
    {
        js::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            std::string file = handler.GetString(0);
            std::string newfile = handler.GetString(1);
            ::rename(file.c_str(), newfile.c_str());
        }
        return js::Return::Void;
    }
}