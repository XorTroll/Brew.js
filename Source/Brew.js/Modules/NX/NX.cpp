#include "NX.hpp"

Brew::API::Module nx("nx");

Brew::API::Function Brew::BuiltIn::NX::reboot(Brew::API::NativeContext Context)
{
    Service bpc;
    smGetService(&bpc, kernelAbove200() ? "bpc" : "bpc:c");
    IpcCommand c;
    ipcInitialize(&c);
    struct Raw
    {
        u64 magic;
        u64 cmd_id;
    } *raw;
    raw = (struct Raw*)ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 1;
    Result rc = serviceIpcDispatch(&bpc);
    serviceClose(&bpc);
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::NX::shutDown(Brew::API::NativeContext Context)
{
    Service bpc;
    smGetService(&bpc, kernelAbove200() ? "bpc" : "bpc:c");
    IpcCommand c;
    ipcInitialize(&c);
    struct Raw
    {
        u64 magic;
        u64 cmd_id;
    } *raw;
    raw = (struct Raw*)ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 0;
    Result rc = serviceIpcDispatch(&bpc);
    serviceClose(&bpc);
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::NX::getTitles(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(R_SUCCEEDED(ncmInitialize()))
    {
        Brew::API::Array arr(Context);
        handler.pushArray(arr);
        NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
        size_t size = sizeof(NcmApplicationContentMetaKey) * 32;
        NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(size);
        u32 total = 0;
        u32 written = 0;
        ncmOpenContentMetaDatabase(FsStorageId_GameCard, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if(total > 0) for(u32 i = 0; i < total; i++) arr.addUInt(metas[i].metaRecord.titleId);
        ncmOpenContentMetaDatabase(FsStorageId_NandUser, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if(total > 0) for(u32 i = 0; i < total; i++) arr.addUInt(metas[i].metaRecord.titleId);
        ncmOpenContentMetaDatabase(FsStorageId_SdCard, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if(total > 0) for(u32 i = 0; i < total; i++) arr.addUInt(metas[i].metaRecord.titleId);
    }
    else throwError(Context, Brew::API::Error::CommonError, "Could not initialize \'ncm\' service");
    ncmExit();
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::NX::getGameCartTitle(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(R_SUCCEEDED(ncmInitialize()))
    {
        NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
        size_t size = sizeof(NcmApplicationContentMetaKey) * 32;
        NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(size);
        u32 total = 0;
        u32 written = 0;
        ncmOpenContentMetaDatabase(FsStorageId_GameCard, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if((total > 0) && (written > 0)) handler.pushUInt(metas[0].metaRecord.titleId);
        else handler.pushUInt(0);
    }
    else throwError(Context, Brew::API::Error::CommonError, "Could not initialize \'ncm\' service");
    ncmExit();
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::NX::getNANDTitles(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(R_SUCCEEDED(ncmInitialize()))
    {
        Brew::API::Array arr(Context);
        handler.pushArray(arr);
        NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
        size_t size = sizeof(NcmApplicationContentMetaKey) * 32;
        NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(size);
        u32 total = 0;
        u32 written = 0;
        ncmOpenContentMetaDatabase(FsStorageId_NandUser, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if(total > 0) for(u32 i = 0; i < total; i++) arr.addUInt(metas[i].metaRecord.titleId);
    }
    else throwError(Context, Brew::API::Error::CommonError, "Could not initialize \'ncm\' service");
    ncmExit();
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::NX::getSDTitles(Brew::API::NativeContext Context)
{
    Brew::API::FunctionHandler handler(Context);
    if(R_SUCCEEDED(ncmInitialize()))
    {
        Brew::API::Array arr(Context);
        handler.pushArray(arr);
        NcmContentMetaDatabase *metadb = (NcmContentMetaDatabase*)malloc(sizeof(*metadb));
        size_t size = sizeof(NcmApplicationContentMetaKey) * 32;
        NcmApplicationContentMetaKey *metas = (NcmApplicationContentMetaKey*)malloc(size);
        u32 total = 0;
        u32 written = 0;
        ncmOpenContentMetaDatabase(FsStorageId_SdCard, metadb);
        ncmContentMetaDatabaseListApplication(metadb, 0x80, metas, size, &written, &total);
        if(total > 0) for(u32 i = 0; i < total; i++) arr.addUInt(metas[i].metaRecord.titleId);
    }
    else throwError(Context, Brew::API::Error::CommonError, "Could not initialize \'ncm\' service");
    ncmExit();
    return Brew::API::Return::Variable;
}

Brew::API::Module Brew::BuiltIn::NX::initModule()
{
    nx.pushFunction("reboot", reboot);
    nx.pushFunction("shutDown", shutDown);
    nx.pushFunction("getTitles", getTitles);
    nx.pushFunction("getGameCartTitle", getGameCartTitle);
    nx.pushFunction("getNANDTitles", getNANDTitles);
    nx.pushFunction("getSDTitles", getSDTitles);
    return nx;
}