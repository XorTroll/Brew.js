
#pragma once
#include <bjs/js.hpp>

namespace bjs::node::path
{
    js::Function basename(js::NativeContext Context);
    js::Function extname(js::NativeContext Context);
}