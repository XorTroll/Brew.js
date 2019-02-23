
#pragma once
#include <bjs/platform.hpp>
#include <bjs/lib.hpp>

namespace bjs::js
{
    /// Represents the value which should be returned on an API JS function. (duk_ret_t)
    typedef duk_ret_t Function;

    /// Represents a native duktape context, the base of JavaScript evaluation. (duk_context*)
    typedef duk_context *NativeContext;

    /// Represents an API JS function as a variable, for adding them to arrays, objects or modules. (duk_c_function / duk_ret_t(duk_context*) function)
    typedef duk_c_function NativeFunction;
    
    /// JavaScript types supported by duktape engine.
    enum class Type
    {
        None,        ///< No type (possibly an error getting the type?)
        Undefined,   ///< Undefined type, similar to plain JS.
        Null,        ///< Null type, similar to plain JS.
        Boolean,     ///< Boolean type, similar to plain JS.
        Number,      ///< Number type, similar to plain JS.
        String,      ///< String type, similar to plain JS.
        ObjectArray, ///< Object/Array type,  type, similar to plain JS. (in duktape both are same type)
        Buffer,      ///< Buffer type, like plain JS/Node.js ones.
        Pointer,     ///< Pointer type (not used by the API but duktape uses it)
    };
    
    /// Throwable errors during the execution.
    enum class Error
    {
        NoError,         ///< Indicates that no error ocurred.
        CommonError,     ///< Normal, non-specific error ("Error: ...")
        EvalError,       ///< Evaluation error ("EvalError: ...")
        RangeError,      ///< Range error ("RangeError: ...")
        ReferenceError,  ///< Reference error ("ReferenceError: ...")
        SyntaxError,     ///< Syntax error ("SyntaxError: ..."), this is thrown when the API fails parsing the code.
        TypeError,       ///< Type-related error ("TypeError: ...")
        URIError,        ///< URI-related error ("URIError: ...")
    };
    
    /// Possible return values in API JS functions.
    enum Return
    {
        Void = 0,                 ///< Void return: no variable (undefined) will be returned even if it's passed via a FunctionHandler.
        Variable = 1,             ///< A variable is expected to be passed before the function returns this, via a FunctionHandler.
        ErrorCode = -1,           ///< Similar to throwing CommonError.
        EvalErrorCode = -2,       ///< Similar to throwing EvalError.
        RangeErrorCode = -3,      ///< Similar to throwing RangeError.
        ReferenceErrorCode = -4,  ///< Similar to throwing ReferenceError.
        SyntaxErrorCode = -5,     ///< Similar to throwing SyntaxError.
        TypeErrorCode = -6,       ///< Similar to throwing TypeError.
        URIErrorCode = -7,        ///< Similar to throwing URIError.
    };
}