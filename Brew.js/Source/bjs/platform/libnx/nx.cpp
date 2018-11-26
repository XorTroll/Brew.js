#include <bjs/platform/libnx/nx.hpp>

namespace bjs::libnx::nx
{
    js::Module CreateModule()
    {
        js::Module nx("nx");
        nx.PushFunction("reboot", reboot);
        nx.PushFunction("shutDown", shutDown);
        nx.PushFunction("getAllTitles", getAllTitles);
        nx.PushFunction("getGameCartTitle", getGameCartTitle);
        nx.PushFunction("getNANDTitles", getNANDTitles);
        nx.PushFunction("getSDCardTitles", getSDCardTitles);
        return nx;
    }
}