
#pragma once
#include <bjs/js/Types.hpp>
#include <bjs/js/Module.hpp>
#include <bjs/js/Handlers.hpp>
#include <bjs/js/JavaScript.hpp>
#include <string>

namespace bjs::js
{
    	/// Class representing a global object. This should just be only instantiated on Global static variable.
	class GlobalObject
	{
		public:
			/**
				@brief Creates a global object wrapper for the given context.
				@param Context The context to use.
			*/
			GlobalObject(NativeContext Context);
			/**
				@brief Adds a variable containing a std::string to the global object.
				@param Name the variable has.
				@param Value the std::string value of the variable.
			*/
			void PushString(std::string Name, std::string Value);
			
			/**
				@brief Adds a variable containing a signed integer to the global object.
				@param Name the variable has.
				@param Value the signed integer value of the variable.
			*/
			void PushInt(std::string Name, s64 Value);

			/**
				@brief Adds a variable containing an unsigned integer to the global object.
				@param Name the variable has.
				@param Value the unsigned integer value of the variable.
			*/
			void PushUInt(std::string Name, u64 Value);

			/**
				@brief Adds a variable containing a double to the global object.
				@param Name the variable has.
				@param Value the double value of the variable.
			*/
			void PushDouble(std::string Name, double Value);
			
			/**
				@brief Adds a variable containing a boolean to the global object.
				@param Name the variable has.
				@param Value the boolean value of the variable.
			*/
			void PushBoolean(std::string Name, bool Value);
			
			/**
				@brief Adds a variable containing a function to the global object.
				@param Name the variable has.
				@param Value the function value of the variable. Check NativeFunction for more information about API JS functions.
			*/
			void PushFunction(std::string Name, NativeFunction Value);
			
			/**
				@brief Adds a variable containing an undefined value to the global object.
				@param Name the undefined variable has.
			*/
			void PushUndefined(std::string Name);

			/**
				@brief Adds a variable containing a null value to the global object.
				@param Name the null variable has.
			*/
			void PushNull(std::string Name);

			/**
				@brief Adds a variable containing a NaN value to the global object.
				@param Name the NaN variable has.
			*/
			void PushNaN(std::string Name);
			
			/**
				@brief Adds a module to the global object. It will be created as an object with the module's name.
				@param Value The module to create.
				@note This is not the way to add modules to require function, check, addModule.
			*/
			void PushModule(Module &Value);

			void InitializePushingObject(std::string Name, Object &Value);
			void FinalizePushingObject();
			NativeContext GetContext();
		private:
			std::string tempname;
			NativeContext ctx;
	};

	void AddModule(js::Module &NewModule);
    std::vector<js::Module> GetModules();
	void ClearModules();

    /**
		@brief [Global] API JS Function: "function require(ModuleName) → Object"
		@note Imports (attempts to import) a module. This description is for using the function with JavaScript.
	*/
	js::Function require(js::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function evalFile(File)"
		@note Evaluates a JavaScript file's source code. This description is for using the function with JavaScript.
	*/
	js::Function evalFile(js::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function randRange(Min, Max) → Number"
		@note Generates a random (integer) number between a minimum and a maximum value. This description is for using the function with JavaScript.
	*/
	js::Function randRange(js::NativeContext Context);

	js::GlobalObject InitializeGlobal(js::NativeContext Context);
}