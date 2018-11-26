#include <bjs/platform/libnx/pegaswitch/Service.hpp>

namespace bjs::libnx::pegaswitch
{
    std::vector<u32> u32data;
    std::vector<u64> u64data;
    std::vector<u32> treg;
    std::map<std::pair<void*, size_t>, u32> desc;
    bool sendpid = false;

    namespace Service
    {
        js::Function xDescriptor(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(2))
            {
                void *buf = NULL;
                if(handler.CheckArgumentType(0, js::Type::String)) buf = (void*)handler.GetString(0).c_str();
                else if(handler.CheckArgumentType(0, js::Type::Buffer)) buf = duk_get_buffer_data(Context, 0, NULL);
                else js::ThrowError(Context, js::Error::CommonError, "Unknown type?");
                size_t bsize = handler.GetUInt(1);
                desc.insert(std::pair<std::pair<void*, size_t>, u32>(std::pair<void*, size_t>(buf, bsize), 0));
            }
            return js::Return::Void;
        }

        js::Function cDescriptor(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(2))
            {
                size_t bsize;
                void *buf = duk_get_buffer_data(Context, 0, &bsize);
                desc.insert(std::pair<std::pair<void*, size_t>, u32>(std::pair<void*, size_t>(buf, bsize), 1));
            }
            return js::Return::Void;
        }

        js::Function datau32(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u32 data = (u32)handler.GetUInt(0);
                u32data.push_back(data);
                treg.push_back(0);
            }
            return js::Return::Void;
        }

        js::Function datau64(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u64 data = handler.GetUInt(0);
                u64data.push_back(data);
                treg.push_back(1);
            }
            return js::Return::Void;
        }

        js::Function sendPid(js::NativeContext Context)
        {
            sendpid = true;
            return js::Return::Void;
        }

        js::Function send(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
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
            if(R_FAILED(rc)) js::ThrowError(Context, js::Error::CommonError, "IPC dispatch failed (command ID " + std::to_string(cmdid) + ")");
            IpcParsedCommand r;
            ipcParse(&r);
            void *rsp = r.Raw;
            rc = ((Result*)rsp)[1];
            if(R_FAILED(rc)) js::ThrowError(Context, js::Error::CommonError, "IPC command result was not 0");
            u32data.clear();
            u64data.clear();
            treg.clear();
            sendpid = false;
            js::Array arr(Context);
            handler.ReturnArray(arr);
            for(u32 i = 0; i < IPC_MAX_OBJECTS; i++) arr.AddUInt(r.Handles[i]);
            return js::Return::Variable;
        }

        js::Function ipcMsg(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u64 cmdid = handler.GetUInt(0);
                handler.SetPropertyUInt("CmdID", cmdid);
            }
            return js::Return::Void;
        }

        js::Function getService(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                ::Service srv;
                std::string srvname = handler.GetString(0);
                Result rc = smGetService(&srv, srvname.c_str());
                if(R_FAILED(rc)) js::ThrowError(Context, js::Error::CommonError, "Could not access \'" + srvname + "\' service");
                handler.SetPropertyUInt("Handle", srv.handle);
                handler.SetPropertyUInt("ObjectID", srv.object_id);
                handler.SetPropertyUInt("Type", srv.type);
            }
            return js::Return::Void;
        }

        js::Class CreateClass()
        {
            js::Class cService("Service", [](js::NativeContext Context) -> js::Function
            {
                js::ClassHandler handler(Context);
                if(handler.CheckArgcEqualOrBigger(1))
                {
                    if(handler.CheckArgumentType(0, js::Type::Number))
                    {
                        ::Service srv;
                        Handle hdl = handler.GetUInt(0);
                        serviceCreate(&srv, hdl);
                        handler.SetPropertyUInt("Handle", srv.handle);
                        handler.SetPropertyUInt("ObjectID", srv.object_id);
                        handler.SetPropertyUInt("Type", srv.type);
                    }
                    else if(handler.CheckArgumentType(0, js::Type::String))
                    {
                        ::Service srv;
                        std::string srvname = handler.GetString(0);
                        Result rc = smGetService(&srv, srvname.c_str());
                        if(R_FAILED(rc)) js::ThrowError(Context, js::Error::CommonError, "Could not access \'" + srvname + "\' service");
                        handler.SetPropertyUInt("Handle", srv.handle);
                        handler.SetPropertyUInt("ObjectID", srv.object_id);
                        handler.SetPropertyUInt("Type", srv.type);
                    }
                }
                return js::Return::Void;
            });
            cService.AddFunction("xDescriptor", xDescriptor);
            cService.AddFunction("cDescriptor", cDescriptor);
            cService.AddFunction("datau32", datau32);
            cService.AddFunction("datau64", datau64);
            cService.AddFunction("sendPid", sendPid);
            cService.AddFunction("send", send);
            cService.AddFunction("ipcMsg", ipcMsg);
            cService.AddFunction("getService", getService);
            return cService;
        }
    }
}