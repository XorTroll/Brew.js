
/**

	@file API.hpp
	@brief API of Brew.js, interaction between C++ ↔ JavaScript
	@author XorTroll
	@copyright Brew.js project

*/

#pragma once
#include "Detect.hpp"
#include "Include.hpp"
#include "Libs/Libs.hpp"

namespace Brew
{
	namespace API
	{	
		/// Represents the value which should be returned on an API JS function.
		typedef duk_ret_t Function;

		/// Represents a duktape context, the base of JavaScript evaluation.
		typedef duk_context *NativeContext;

		/// Represents an API JS function as a variable, for adding them to arrays, objects or modules.
		typedef duk_c_function NativeFunction;
		
		/// JavaScript types supported by duktape engine.
		enum Type
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
		enum Error
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
			Void = 0,                 ///< Void return: no variable (undefined) will be returned even if it's passed via a \ref FunctionHandler.
			Variable = 1,             ///< A variable is expected to be passed before the function returns this, via a \ref FunctionHandler.
			ErrorCode = -1,           ///< Similar to throwing error \ref CommonError.
			EvalErrorCode = -2,       ///< Similar to throwing error \ref EvalError.
			RangeErrorCode = -3,      ///< Similar to throwing error \ref RangeError.
			ReferenceErrorCode = -4,  ///< Similar to throwing error \ref ReferenceError.
			SyntaxErrorCode = -5,     ///< Similar to throwing error \ref SyntaxError.
			TypeErrorCode = -6,       ///< Similar to throwing error \ref TypeError.
			URIErrorCode = -7,        ///< Similar to throwing error \ref URIError.
		};

		class Object;
		
		/// Represents a JavaScript array. Usually used in \ref FunctionHandler.
		class Array
		{
			public:

				/**
				    @brief Creates a new array object with the given context.
					@param Context The context to use with the array.
				*/
				Array(NativeContext Context);

				/**
				    @brief Initializes internally the array.
					@note This should NOT be CALLED manually, as pushing it to \ref FunctionHandler actually calls this.
				*/
				void init();
				
				/**
				    @brief Adds (pushes) a string to the end of the array.
					@param Value The string value to add.
				*/
				void addString(string Value);
				
				/**
				    @brief Adds (pushes) a signed integer to the end of the array.
					@param Value The signed integer value to add.
				*/
				void addInt(s64 Value);
				
				/**
				    @brief Adds (pushes) an unsigned integer to the end of the array.
					@param Value The unsigned integer value to add.
				*/
				void addUInt(u64 Value);
				
				/**
				    @brief Adds (pushes) a double to the end of the array.
					@param Value The double value to add.
				*/
				void addDouble(double Value);
				
				/**
				    @brief Adds (pushes) a boolean to the end of the array.
					@param Value The boolean value to add.
				*/
				void addBoolean(bool Value);

				/**
				    @brief Adds (pushes) an API JS function to the end of the array.
					@param Value The function to add.
				*/
				void addFunction(NativeFunction Value);
				
				/**
				    @brief Adds (pushes) an undefined value to the end of the array.
				*/
				void addUndefined();
				
				/**
				    @brief Adds (pushes) a null value to the end of the array.
				*/
				void addNull();

				/**
				    @brief Adds (pushes) a NaN value to the end of the array.
				*/
				void addNaN();

				/**
				    @brief Finishes the array (closes it).
					@note This should NOT be CALLED if using it from a \ref FunctionHandler. 
				*/
				void end();
			private:
				NativeContext Context;
				duk_idx_t arridx;
				u32 idx;
				bool start;
		};
		
		/// Represents a JavaScript object. Usually used in \ref FunctionHandler.
		class Object
		{
			public:

				/**
				    @brief Creates a new object with the given context.
					@param Context The context to use with the array.
				*/
				Object(NativeContext Context);

				/**
				    @brief Initializes internally the array.
					@note This should NOT be CALLED manually, as pushing it to \ref FunctionHandler actually calls this.
				*/
				void init();

				/**
				    @brief Adds (pushes) a string with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The string value to add.
				*/
				void addString(string Name, string Value);
				
				/**
				    @brief Adds (pushes) a signed integer with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The signed integer value to add.
				*/
				void addInt(string Name, s64 Value);
				
				/**
				    @brief Adds (pushes) an unsigned integer with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The unsigned integer value to add.
				*/
				void addUInt(string Name, u64 Value);

				/**
				    @brief Adds (pushes) a double with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The double value to add.
				*/
				void addDouble(string Name, double Value);
				
				/**
				    @brief Adds (pushes) a boolean with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The boolean value to add.
				*/
				void addBoolean(string Name, bool Value);
				
				/**
				    @brief Adds (pushes) an API JS function with the specified property name to the object.
					@param Name The name the value will have.
					@param Value The function to add.
				*/
				void addFunction(string Name, NativeFunction Value);
				
				/**
				    @brief Adds (pushes) an undefined value with the specified property name to the object.
					@param Name The name the undefined will have.
				*/
				void addUndefined(string Name);
				
				/**
				    @brief Adds (pushes) a null value with the specified property name to the object.
					@param Name The name the null will have.
				*/
				void addNull(string Name);

				/**
				    @brief Adds (pushes) a NaN value with the specified property name to the object.
					@param Name The name the NaN will have.
				*/
				void addNaN(string Name);

				/**
				    @brief Finishes the object (closes it).
					@note This should NOT be CALLED if using it from a \ref FunctionHandler. 
				*/
				void end();
			private:
				NativeContext Context;
				duk_idx_t objidx;
				bool start;
		};
		
		/// Class representing a callback, which is a function as an object. To be used inside API JS functions/methods.
		class Callback
		{
			public:

				/**
				    @brief Gets the callback function from the given index using the given context. If the argument is not a function throws an error.
					@param Context The context to use it with.
					@param Index The index of the argument of the API JS function.
					@note This should NOT be CALLED manually. use \ref FunctionHandler::getCallback(...) to get it.
				*/
				Callback(NativeContext Context, u32 Index);
				
				/**
				    @brief Passes a string as an argument for the callback function.
					@param Value The string to pass.
					@note The value is added as the next argument.
				*/
				void addArgumentString(string Value);
				
				/**
				    @brief Passes a signed integer as an argument for the callback function.
					@param Value The signed integer to pass.
					@note The value is added as the next argument.
				*/
				void addArgumentInt(s64 Value);

				/**
				    @brief Passes an unsigned integer as an argument for the callback function.
					@param Value The unsigned integer to pass.
					@note The value is added as the next argument.
				*/
				void addArgumentUInt(u64 Value);

				/**
				    @brief Passes a double as an argument for the callback function.
					@param Value The double to pass.
					@note The value is added as the next argument.
				*/
				void addArgumentDouble(double Value);

				/**
				    @brief Passes a boolean as an argument for the callback function.
					@param Value The boolean to pass.
					@note The value is added as the next argument.
				*/
				void addArgumentBoolean(bool Value);

				/**
				    @brief Passes undefined as an argument for the callback function.
					@note The value is added as the next argument.
				*/
				void addArgumentUndefined();
				
				/**
				    @brief Passes null as an argument for the callback function.
					@note The value is added as the next argument.
				*/
				void addArgumentNull();

				/**
				    @brief Passes NaN as an argument for the callback function.
					@note The value is added as the next argument.
				*/
				void addArgumentNaN();
				void callFunction();
				void callFunctionNew();
			private:
				NativeContext Context;
				map<u32, string> Strings;
				map<u32, s64> Ints;
				map<u32, u64> UInts;
				map<u32, double> Doubles;
				map<u32, bool> Booleans;
				vector<u32> Undefineds;
				vector<u32> Nulls;
				vector<u32> NaNs;
				vector<u32> reg;
				u32 iter;
		};

		/// Class for handling API JS functions. It should be instantiated in API JS functions.
		class FunctionHandler
		{
			public:

				/**
					@brief Creates a function handler using the given context.
					@param Context The JS context being used.
				*/
				FunctionHandler(NativeContext Context);

				/**
					@brief Gets the number of arguments passed from JS to the function.
					@return The number of passed arguments.
				*/
				u32 getArgc();
				
				/**
					@brief Checks if the passed argument number are equal OR BIGGER than the specified number.
					@param Argc The desired number of passed arguments.
					@return If the argument number is equal or bigger.
				*/
				bool checkArgc(u32 Argc);
				
				/**
					@brief Same as \ref checkArgc, but only if they're equal (not bigger).
					@param Argc The desired number of passed arguments.
					@return If the argument number is equal.
				*/
				bool checkArgcEqual(u32 Argc);

				/**
					@brief Gets the type of the argument located at the given position. If there's no argument, \ref None is returned.
					@param Index The position of the argument.
					@return The type of the argument.
				*/
				Type getArgType(u32 Index);
				
				/**
					@brief Checks if the type of the argument located at the given position is a specific type.
					@param Index The position of the argument.
					@param ArgType The type to check with.
					@return The type of the argument.
				*/
				bool checkArgType(u32 Index, Type ArgType);

				/**
					@brief Checks if the the function was called as a constructor call.
					@return Whether it's a constructor call or not.
					@note `func()` is a normal call, while `new func()` is a constructor call This is a wrapper for duktape's `duk_is_constructor_call(ctx)`.
				*/
				bool isConstructorCall();
				
				/**
					@brief Gets the argument located at the given position as a string.
					@param Index The position of the argument.
					@return The argument as a string.
				*/
				string getString(u32 Index);
				
				/**
					@brief Gets the argument located at the given position as a signed integer.
					@param Index The position of the argument.
					@return The argument as a signed integer.
				*/
				s64 getInt(u32 Index);

				/**
					@brief Gets the argument located at the given position as an unsigned integer.
					@param Index The position of the argument.
					@return The argument as an unsigned integer.
				*/
				u64 getUInt(u32 Index);

				/**
					@brief Gets the argument located at the given position as a double.
					@param Index The position of the argument.
					@return The argument as a double.
				*/
				double getDouble(u32 Index);

				/**
					@brief Gets the argument located at the given position as a boolean.
					@param Index The position of the argument.
					@return The argument as a boolean.
				*/
				bool getBoolean(u32 Index);

				Callback getCallback(u32 Index);
				
				/**
					@brief Returns a string. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The string to return.
				*/
				void pushString(string Value);
				
				/**
					@brief Returns a signed integer. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The signed integer to return.
				*/
				void pushInt(s64 Value);
				
				/**
					@brief Returns an unsigned integer. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The unsigned integer to return.
				*/
				void pushUInt(u64 Value);
				
				/**
					@brief Returns a double. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The double to return.
				*/
				void pushDouble(double Value);

				/**
					@brief Returns a boolean. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The boolean to return.
				*/
				void pushBoolean(bool Value);

				/**
					@brief Returns an array. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The array to return.
					@note Add the elements to the array AFTER calling this function!
				*/
				void pushArray(Array Value);

				/**
					@brief Returns an object. Will only return the variable if the return code of the JS function is \ref Variable.
					@param Value The object to return.
					@note Add the elements to the object AFTER calling this function!
				*/
				void pushObject(Object Value);

				/**
					@brief Returns undefined. Will only return the variable if the return code of the JS function is \ref Variable.
				*/
				void pushUndefined();
				
				/**
					@brief Returns null. Will only return the variable if the return code of the JS function is \ref Variable.
				*/
				void pushNull();

				/**
					@brief Returns NaN. Will only return the variable if the return code of the JS function is \ref Variable.
				*/
				void pushNaN();

				/**
					@brief Aborts the execution throwing an error.
					@param ErrorType The error to throw.
					@param Message the message of the thrown error.
				*/
				void throwError(Error ErrorType, string Message);

			protected:
				NativeContext Context;
				int count;
		};

		class ClassHandler : public FunctionHandler
		{
			public:
				ClassHandler(NativeContext Context);
				void setPropertyString(string Name, string Value);
				void setPropertyInt(string Name, s64 Value);
				void setPropertyUInt(string Name, u64 Value);
				void setPropertyDouble(string Name, double Value);
				void setPropertyBoolean(string Name, bool Value);
				void setPropertyUndefined(string Name);
				void setPropertyNull(string Name);
				void setPropertyNaN(string Name);
				string getPropertyString(string Name);
				s64 getPropertyInt(string Name);
				u64 getPropertyUInt(string Name);
				double getPropertyDouble(string Name);
				bool getPropertyBoolean(string Name);
			private:
				int propcount;
		};

		struct Class
		{
			Class(string Name, NativeFunction Constructor);
			void addString(string Name, string Value);
			void addInt(string Name, s64 Value);
			void addUInt(string Name, u64 Value);
			void addDouble(string Name, double Value);
			void addBoolean(string Name, bool Value);
			void addFunction(string Name, NativeFunction Value);
			void addUndefined(string Name);
			void addNull(string Name);
			void addNaN(string Name);
			NativeFunction Constructor;
			map<string, string> Strings;
			map<string, s64> Ints;
			map<string, u64> UInts;
			map<string, double> Doubles;
			map<string, bool> Booleans;
			map<string, NativeFunction> Functions;
			vector<string> Undefineds;
			vector<string> Nulls;
			vector<string> NaNs;
			string Name;
		};

		/// Class holding a module which can be added and required via Brew.js API.
		struct Module
		{
			/**
				@brief Creates a module from a name.
				@param Module The name the module will have. It will be accessed via that name if it's added to the module list.
			*/
			Module(string Module);

			/**
				@brief Adds a variable containing a string to the module.
				@param Name the variable has.
				@param Value the string value of the variable.
			*/
			void pushString(string Name, string Value);
			
			/**
				@brief Adds a variable containing a signed integer to the module.
				@param Name the variable has.
				@param Value the signed integer value of the variable.
			*/
			void pushInt(string Name, s64 Value);

			/**
				@brief Adds a variable containing an unsigned integer to the module.
				@param Name the variable has.
				@param Value the unsigned integer value of the variable.
			*/
			void pushUInt(string Name, u64 Value);

			/**
				@brief Adds a variable containing a double to the module.
				@param Name the variable has.
				@param Value the double value of the variable.
			*/
			void pushDouble(string Name, double Value);
			
			/**
				@brief Adds a variable containing a boolean to the module.
				@param Name the variable has.
				@param Value the boolean value of the variable.
			*/
			void pushBoolean(string Name, bool Value);
			
			/**
				@brief Adds a variable containing a function to the module.
				@param Name the variable has.
				@param Value the function value of the variable. Check \ref NativeFunction for more information about API JS functions.
			*/
			void pushFunction(string Name, NativeFunction Value);
			
			/**
				@brief Adds a variable containing an undefined value to the module.
				@param Name the undefined variable has.
			*/
			void pushUndefined(string Name);

			/**
				@brief Adds a variable containing a null value to the module.
				@param Name the null variable has.
			*/
			void pushNull(string Name);

			/**
				@brief Adds a variable containing a NaN value to the module.
				@param Name the NaN variable has.
			*/
			void pushNaN(string Name);
			void pushClass(Class Value);
			map<string, string> Strings;
			map<string, s64> Ints;
			map<string, u64> UInts;
			map<string, double> Doubles;
			map<string, bool> Booleans;
			map<string, NativeFunction> Functions;
			vector<string> Undefineds;
			vector<string> Nulls;
			vector<string> NaNs;
			vector<Class> Classes;
			string Name;
		};

		/// Class representing a global object. This should just be only instantiated on Global static variable.
		class GlobalObject
		{
			public:
				/**
					@brief Creates a global object wrapper for the given context.
					@param Value The context to use.
				*/
				GlobalObject(NativeContext Context);
				/**
					@brief Adds a variable containing a string to the global object.
					@param Name the variable has.
					@param Value the string value of the variable.
				*/
				void pushString(string Name, string Value);
				
				/**
					@brief Adds a variable containing a signed integer to the global object.
					@param Name the variable has.
					@param Value the signed integer value of the variable.
				*/
				void pushInt(string Name, s64 Value);

				/**
					@brief Adds a variable containing an unsigned integer to the global object.
					@param Name the variable has.
					@param Value the unsigned integer value of the variable.
				*/
				void pushUInt(string Name, u64 Value);

				/**
					@brief Adds a variable containing a double to the global object.
					@param Name the variable has.
					@param Value the double value of the variable.
				*/
				void pushDouble(string Name, double Value);
				
				/**
					@brief Adds a variable containing a boolean to the global object.
					@param Name the variable has.
					@param Value the boolean value of the variable.
				*/
				void pushBoolean(string Name, bool Value);
				
				/**
					@brief Adds a variable containing a function to the global object.
					@param Name the variable has.
					@param Value the function value of the variable. Check \ref NativeFunction for more information about API JS functions.
				*/
				void pushFunction(string Name, NativeFunction Value);
				
				/**
					@brief Adds a variable containing an undefined value to the global object.
					@param Name the undefined variable has.
				*/
				void pushUndefined(string Name);

				/**
					@brief Adds a variable containing a null value to the global object.
					@param Name the null variable has.
				*/
				void pushNull(string Name);

				/**
					@brief Adds a variable containing a NaN value to the global object.
					@param Name the NaN variable has.
				*/
				void pushNaN(string Name);
				
				/**
					@brief Adds a module to the global object. It will be created as an object with the module's name.
					@param Value The module to create.
					@note This is not the way to add modules to \ref require function, check, \ref addModule.
				*/
				void pushModule(Module Module);

			private:
				NativeContext Context;
		};
		
		/**
			@brief Creates a context using default heap.
			@note This is a wrapper for duktape's "duk_create_heap_default" function.
			@return The created context.
		*/
		NativeContext createContext();
		
		/**
			@brief Adds a module to the module list, so it can be included via \ref require function.
			@param Module The module to add to the module list.
			@note If there's already another module with the same name, then the module won't be added.
		*/
		void addModule(Module Module);
		
		/**
			@brief [Global] API JS Function: "function require(ModuleName) → Object"
			@note Imports (tries to import) a module. This description is for using the function with JavaScript.
		*/
		Function require(NativeContext Context);
		Function evalFile(NativeContext Context);
		Function randRange(NativeContext Context);
		
		/**
			@brief This function adds basic API (require function, global objects like 'console') to the \ref Global object of Brew.js.
			@param Context The context to use with.
			@note This function is called internally when creating a new \ref Environment, so it should not be called anywhere else.
		*/
		void initializeGlobalObject(NativeContext Context);

		/// Represents the global object of Brew.js API. Add manually variable to the global object via this class.
		static GlobalObject Global(NULL);

		/// The list of modules which will be included when creating a Environment.
		static vector<Module> Modules;

		/// Current version of Brew.js (it's on API.hpp to be accessed from built-in modules)
		static string Version = "2.0 beta 1";
	}
}

#if __curOS == 0

	#include "Modules/NX/Console.hpp"

#elif __curOS == 1

	#include "Modules/CTR/Console.hpp"

#elif __curOS == 2

	#include "Modules/NTR/Console.hpp"

#endif