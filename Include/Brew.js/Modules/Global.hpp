
/**

    @file Global.hpp
    @brief Brew.js API - Global functions, variables, classes
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include "../API.hpp"

#include "Node/Process.hpp"
#if __curOS == 0
	#include "NX/Console.hpp"
#elif __curOS == 1
	#include "CTR/Console.hpp"
#elif __curOS == 2
	#include "NTR/Console.hpp"
#endif

namespace Brew
{
    namespace BuiltIn
    {   
        /**
			@brief [Global] API JS Function: "function require(ModuleName) → Object"
			@note Imports (tries to import) a module. This description is for using the function with JavaScript.
		*/
		API::Function require(API::NativeContext Context);
		API::Function evalFile(API::NativeContext Context);
		API::Function randRange(API::NativeContext Context);

        /**
            @brief Initializes global elements into the API.
            @param Context The context to laod the global elements to.
            @note This is internally called in the constructor of \ref Environment
        */
        void initGlobal(API::NativeContext Context);
    }
}