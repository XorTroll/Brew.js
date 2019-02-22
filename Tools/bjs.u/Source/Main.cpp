#include <bjs/service.hpp>

extern "C"
{
    extern u32 __start__;
    u32 __nx_applet_type = AppletType_None;
    #define INNER_HEAP_SIZE 0x50000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char nx_inner_heap[INNER_HEAP_SIZE];
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
    svcSleepThread(8000000000L);
    rc = smInitialize();
    rc = fsInitialize();
    rc = fsdevMountSdmc();
}

void __appExit(void)
{
    fsdevUnmountAll();
    fsExit();
    smExit();
}

int main()
{
    auto server = new WaitableManager(1);
    server->AddWaitable(new ServiceServer<bjs::service::IJavaScriptService>("bjs:u", 10));
    server->Process();
    delete server;
    return 0;
}