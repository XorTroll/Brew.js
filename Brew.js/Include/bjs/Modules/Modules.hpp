
#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules
{
    void Add(js::Module &NewModule);
    std::vector<js::Module> GetModules();

    /**
		@brief [Global] API JS Function: "function require(ModuleName) → Object"
		@note Imports (tries to import) a module. This description is for using the function with JavaScript.
	*/
	js::Function require(js::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function evalFile(File)"
		@note Evaluated a JavaScript file's source code. This description is for using the function with JavaScript.
	*/
	js::Function evalFile(js::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function randRange(Min, Max) → Number"
		@note Generates a random (integer) number between a minimum and a maximum value. This description is for using the function with JavaScript.
	*/
	js::Function randRange(js::NativeContext Context);
}