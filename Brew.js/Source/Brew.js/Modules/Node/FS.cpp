#include <Brew.js/Modules/Node/FS.hpp>

namespace Brew::BuiltIn::FS
{
    API::Function readFile(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        string data;
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string path = handler.GetString(0);
            ifstream ifs(path);
            bool ok = true;
            if(ifs.good())
            {
                stringstream strm;
                strm << ifs.rdbuf();
                data = strm.str();
            }
            else ok = false;
            ifs.close();
            API::Callback cb = handler.GetCallback(1);
            if(ok) cb.AddNull();
            else cb.AddString("File \'" + path + "\' was not found");
            cb.AddString(data);
            cb.Call();
        }
        handler.ReturnString(data);
        return API::Return::Void;
    }

    API::Function writeFile(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            string path = handler.GetString(0);
            string data = handler.GetString(1);
            ofstream ofs(path);
            if(ofs.good()) ofs << data;
            // else API::ThrowError(Context, API::Error::CommonError, "File \'" + path + "\' was not found");
            ofs.close();
        }
        return API::Return::Void;
    }

    API::Function exists(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string path = handler.GetString(0);
            ifstream ifs(path);
            bool ex = ifs.good();
            ifs.close();
            handler.ReturnBoolean(ex);
        }
        return API::Return::Variable;
    }

    API::Function mkdir(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string dir = handler.GetString(0);
            ::mkdir(dir.c_str(), 777);
        }
        return API::Return::Void;
    }

    API::Function unlink(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string file = handler.GetString(0);
            remove(file.c_str());
        }
        return API::Return::Void;
    }

    API::Function rmdir(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string dir = handler.GetString(0);
            ::rmdir(dir.c_str());
        }
        return API::Return::Void;
    }

    API::Function readdir(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            string path = handler.GetString(0);
            struct dirent *de;
            DIR *dir = opendir(path.c_str());
            if(dir)
            {
                API::Array arr(Context);
                handler.ReturnArray(arr);
                while(true)
                {
                    de = readdir(dir);
                    if(de == NULL) break;
                    arr.AddString(string(de->d_name));
                }
            }
            closedir(dir);
        }
        return API::Return::Variable;
    }

    API::Function rename(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            string file = handler.GetString(0);
            string newfile = handler.GetString(1);
            ::rename(file.c_str(), newfile.c_str());
        }
        return API::Return::Void;
    }

    API::Function Stats_isDirectory(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isdir = (smod & S_IFDIR);
        handler.ReturnBoolean(isdir);
        return API::Return::Variable;
    }

    API::Function Stats_isFile(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isfile = (smod & S_IFREG);
        handler.ReturnBoolean(isfile);
        return API::Return::Variable;
    }

    API::Function Stats_isFIFO(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isffo = (smod & S_IFIFO);
        handler.ReturnBoolean(isffo);
        return API::Return::Variable;
    }

    API::Function Stats_isBlockDevice(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool isblk = (smod & S_IFBLK);
        handler.ReturnBoolean(isblk);
        return API::Return::Variable;
    }

    API::Function Stats_isCharacterDevice(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool ischr = (smod & S_IFCHR);
        handler.ReturnBoolean(ischr);
        return API::Return::Variable;
    }

    API::Function Stats_isSocket(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool issck = (smod & S_IFSOCK);
        handler.ReturnBoolean(issck);
        return API::Return::Variable;
    }

    API::Function Stats_isSymbolicLink(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        mode_t smod = handler.GetPropertyUInt("mode");
        bool islnk = (smod & S_IFLNK);
        handler.ReturnBoolean(islnk);
        return API::Return::Variable;
    }

    API::Class Stats()
    {
        API::Class cStats("Stats", [](API::NativeContext Context) -> API::Function
        {
            API::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                string path = handler.GetString(0);
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
                // else API::ThrowError(Context, API::Error::CommonError, "No such file or directory: \'" + path + "\'");
            }
            return API::Return::Void;
        });

        cStats.AddFunction("isDirectory", Stats_isDirectory);
        cStats.AddFunction("isFile", Stats_isFile);
        cStats.AddFunction("isFIFO", Stats_isFIFO);
        cStats.AddFunction("isBlockDevice", Stats_isBlockDevice);
        cStats.AddFunction("isCharacterDevice", Stats_isCharacterDevice);
        cStats.AddFunction("isSocket", Stats_isSocket);
        cStats.AddFunction("isSymbolicLink", Stats_isSymbolicLink);
        return cStats;
    }

    API::Module Initialize()
    {
        API::Module fs("fs");
        API::Class cStats = Stats();
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