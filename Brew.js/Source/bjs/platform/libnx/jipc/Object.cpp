#include <bjs/platform/libnx/jipc/Object.hpp>
#include <iostream>

namespace bjs::libnx::jipc
{
    struct jipcCommandBlock
    {
        u8 type;
        size_t inrawoff;
        u8 bufparam;
        size_t size;
        void *data;
    };

    void split64(u64 val, u32 *h, u32 *l)
    {
        *h = (val >> 32);
        *l = (val & 0xffffffffl);
    }

    u64 join64(u32 h, u32 l)
    {
        return (((u64)h << 32) | l);
    }

    namespace Object
    {
        js::Function isValid(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u64 type = handler.GetPropertyUInt("Type");
            handler.ReturnBoolean(type != 0);
            return js::Return::Variable;
        }

        js::Function isDomain(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u64 type = handler.GetPropertyUInt("Type");
            handler.ReturnBoolean(type == 2);
            return js::Return::Variable;
        }

        js::Function convertToDomain(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            Service s;
            s.handle = (Handle)handler.GetPropertyUInt("Handle");
            s.object_id = (u32)handler.GetPropertyUInt("ObjectId");
            s.type = (ServiceType)handler.GetPropertyUInt("Type");
            Result rc = serviceConvertToDomain(&s);
            if(rc == 0)
            {
                handler.SetPropertyUInt("Handle", s.handle);
                handler.SetPropertyUInt("ObjectId", s.object_id);
                handler.SetPropertyUInt("Type", s.type);
            }
            handler.ReturnUInt(rc);
            return js::Return::Variable;
        }

        js::Function isSubService(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u64 type = handler.GetPropertyUInt("Type");
            handler.ReturnBoolean(type == 3);
            return js::Return::Variable;
        }

        js::Function queryPointerBufferSize(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u64 size = 0;
            Handle oh = (Handle)handler.GetPropertyUInt("Handle");
            ipcQueryPointerBufferSize(oh, &size);
            handler.ReturnUInt(size);
            return js::Return::Variable;
        }

        js::Function close(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            Service s;
            s.handle = (Handle)handler.GetPropertyUInt("Handle");
            s.object_id = (u32)handler.GetPropertyUInt("ObjectId");
            s.type = (ServiceType)handler.GetPropertyUInt("Type");
            serviceClose(&s);
            handler.SetPropertyUInt("Handle", s.handle);
            handler.SetPropertyUInt("ObjectId", s.object_id);
            handler.SetPropertyUInt("Type", s.type);
            return js::Return::Void;
        }

        js::Function prepareForRequest(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            IpcCommand *cmd = (IpcCommand*)malloc(sizeof(IpcCommand));
            memset(cmd, 0, sizeof(IpcCommand));
            ipcInitialize(cmd);
            jipcCommandBlock *cmds = (jipcCommandBlock*)malloc(sizeof(jipcCommandBlock) * 16);
            memset(cmds, 0, sizeof(jipcCommandBlock) * 16);
            u32 cmdh = 0;
            u32 cmdl = 0;
            split64((u64)cmd, &cmdh, &cmdl);
            u32 cmdsh = 0;
            u32 cmdsl = 0;
            split64((u64)cmds, &cmdsh, &cmdsl);
            handler.SetPropertyUInt("HCommandAddress", cmdh);
            handler.SetPropertyUInt("LCommandAddress", cmdl);
            handler.SetPropertyUInt("HBlockAddress", cmdsh);
            handler.SetPropertyUInt("LBlockAddress", cmdsl);
            return js::Return::Void;
        }

        js::Function requestSendProcessId(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u32 cmdh = handler.GetPropertyUInt("HCommandAddress");
            u32 cmdl = handler.GetPropertyUInt("LCommandAddress");
            u64 cmda = join64(cmdh, cmdl);
            IpcCommand *cmd = (IpcCommand*)cmda;
            ipcSendPid(cmd);
            return js::Return::Void;
        }

        js::Function requestSetIn(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(2))
            {
                std::string format = handler.GetString(0);
                u32 bwidx = handler.GetPropertyUInt("BlockWriteIndex");
                u32 cbah = handler.GetPropertyUInt("HBlockAddress");
                u32 cbal = handler.GetPropertyUInt("LBlockAddress");
                u64 cba = join64(cbah, cbal);
                jipcCommandBlock *cmds = (jipcCommandBlock*)cba;
                jipcCommandBlock *cmdb = &cmds[bwidx];
                if(format == "u64")
                {
                    u64 val = (u64)handler.GetUInt(1);
                    cmdb->type = 0;
                    cmdb->data = (u64*)&val;
                    bwidx++;
                }
                else if(format == "u32")
                {
                    u32 val = (u32)handler.GetUInt(1);
                    cmdb->type = 1;
                    cmdb->data = (u32*)&val;
                    bwidx++;
                }
                else if(format == "u8")
                {
                    u8 val = (u8)handler.GetUInt(1);
                    cmdb->type = 2;
                    cmdb->data = (u8*)&val;
                    bwidx++;
                }
                else if(format == "inBuffer")
                {
                    js::Buffer buf = handler.GetBuffer(1);
                    u32 type = handler.GetUInt(2);
                    cmdb->type = 3;
                    cmdb->bufparam = type;
                    cmdb->size = buf.Size;
                    cmdb->data = buf.Pointer;
                    bwidx++;
                }
                else if(format == "inStatic")
                {
                    js::Buffer buf = handler.GetBuffer(1);
                    u32 idx = handler.GetUInt(2);
                    cmdb->type = 4;
                    cmdb->bufparam = idx;
                    cmdb->size = buf.Size;
                    cmdb->data = buf.Pointer;
                    bwidx++;
                }
                else if(format == "outBuffer")
                {
                    js::Buffer buf = handler.GetBuffer(1);
                    u32 type = handler.GetUInt(2);
                    cmdb->type = 5;
                    cmdb->bufparam = type;
                    cmdb->size = buf.Size;
                    cmdb->data = buf.Pointer;
                    bwidx++;
                }
                else if(format == "outStatic")
                {
                    js::Buffer buf = handler.GetBuffer(1);
                    u32 idx = handler.GetUInt(2);
                    cmdb->type = 6;
                    cmdb->bufparam = idx;
                    cmdb->size = buf.Size;
                    cmdb->data = buf.Pointer;
                    bwidx++;
                }
                else if(format == "rawData")
                {
                    js::Buffer buf = handler.GetBuffer(1);
                    cmdb->type = 7;
                    cmdb->size = buf.Size;
                    cmdb->data = buf.Pointer;
                    bwidx++;
                }
                handler.SetPropertyUInt("BlockWriteIndex", bwidx);
            }
            return js::Return::Void;
        }

        js::Function processRequest(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u32 cmdid = handler.GetUInt(0);
                u32 cbah = handler.GetPropertyUInt("HBlockAddress");
                u32 cbal = handler.GetPropertyUInt("LBlockAddress");
                u64 cba = join64(cbah, cbal);
                u32 bwidx = handler.GetPropertyUInt("BlockWriteIndex");
                u32 cmdah = handler.GetPropertyUInt("HCommandAddress");
                u32 cmdal = handler.GetPropertyUInt("LCommandAddress");
                u64 cmda = join64(cmdah, cmdal);
                IpcCommand *icmd = (IpcCommand*)cmda;
                Service s;
                s.handle = (Handle)handler.GetPropertyUInt("Handle");
                s.object_id = (u32)handler.GetPropertyUInt("ObjectId");
                s.type = (ServiceType)handler.GetPropertyUInt("Type");
                jipcCommandBlock *cmds = (jipcCommandBlock*)cba;
                size_t inrawsz = 0;
                inrawsz += (alignof(u64) - 1);
                inrawsz -= (inrawsz % alignof(u64));
                size_t ofmagic = inrawsz;
                inrawsz += sizeof(u64);
                inrawsz += (alignof(u64) - 1);
                inrawsz -= (inrawsz % alignof(u64));
                size_t ofcmdid = inrawsz;
                inrawsz += sizeof(u64);
                if(bwidx > 0) for(u32 i = 0; i < bwidx; i++)
                {
                    jipcCommandBlock *cmd = &cmds[i];
                    size_t roff = 0;
                    switch(cmd->type)
                    {
                        case 0:
                            inrawsz += (alignof(u64) - 1);
                            inrawsz -= (inrawsz % alignof(u64));
                            roff = inrawsz;
                            inrawsz += sizeof(u64);
                            cmd->inrawoff = roff;
                            break;
                        case 1:
                            inrawsz += (alignof(u32) - 1);
                            inrawsz -= (inrawsz % alignof(u32));
                            roff = inrawsz;
                            inrawsz += sizeof(u32);
                            cmd->inrawoff = roff;
                            break;
                        case 2:
                            inrawsz += (alignof(u8) - 1);
                            inrawsz -= (inrawsz % alignof(u8));
                            roff = inrawsz;
                            inrawsz += sizeof(u8);
                            cmd->inrawoff = roff;
                            break;
                        case 3:
                            ipcAddSendBuffer(icmd, cmd->data, cmd->size, (BufferType)cmd->bufparam);
                            break;
                        case 4:
                            ipcAddSendStatic(icmd, cmd->data, cmd->size, cmd->bufparam);
                            break;
                        case 5:
                            ipcAddRecvBuffer(icmd, cmd->data, cmd->size, (BufferType)cmd->bufparam);
                            break;
                        case 6:
                            ipcAddRecvStatic(icmd, cmd->data, cmd->size, cmd->bufparam);
                            break;
                        case 7:
                            roff = inrawsz;
                            inrawsz += cmd->size;
                            cmd->inrawoff = roff;
                            break;
                    }
                }
                void *inraw = ipcPrepareHeader(icmd, inrawsz);
                *((u64*)(((u8*)inraw) + ofmagic)) = SFCI_MAGIC;
                *((u64*)(((u8*)inraw) + ofcmdid)) = (u64)cmdid;
                if(bwidx > 0) for(u32 i = 0; i < bwidx; i++)
                {
                    jipcCommandBlock *cmd = &cmds[i];
                    if(cmd != NULL) switch(cmd->type)
                    {
                        case 0:
                            *((u64*)(((u8*)inraw) + cmd->inrawoff)) = *((u64*)cmd->data);
                            break;
                        case 1:
                            *((u32*)(((u8*)inraw) + cmd->inrawoff)) = *((u32*)cmd->data);
                            break;
                        case 2:
                            *((u8*)(((u8*)inraw) + cmd->inrawoff)) = *((u8*)cmd->data);
                            break;
                        case 7:
                            memcpy(((u8*)(((u8*)inraw) + cmd->inrawoff)), cmd->data, cmd->size);
                            break;
                    }
                }
                Result rc = serviceIpcDispatch(&s);
                free(icmd);
                icmd = NULL;
                if(rc == 0)
                {
                    IpcParsedCommand *pcmd = (IpcParsedCommand*)malloc(sizeof(IpcParsedCommand));
                    memset(pcmd, 0, sizeof(IpcParsedCommand));
                    rc = ipcParse(pcmd);
                    if(rc == 0)
                    {
                        size_t srawsz = 0;
                        srawsz += (alignof(u64) - 1);
                        srawsz -= (srawsz % alignof(u64));
                        srawsz += sizeof(u64);
                        srawsz += (alignof(u64) - 1);
                        srawsz -= (srawsz % alignof(u64));
                        u64 resoff = srawsz;
                        srawsz += sizeof(u64);
                        rc = (u32)(*((u64*)(((u8*)pcmd->Raw) + resoff)));
                        u32 cmdh = 0;
                        u32 cmdl = 0;
                        split64((u64)pcmd, &cmdh, &cmdl);
                        handler.SetPropertyUInt("HCommandAddress", cmdh);
                        handler.SetPropertyUInt("LCommandAddress", cmdl);
                        handler.SetPropertyUInt("OutRawSize", srawsz);
                    }
                }
                handler.ReturnUInt(rc);
                return js::Return::Variable;
            }
            return js::Return::Void;
        }

        js::Function requestPopOut(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            if(handler.CheckArgcEqualOrBigger(1))
            {
                u32 cbah = handler.GetPropertyUInt("HBlockAddress");
                u32 cbal = handler.GetPropertyUInt("LBlockAddress");
                u64 cba = join64(cbah, cbal);
                jipcCommandBlock *cmds = (jipcCommandBlock*)cba;
                u32 cmdah = handler.GetPropertyUInt("HCommandAddress");
                u32 cmdal = handler.GetPropertyUInt("LCommandAddress");
                u64 cmda = join64(cmdah, cmdal);
                IpcParsedCommand *pcmd = (IpcParsedCommand*)cmda;
                std::string format = handler.GetString(0);
                size_t orawsz = handler.GetPropertyUInt("OutRawSize");
                u32 bwidx = handler.GetPropertyUInt("BlockWriteIndex");
                if(format == "u64")
                {
                    orawsz += (alignof(u64) - 1);
                    orawsz -= (orawsz % alignof(u64));
                    u64 valoff = orawsz;
                    orawsz += sizeof(u64);
                    u64 val = (*((u64*)(((u8*)pcmd->Raw) + valoff)));
                    handler.SetPropertyUInt("OutRawSize", orawsz);
                    handler.ReturnUInt(val);
                    return js::Return::Variable;
                }
                else if(format == "u32")
                {
                    orawsz += (alignof(u32) - 1);
                    orawsz -= (orawsz % alignof(u32));
                    u64 valoff = orawsz;
                    orawsz += sizeof(u32);
                    u32 val = (*((u32*)(((u8*)pcmd->Raw) + valoff)));
                    handler.SetPropertyUInt("OutRawSize", orawsz);
                    handler.ReturnUInt(val);
                    return js::Return::Variable;
                }
                else if(format == "u8")
                {
                    orawsz += (alignof(u8) - 1);
                    orawsz -= (orawsz % alignof(u8));
                    u64 valoff = orawsz;
                    orawsz += sizeof(u8);
                    u8 val = (*((u8*)(((u8*)pcmd->Raw) + valoff)));
                    handler.SetPropertyUInt("OutRawSize", orawsz);
                    handler.ReturnUInt(val);
                    return js::Return::Variable;
                }
                else if(format == "outBuffer")
                {
                    if(bwidx > 0) for(u32 i = 0; i < bwidx; i++)
                    {
                        jipcCommandBlock *cmd = &cmds[i];
                        if(cmd->type == 5)
                        {
                            handler.ReturnBuffer(cmd->data, cmd->size);
                            return js::Return::Variable;
                        }
                    }
                }
                else if(format == "outStatic")
                {
                    if(bwidx > 0) for(u32 i = 0; i < bwidx; i++)
                    {
                        jipcCommandBlock *cmd = &cmds[i];
                        if(cmd->type == 6)
                        {
                            handler.ReturnBuffer(cmd->data, cmd->size);
                            return js::Return::Variable;
                        }
                    }
                }
                else if(format == "rawData")
                {
                    u32 datasz = handler.GetUInt(1);
                    u64 valoff = orawsz;
                    orawsz += datasz;
                    void *data = malloc(datasz);
                    memcpy(data, ((u8*)(((u8*)pcmd->Raw) + valoff)), datasz);
                    handler.SetPropertyUInt("OutRawSize", orawsz);
                    handler.ReturnBuffer(data, datasz);
                    return js::Return::Variable;
                }
                else if(format == "handle")
                {
                    u32 idx = handler.GetUInt(1);
                    handler.ReturnUInt(pcmd->Handles[idx]);
                    return js::Return::Variable;
                }
                else if(format == "objectId")
                {
                    u32 idx = handler.GetUInt(1);
                    handler.ReturnUInt(pcmd->OutObjectIds[idx]);
                    return js::Return::Variable;
                }
            }
            return js::Return::Void;
        }

        js::Function finalizeRequest(js::NativeContext Context)
        {
            js::ClassHandler handler(Context);
            u32 cmdah = handler.GetPropertyUInt("HCommandAddress");
            u32 cmdal = handler.GetPropertyUInt("LCommandAddress");
            u64 cmda = join64(cmdah, cmdal);
            if(cmda != 0)
            {
                IpcParsedCommand *cmd = (IpcParsedCommand*)cmda;
                if(cmd != NULL)
                {
                    free(cmd);
                    cmd = NULL;
                }
            }
            handler.SetPropertyUInt("HCommandAddress", 0);
            handler.SetPropertyUInt("LCommandAddress", 0);
            handler.SetPropertyUInt("HBlockAddress", 0);
            handler.SetPropertyUInt("LBlockAddress", 0);
            handler.SetPropertyUInt("BlockWriteIndex", 0);
            handler.SetPropertyUInt("InRawSize", 0);
            handler.SetPropertyUInt("OutRawSize", 0);
            return js::Return::Void;
        }

        js::Class CreateClass()
        {
            js::Class cObject("Object", [](js::NativeContext Context) -> js::Function
            {
                js::ClassHandler handler(Context);
                handler.SetPropertyUInt("HCommandAddress", 0);
                handler.SetPropertyUInt("LCommandAddress", 0);
                handler.SetPropertyUInt("HBlockAddress", 0);
                handler.SetPropertyUInt("LBlockAddress", 0);
                handler.SetPropertyUInt("BlockWriteIndex", 0);
                handler.SetPropertyUInt("InRawSize", 0);
                handler.SetPropertyUInt("OutRawSize", 0);
                if(handler.CheckArgcEqual(0))
                {
                    handler.SetPropertyUInt("Handle", 0);
                    handler.SetPropertyUInt("ObjectId", 0);
                    handler.SetPropertyUInt("Type", 0);
                }
                else if(handler.CheckArgcEqual(1))
                {
                    Handle hdl = handler.GetUInt(0);
                    handler.SetPropertyUInt("Handle", hdl);
                    handler.SetPropertyUInt("ObjectId", UINT32_MAX);
                    handler.SetPropertyUInt("Type", 1);
                }
                else if(handler.CheckArgcEqual(2))
                {
                    Handle hdl = handler.GetUInt(0);
                    u32 objid = handler.GetUInt(1);
                    handler.SetPropertyUInt("Handle", hdl);
                    handler.SetPropertyUInt("ObjectId", objid);
                    handler.SetPropertyUInt("Type", 3);
                }
                else if(handler.CheckArgcEqualOrBigger(3))
                {
                    Handle hdl = handler.GetUInt(0);
                    u32 objid = handler.GetUInt(1);
                    u32 type = handler.GetUInt(2);
                    handler.SetPropertyUInt("Handle", hdl);
                    handler.SetPropertyUInt("ObjectId", objid);
                    handler.SetPropertyUInt("Type", type);
                }
                return js::Return::Void;
            });
            cObject.AddFunction("isValid", isValid);
            cObject.AddFunction("isDomain", isDomain);
            cObject.AddFunction("convertToDomain", convertToDomain);
            cObject.AddFunction("isSubService", isSubService);
            cObject.AddFunction("queryPointerBufferSize", queryPointerBufferSize);
            cObject.AddFunction("close", close);
            cObject.AddFunction("prepareForRequest", prepareForRequest);
            cObject.AddFunction("requestSendProcessId", requestSendProcessId);
            cObject.AddFunction("requestSetIn", requestSetIn);
            cObject.AddFunction("processRequest", processRequest);
            cObject.AddFunction("requestPopOut", requestPopOut);
            cObject.AddFunction("finalizeRequest", finalizeRequest);
            return cObject;
        }
    }
}