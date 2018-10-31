#include <bjs/Modules/Node/FS.hpp>

namespace bjs::modules::FS
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

    js::Function Stats_isDirectory(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isdir = (smod & S_IFDIR);
        handler.ReturnBoolean(isdir);
        return js::Return::Variable;
    }

    js::Function Stats_isFile(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isfile = (smod & S_IFREG);
        handler.ReturnBoolean(isfile);
        return js::Return::Variable;
    }

    js::Function Stats_isFIFO(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isffo = (smod & S_IFIFO);
        handler.ReturnBoolean(isffo);
        return js::Return::Variable;
    }

    js::Function Stats_isBlockDevice(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isblk = (smod & S_IFBLK);
        handler.ReturnBoolean(isblk);
        return js::Return::Variable;
    }

    js::Function Stats_isCharacterDevice(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool ischr = (smod & S_IFCHR);
        handler.ReturnBoolean(ischr);
        return js::Return::Variable;
    }

    js::Function Stats_isSocket(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool issck = (smod & S_IFSOCK);
        handler.ReturnBoolean(issck);
        return js::Return::Variable;
    }

    js::Function Stats_isSymbolicLink(js::NativeContext Context)
    {
        js::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool islnk = (smod & S_IFLNK);
        handler.ReturnBoolean(islnk);
        return js::Return::Variable;
    }

    js::Module Initialize()
    {
        js::Module fs("fs");
        js::Class cStats("Stats", [](js::NativeContext Context) -> js::Function
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                std::string path = handler.GetString(0);
                struct stat st;
                if(stat(path.c_str(), &st) == 0)
                {
                    handler.SetPropertyUInt("dev", st.st_dev);
                    handler.SetPropertyUInt("ino", st.st_ino);
                    handler.SetPropertyUInt("mode", st.st_mode);
                    handler.SetPropertyUInt("nlink", st.st_nlink);
                    handler.SetPropertyUInt("uid", st.st_uid);
                    handler.SetPropertyUInt("rdev", st.st_rdev);
                    handler.SetPropertyUInt("size", st.st_size);
                    handler.SetPropertyUInt("blksize", st.st_blksize);
                    handler.SetPropertyUInt("blocks", st.st_blocks);
                    handler.SetPropertyUInt("atimeMs", st.st_atime);
                    handler.SetPropertyUInt("mtimeMs", st.st_mtime);
                    handler.SetPropertyUInt("ctimeMs", st.st_ctime);
                }
                else js::ThrowError(Context, js::Error::CommonError, "No such file or directory: \'" + path + "\'");
            }
            return js::Return::Void;
        });
        cStats.AddFunction("isDirectory", Stats_isDirectory);
        cStats.AddFunction("isFile", Stats_isFile);
        cStats.AddFunction("isFIFO", Stats_isFIFO);
        cStats.AddFunction("isBlockDevice", Stats_isBlockDevice);
        cStats.AddFunction("isCharacterDevice", Stats_isCharacterDevice);
        cStats.AddFunction("isSocket", Stats_isSocket);
        cStats.AddFunction("isSymbolicLink", Stats_isSymbolicLink);
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