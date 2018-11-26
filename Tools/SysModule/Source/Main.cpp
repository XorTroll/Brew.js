#include <bjs/bjs.hpp>
#include <bjs/sysmodule.hpp>

extern "C"
{
    extern u32 __start__;
    u32 __nx_applet_type = AppletType_None;
    #define bjsInnerHeap 0x20000
    size_t nx_inner_heap_size = bjsInnerHeap;
    char   nx_inner_heap[bjsInnerHeap];
    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);

}


void __libnx_initheap(void)
{
	void* addr = nx_inner_heap;
	size_t size = nx_inner_heap_size;
	extern char* fake_heap_start;
	extern char* fake_heap_end;
	fake_heap_start = (char*)addr;
	fake_heap_end = (char*)addr + size;
}


void __appInit(void)
{
    Result rc = 0;
    svcSleepThread(3000000000L);
    rc = smInitialize();
    if(rc != 0) fatalSimple(MAKERESULT(100, 101));
    rc = fsInitialize();
    if(rc != 0) fatalSimple(MAKERESULT(100, 102));
    rc = fsdevMountSdmc();
    if(rc != 0) fatalSimple(MAKERESULT(100, 103));
    rc = timeInitialize();
    if(rc != 0) fatalSimple(MAKERESULT(100, 104));
}

void __appExit(void)
{
    timeExit();
    fsdevUnmountAll();
    fsExit();
    smExit();
}

int main(int argc, char **argv)
{
    auto server = new WaitableManager(U64_MAX);
    server->AddWaitable(new ServiceServer<bjs::sysmodule::JavaScriptService>("brew:js", 1));
    server->Process();
    delete server;
    return 0;
}