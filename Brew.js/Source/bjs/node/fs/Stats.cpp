#include <bjs/node/fs/Stats.hpp>
#include <sys/stat.h>

namespace bjs::node::fs
{
    namespace Stats
    {
        js::Function isDirectory(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool isdir = (smod & S_IFDIR);
            handler.ReturnBoolean(isdir);
            return js::Return::Variable;
        }

        js::Function isFile(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool isfile = (smod & S_IFREG);
            handler.ReturnBoolean(isfile);
            return js::Return::Variable;
        }

        js::Function isFIFO(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool isffo = (smod & S_IFIFO);
            handler.ReturnBoolean(isffo);
            return js::Return::Variable;
        }

        js::Function isBlockDevice(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool isblk = (smod & S_IFBLK);
            handler.ReturnBoolean(isblk);
            return js::Return::Variable;
        }

        js::Function isCharacterDevice(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool ischr = (smod & S_IFCHR);
            handler.ReturnBoolean(ischr);
            return js::Return::Variable;
        }

        js::Function isSocket(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool issck = (smod & S_IFSOCK);
            handler.ReturnBoolean(issck);
            return js::Return::Variable;
        }

        js::Function isSymbolicLink(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            mode_t smod = handler.GetPropertyUInt("mode");
            bool islnk = (smod & S_IFLNK);
            handler.ReturnBoolean(islnk);
            return js::Return::Variable;
        }
        
        js::Class CreateClass()
        {
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
            cStats.AddFunction("isDirectory", isDirectory);
            cStats.AddFunction("isFile", isFile);
            cStats.AddFunction("isFIFO", isFIFO);
            cStats.AddFunction("isBlockDevice", isBlockDevice);
            cStats.AddFunction("isCharacterDevice", isCharacterDevice);
            cStats.AddFunction("isSocket", isSocket);
            cStats.AddFunction("isSymbolicLink", isSymbolicLink);
            return cStats;
        }
    }
}