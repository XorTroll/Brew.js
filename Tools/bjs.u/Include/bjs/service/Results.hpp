
#pragma once
#include <bjs/Brew.js>
#include <stratosphere.hpp>

namespace bjs::service
{
    static const u32 ResultModule = 356;
    static const Result ResultBadNativeContext = MAKERESULT(ResultModule, 1);
    static const Result ResultAlreadyInitialized = MAKERESULT(ResultModule, 2);
    static const Result ResultNotInitialized = MAKERESULT(ResultModule, 2);
    static const Result ResultJavaScriptError = MAKERESULT(ResultModule, 3);
}