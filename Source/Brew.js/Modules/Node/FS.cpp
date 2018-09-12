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
        else throwError(Context, Brew::API::Error::CommonError, "File \'" + path + "\' was not found");
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
        else throwError(Context, Brew::API::Error::CommonError, "File \'" + path + "\' was not found");
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
        closedir(dir);
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

Brew::API::Function Brew::BuiltIn::FS::Stats_isDirectory(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool isdir = (smod & S_IFDIR);
    handler.pushBoolean(isdir);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isFile(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool isfile = (smod & S_IFREG);
    handler.pushBoolean(isfile);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isFIFO(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool isffo = (smod & S_IFIFO);
    handler.pushBoolean(isffo);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isBlockDevice(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool isblk = (smod & S_IFBLK);
    handler.pushBoolean(isblk);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isCharacterDevice(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool ischr = (smod & S_IFCHR);
    handler.pushBoolean(ischr);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isSocket(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool issck = (smod & S_IFSOCK);
    handler.pushBoolean(issck);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::FS::Stats_isSymbolicLink(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    mode_t smod = handler.getPropertyUInt("mode");
    bool islnk = (smod & S_IFLNK);
    handler.pushBoolean(islnk);
    return Brew::API::Return::Variable;
}

Brew::API::Class Brew::BuiltIn::FS::Stats()
{
    Brew::API::Class cStats("Stats", [](Brew::API::NativeContext Context) -> Brew::API::Function
    {
        Brew::API::ClassHandler handler(Context);
        if(handler.checkArgc(1))
        {
            string path = handler.getString(0);
            struct stat st;
            bool err = false;
            if(stat(path.c_str(), &st) == 0)
            {
                handler.setPropertyUInt("dev", st.st_dev);
                handler.setPropertyUInt("ino", st.st_ino);
                handler.setPropertyUInt("mode", st.st_mode);
                handler.setPropertyUInt("nlink", st.st_nlink);
                handler.setPropertyUInt("uid", st.st_uid);
                handler.setPropertyUInt("rdev", st.st_rdev);
                handler.setPropertyUInt("size", st.st_size);
                handler.setPropertyUInt("blksize", st.st_blksize);
                handler.setPropertyUInt("blocks", st.st_blocks);
                handler.setPropertyUInt("atimeMs", st.st_atime);
                handler.setPropertyUInt("mtimeMs", st.st_mtime);
                handler.setPropertyUInt("ctimeMs", st.st_ctime);
            }
            else throwError(Context, Brew::API::Error::CommonError, "No such file or directory: \'" + path + "\'");
        }
        return Brew::API::Return::Void;
    });

    cStats.addFunction("isDirectory", Stats_isDirectory);
    cStats.addFunction("isFile", Stats_isFile);
    cStats.addFunction("isFIFO", Stats_isFIFO);
    cStats.addFunction("isBlockDevice", Stats_isBlockDevice);
    cStats.addFunction("isCharacterDevice", Stats_isCharacterDevice);
    cStats.addFunction("isSocket", Stats_isSocket);
    cStats.addFunction("isSymbolicLink", Stats_isSymbolicLink);
    return cStats;
}

Brew::API::Module Brew::BuiltIn::FS::initModule()
{
    fs.pushClass(Stats());
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