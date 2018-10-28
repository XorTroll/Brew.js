#include <Brew.js/Modules/NX/PegaSwitch.hpp>

namespace Brew::BuiltIn::PegaSwitch
{
    vector<u32> u32data;
    vector<u64> u64data;
    vector<u32> treg;
    map<pair<void*, size_t>, u32> desc;
    bool sendpid = false;

    API::Function Service_xDescriptor(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            void *buf = NULL;
            if(handler.CheckArgumentType(0, API::Type::String)) buf = (void*)handler.GetString(0).c_str();
            else if(handler.CheckArgumentType(0, API::Type::Buffer)) buf = duk_get_buffer_data(Context, 0, NULL);
            else API::ThrowError(Context, API::Error::CommonError, "Unknown type?");
            size_t bsize = handler.GetUInt(1);
            desc.insert(pair<pair<void*, size_t>, u32>(pair<void*, size_t>(buf, bsize), 0));
        }
        return API::Return::Void;
    }

    API::Function Service_cDescriptor(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(2))
        {
            size_t bsize;
            void *buf = duk_get_buffer_data(Context, 0, &bsize);
            desc.insert(pair<pair<void*, size_t>, u32>(pair<void*, size_t>(buf, bsize), 1));
        }
        return API::Return::Void;
    }

    API::Function Service_datau32(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            u32 data = (u32)handler.GetUInt(0);
            u32data.push_back(data);
            treg.push_back(0);
        }
        return API::Return::Void;
    }

    API::Function Service_datau64(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            u64 data = handler.GetUInt(0);
            u64data.push_back(data);
            treg.push_back(1);
        }
        return API::Return::Void;
    }

    API::Function Service_sendPid(API::NativeContext Context)
    {
        sendpid = true;
        return API::Return::Void;
    }

    API::Function Service_send(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        size_t csize = (sizeof(u64) * 2) + (sizeof(u32) * u32data.size()) + (sizeof(u64) * u64data.size());
        ::Service srv = { (Handle)handler.GetPropertyUInt("Handle"), (u32)handler.GetPropertyUInt("ObjectID"), (ServiceType)handler.GetPropertyUInt("Type") };
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
        u64 cmdid = handler.GetPropertyUInt("CmdID");
        ((u64*)raw)[1] = cmdid;
        u32 last32 = 0;
        u32 last64 = 0;
        if(!treg.empty()) for(u32 i = 0; i < treg.size(); i++)
        {
            u32 lpos = 2 + last32 + last64;
            if(treg[i] == 0)
            {
                ((u32*)raw)[lpos] = u32data[last32];
                last32++;
            }
            else if(treg[i] == 1)
            {
                ((u64*)raw)[lpos] = u64data[last64];
                last64++;
            }
        }
        rc = serviceIpcDispatch(&srv);
        if(R_FAILED(rc)) API::ThrowError(Context, API::Error::CommonError, "IPC dispatch failed (command ID " + to_string(cmdid) + ")");
        IpcParsedCommand r;
        ipcParse(&r);
        void *rsp = r.Raw;
        rc = ((Result*)rsp)[1];
        if(R_FAILED(rc)) API::ThrowError(Context, API::Error::CommonError, "IPC command result was not 0");
        u32data.clear();
        u64data.clear();
        treg.clear();
        sendpid = false;
        API::Array arr(Context);
        handler.ReturnArray(arr);
        for(u32 i = 0; i < IPC_MAX_OBJECTS; i++) arr.AddUInt(r.Handles[i]);
        return API::Return::Variable;
    }

    API::Function Service_ipcMsg(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            u64 cmdid = handler.GetUInt(0);
            handler.SetPropertyUInt("CmdID", cmdid);
        }
        return API::Return::Void;
    }

    API::Function Service_getService(API::NativeContext Context)
    {
        API::ClassHandler handler(Context);
        if(handler.CheckArgcEqualOrBigger(1))
        {
            ::Service srv;
            string srvname = handler.GetString(0);
            Result rc = smGetService(&srv, srvname.c_str());
            if(R_FAILED(rc)) API::ThrowError(Context, API::Error::CommonError, "Could not access \'" + srvname + "\' service");
            handler.SetPropertyUInt("Handle", srv.handle);
            handler.SetPropertyUInt("ObjectID", srv.object_id);
            handler.SetPropertyUInt("Type", srv.type);
        }
        return API::Return::Void;
    }

    API::Class Service()
    {
        API::Class cService("Service", [](API::NativeContext Context) -> API::Function
        {
            API::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                if(handler.CheckArgumentType(0, API::Type::Number))
                {
                    ::Service srv;
                    Handle hdl = handler.GetUInt(0);
                    serviceCreate(&srv, hdl);
                    handler.SetPropertyUInt("Handle", srv.handle);
                    handler.SetPropertyUInt("ObjectID", srv.object_id);
                    handler.SetPropertyUInt("Type", srv.type);
                }
                else if(handler.CheckArgumentType(0, API::Type::String))
                {
                    ::Service srv;
                    string srvname = handler.GetString(0);
                    Result rc = smGetService(&srv, srvname.c_str());
                    if(R_FAILED(rc)) API::ThrowError(Context, API::Error::CommonError, "Could not access \'" + srvname + "\' service");
                    handler.SetPropertyUInt("Handle", srv.handle);
                    handler.SetPropertyUInt("ObjectID", srv.object_id);
                    handler.SetPropertyUInt("Type", srv.type);
                }
            }
            return API::Return::Void;
        });
        cService.AddFunction("xDescriptor", Service_xDescriptor);
        cService.AddFunction("cDescriptor", Service_cDescriptor);
        cService.AddFunction("datau32", Service_datau32);
        cService.AddFunction("datau64", Service_datau64);
        cService.AddFunction("sendPid", Service_sendPid);
        cService.AddFunction("send", Service_send);
        cService.AddFunction("ipcMsg", Service_ipcMsg);
        cService.AddFunction("getService", Service_getService);
        return cService;
    }

    API::Module Initialize()
    {
        API::Module pegaswitch("pegaswitch");
        API::Class cService = Service();
        pegaswitch.PushClass(cService);
        return pegaswitch;
    }
}