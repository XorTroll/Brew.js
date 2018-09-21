#include "PegaSwitch.hpp"

Brew::API::Module pegaswitch("pegaswitch");
vector<u32> u32data;
vector<u64> u64data;
vector<u32> treg;
map<pair<void*, size_t>, u32> desc;
bool sendpid = false;

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_xDescriptor(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(2))
    {
        void *buf = NULL;
        if(handler.checkArgType(0, Brew::API::Type::String)) buf = (void*)handler.getString(0).c_str();
        else if(handler.checkArgType(0, Brew::API::Type::Buffer)) buf = duk_get_buffer_data(Context, 0, NULL);
        else throwError(Context, Brew::API::Error::CommonError, "Unknown type?");
        size_t bsize = handler.getUInt(1);
        desc.insert(pair<pair<void*, size_t>, u32>(pair<void*, size_t>(buf, bsize), 0));
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_cDescriptor(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(2))
    {
        size_t bsize;
        void *buf = duk_get_buffer_data(Context, 0, &bsize);
        desc.insert(pair<pair<void*, size_t>, u32>(pair<void*, size_t>(buf, bsize), 1));
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_datau32(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(1))
    {
        u32 data = (u32)handler.getUInt(0);
        u32data.push_back(data);
        treg.push_back(0);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_datau64(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(1))
    {
        u64 data = handler.getUInt(0);
        u64data.push_back(data);
        treg.push_back(1);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_sendPid(Brew::API::NativeContext Context)
{
    sendpid = true;
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_send(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    size_t csize = sizeof(u64) + sizeof(u64) + (sizeof(u32) * u32data.size()) + (sizeof(u64) * u64data.size());
    ::Service srv = { handler.getPropertyUInt("Handle"), handler.getPropertyUInt("ObjectID"), (ServiceType)handler.getPropertyUInt("Type") };
    Result rc = 0;
    IpcCommand c;
    ipcInitialize(&c);
    if(sendpid) ipcSendPid(&c);
    u8 idx = 0;
    if(!desc.empty()) for(auto const& ds : desc)
    {
        if(ds.second == 0) ipcAddSendStatic(&c, ds.first.first, ds.first.second, idx);
        else if(ds.second == 1) ipcAddRecvStatic(&c, ds.first.first, ds.first.second, idx);
        idx++;
    }
    void *raw = ipcPrepareHeader(&c, csize);
    ((u64*)raw)[0] = SFCI_MAGIC;
    u64 cmdid = handler.getPropertyUInt("CmdID");
    ((u64*)raw)[1] = cmdid;
    u32 last32 = 0;
    u32 last64 = 0;
    if(!treg.empty()) for(u32 i = 0; i < treg.size(); i++)
    {
        if(treg[i] == 0)
        {
            ((u32*)raw)[2 + last32 + last64] = u32data[last32];
            last32++;
        }
        else if(treg[i] == 1)
        {
            ((u64*)raw)[2 + last32 + last64] = u64data[last64];
            last64++;
        }
    }
    rc = serviceIpcDispatch(&srv);
    if(R_FAILED(rc)) throwError(Context, Brew::API::Error::CommonError, "IPC dispatch failed: command ID " + to_string(cmdid));
    IpcParsedCommand r;
    ipcParse(&r);
    void *rsp = r.Raw;
    rc = ((Result*)rsp)[1];
    if(R_FAILED(rc)) throwError(Context, Brew::API::Error::CommonError, "IPC command result was not 0");
    u32data.clear();
    u64data.clear();
    treg.clear();
    sendpid = false;
    Brew::API::Array arr(Context);
    handler.pushArray(arr);
    for(u32 i = 0; i < IPC_MAX_OBJECTS; i++) arr.addUInt(r.Handles[i]);
    return Brew::API::Return::Variable;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_ipcMsg(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(1))
    {
        u64 cmdid = handler.getUInt(0);
        handler.setPropertyUInt("CmdID", cmdid);
    }
    return Brew::API::Return::Void;
}

Brew::API::Function Brew::BuiltIn::PegaSwitch::Service_getService(Brew::API::NativeContext Context)
{
    Brew::API::ClassHandler handler(Context);
    if(handler.checkArgc(1))
    {
        ::Service srv;
        string srvname = handler.getString(0);
        Result rc = smGetService(&srv, srvname.c_str());
        if(R_FAILED(rc)) throwError(Context, Brew::API::Error::CommonError, "Could not access \'" + srvname + "\' service");
        handler.setPropertyUInt("Handle", srv.handle);
        handler.setPropertyUInt("ObjectID", srv.object_id);
        handler.setPropertyUInt("Type", srv.type);
    }
    return Brew::API::Return::Void;
}

Brew::API::Class Brew::BuiltIn::PegaSwitch::Service()
{
    Brew::API::Class cService("Service", [](Brew::API::NativeContext Context) -> Brew::API::Function
    {
        Brew::API::ClassHandler handler(Context);
        if(handler.checkArgc(1))
        {
            if(handler.checkArgType(0, Brew::API::Type::Number))
            {
                ::Service srv;
                Handle hdl = handler.getUInt(0);
                serviceCreate(&srv, hdl);
                handler.setPropertyUInt("Handle", srv.handle);
                handler.setPropertyUInt("ObjectID", srv.object_id);
                handler.setPropertyUInt("Type", srv.type);
            }
            else if(handler.checkArgType(0, Brew::API::Type::String))
            {
                ::Service srv;
                string srvname = handler.getString(0);
                Result rc = smGetService(&srv, srvname.c_str());
                if(R_FAILED(rc)) throwError(Context, Brew::API::Error::CommonError, "Could not access \'" + srvname + "\' service");
                handler.setPropertyUInt("Handle", srv.handle);
                handler.setPropertyUInt("ObjectID", srv.object_id);
                handler.setPropertyUInt("Type", srv.type);
            }
        }
        return Brew::API::Return::Void;
    });
    cService.addFunction("xDescriptor", Service_xDescriptor);
    cService.addFunction("cDescriptor", Service_cDescriptor);
    cService.addFunction("datau32", Service_datau32);
    cService.addFunction("datau64", Service_datau64);
    cService.addFunction("sendPid", Service_sendPid);
    cService.addFunction("send", Service_send);
    cService.addFunction("ipcMsg", Service_ipcMsg);
    cService.addFunction("getService", Service_getService);
    return cService;
}

Brew::API::Module Brew::BuiltIn::PegaSwitch::initModule()
{
    pegaswitch.pushClass(Service());
    return pegaswitch;
}