#include <bjs/js/JavaScript.hpp>

namespace bjs::js
{
    NativeContext CreateContext()
    {
        return duk_create_heap_default();
    }

    void ThrowError(NativeContext Context, Error ErrorType = Error::CommonError, std::string Message = "An error was thrown.")
    {
        duk_error(Context, static_cast<duk_errcode_t>(ErrorType), Message.c_str());
    }
}