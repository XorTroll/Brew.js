#include <bjs/platform.hpp>

namespace bjs
{
    Platform GetCurrentPlatform()
    {
        return static_cast<Platform>(bjsPlatformIndex);
    }
}