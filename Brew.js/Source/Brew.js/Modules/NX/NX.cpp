#include <Brew.js/Modules/NX/NX.hpp>

namespace Brew::BuiltIn::NX
{
    API::Function reboot(API::NativeContext Context)
    {
        bpcInitialize();
        bpcRebootSystem();
        bpcExit();
        return API::Return::Void;
    }

    API::Function shutDown(API::NativeContext Context)
    {
        bpcInitialize();
        bpcShutdownSystem();
        bpcExit();
        return API::Return::Void;
    }

    API::Function getTitles(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(R_SUCCEEDED(ncmInitialize()))
        {
            API::Array arr(Context);
            handler.ReturnArray(arr);
            NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
            NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(sizeof(NcmApplicationContentMetaKey) * 32);
            u32 total = 0;
            u32 written = 0;
            ncmOpenContentMetaDatabase(FsStorageId_GameCard, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if(total > 0) for(u32 i = 0; i < total; i++) arr.AddUInt(metas[i].metaRecord.titleId);
            ncmOpenContentMetaDatabase(FsStorageId_NandUser, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if(total > 0) for(u32 i = 0; i < total; i++) arr.AddUInt(metas[i].metaRecord.titleId);
            ncmOpenContentMetaDatabase(FsStorageId_SdCard, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if(total > 0) for(u32 i = 0; i < total; i++) arr.AddUInt(metas[i].metaRecord.titleId);
        }
        else API::ThrowError(Context, API::Error::CommonError, "Could not initialize \'ncm\' service");
        ncmExit();
        return API::Return::Variable;
    }

    API::Function getGameCartTitle(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(R_SUCCEEDED(ncmInitialize()))
        {
            NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
            NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(sizeof(NcmApplicationContentMetaKey) * 32);
            u32 total = 0;
            u32 written = 0;
            ncmOpenContentMetaDatabase(FsStorageId_GameCard, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if((total > 0) && (written > 0)) handler.ReturnUInt(metas[0].metaRecord.titleId);
            else handler.ReturnUInt(0);
        }
        else API::ThrowError(Context, API::Error::CommonError, "Could not initialize \'ncm\' service");
        ncmExit();
        return API::Return::Variable;
    }

    API::Function getNANDTitles(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(R_SUCCEEDED(ncmInitialize()))
        {
            API::Array arr(Context);
            handler.ReturnArray(arr);
            NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
            NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(sizeof(NcmApplicationContentMetaKey) * 32);
            u32 total = 0;
            u32 written = 0;
            ncmOpenContentMetaDatabase(FsStorageId_NandUser, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if(total > 0) for(u32 i = 0; i < total; i++) arr.AddUInt(metas[i].metaRecord.titleId);
        }
        else API::ThrowError(Context, API::Error::CommonError, "Could not initialize \'ncm\' service");
        ncmExit();
        return API::Return::Variable;
    }

    API::Function getSDTitles(API::NativeContext Context)
    {
        API::FunctionHandler handler(Context);
        if(R_SUCCEEDED(ncmInitialize()))
        {
            API::Array arr(Context);
            handler.ReturnArray(arr);
            NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
            NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(sizeof(NcmApplicationContentMetaKey) * 32);
            u32 total = 0;
            u32 written = 0;
            ncmOpenContentMetaDatabase(FsStorageId_SdCard, metadb);
            ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, (sizeof(NcmApplicationContentMetaKey) * 32), &written, &total);
            if(total > 0) for(u32 i = 0; i < total; i++) arr.AddUInt(metas[i].metaRecord.titleId);
        }
        else API::ThrowError(Context, API::Error::CommonError, "Could not initialize \'ncm\' service");
        ncmExit();
        return API::Return::Variable;
    }

    API::Module Initialize()
    {
        API::Module nx("nx");
        nx.PushFunction("reboot", reboot);
        nx.PushFunction("shutDown", shutDown);
        nx.PushFunction("getTitles", getTitles);
        nx.PushFunction("getGameCartTitle", getGameCartTitle);
        nx.PushFunction("getNANDTitles", getNANDTitles);
        nx.PushFunction("getSDTitles", getSDTitles);
        return nx;
    }
}