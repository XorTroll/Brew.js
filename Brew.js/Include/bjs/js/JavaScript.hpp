
#pragma once
#include <bjs/js/Base.hpp>
#include <string>

namespace bjs::js
{
	/**
		@brief Creates a context using default heap.
		@note This is a wrapper for duktape's "duk_create_heap_default" function.
		@return The created context.
	*/
	NativeContext CreateContext();

	/**
		@brief Aborts the execution throwing an error.
		@param Context The context to throw the error on.
		@param ErrorType The error to throw.
		@param Message the message of the thrown error.
	*/
	void ThrowError(NativeContext Context, Error ErrorType, std::string Message);
}