
/**

	@file API.hpp
	@brief API of Brew.js, interaction between C++ ↔ JavaScript
	@author XorTroll
	@copyright Brew.js project

*/

#pragma once
#include <Brew.js/Detect.hpp>
#include <Brew.js/Include.hpp>
#include <Brew.js/Libs/Libs.hpp>

namespace Brew::API
{
	/// Represents the value which should be returned on an API JS function.
	typedef duk_ret_t Function;

	/// Represents a duktape context, the base of JavaScript evaluation.
	typedef duk_context *NativeContext;

	/// Represents an API JS function as a variable, for adding them to arrays, objects or modules.
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
				@brief Creates a new array object with the given context and from the specifiec argument index.
				@param Context The context to use with the array.
				@param Index The argument index to get it from.
				@note Better to call \ref FunctionHandler.getArray(...) instead!
			*/
			Array(NativeContext Context, u32 Index);

			/**
				@brief Initializes internally the array.
				@note This should NOT be CALLED manually, as pushing it to \ref FunctionHandler actually calls this.
			*/
			void Initialize();
			
			/**
				@brief Adds (pushes) a string to the end of the array.
				@param Value The string value to add.
			*/
			void AddString(string Value);
			
			/**
				@brief Adds (pushes) a signed integer to the end of the array.
				@param Value The signed integer value to add.
			*/
			void AddInt(s64 Value);
			
			/**
				@brief Adds (pushes) an unsigned integer to the end of the array.
				@param Value The unsigned integer value to add.
			*/
			void AddUInt(u64 Value);
			
			/**
				@brief Adds (pushes) a double to the end of the array.
				@param Value The double value to add.
			*/
			void AddDouble(double Value);
			
			/**
				@brief Adds (pushes) a boolean to the end of the array.
				@param Value The boolean value to add.
			*/
			void AddBoolean(bool Value);

			/**
				@brief Adds (pushes) an API JS function to the end of the array.
				@param Value The function to add.
			*/
			void AddFunction(NativeFunction Value);
			
			/**
				@brief Adds (pushes) an undefined value to the end of the array.
			*/
			void AddUndefined();
			
			/**
				@brief Adds (pushes) a null value to the end of the array.
			*/
			void AddNull();

			/**
				@brief Adds (pushes) a NaN value to the end of the array.
			*/
			void AddNaN();

			/**
				@brief Finishes the array (closes it).
				@note This should NOT be CALLED if using it from a \ref FunctionHandler. 
			*/
			void Close();

			/**
				@brief Gets the argument at given index as a string. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as a string.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			string GetString(u32 Index);
			
			/**
				@brief Gets the argument at given index as a signed integer. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as a signed integer.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			s64 GetInt(u32 Index);

			/**
				@brief Gets the argument at given index as an unsigned integer. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as an unsigned integer.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			u64 GetUInt(u32 Index);

			/**
				@brief Gets the argument at given index as a double. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as a double.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			double GetDouble(u32 Index);

			/**
				@brief Gets the argument at given index as a boolean. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as a boolean.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			bool GetBoolean(u32 Index);

			/**
				@brief Gets the argument at given index as an array. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as an array.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			Array GetArray(u32 Index);

			/**
				@brief Gets the argument at given index as an object. No conversions are done to get it.
				@param Index The index to get the argument from.
				@return The argument as an object.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			Object GetObject(u32 Index);

			/**
				@brief Gets the JSON string representation of this array.
				@return The JSON representation as a string.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			string AsJSON();
			int GetId();

		private:
			NativeContext Context;
			duk_idx_t arridx;
			u32 idx;
			bool start;
			bool padd;
			u32 uidx;
	};
	
	/// Represents a JavaScript object. Usually used in \ref FunctionHandler.
	class Object
	{
		public:

			/**
				@brief Creates a new object with the given context.
				@param Context The context to use with the object.
			*/
			Object(NativeContext Context);

			/**
				@brief Creates a new object object with the given context and from the specifiec argument index.
				@param Context The context to use with the object.
				@param Index The argument index to get it from.
				@note Better to call \ref FunctionHandler.getObject(...) instead!
			*/
			Object(NativeContext Context, u32 Index);

			/**
				@brief Initializes internally the array.
				@note This should NOT be CALLED manually, as pushing it to \ref FunctionHandler actually calls this.
			*/
			void Initialize();

			/**
				@brief Adds (pushes) a string with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The string value to add.
			*/
			void AddString(string Name, string Value);
			
			/**
				@brief Adds (pushes) a signed integer with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The signed integer value to add.
			*/
			void AddInt(string Name, s64 Value);
			
			/**
				@brief Adds (pushes) an unsigned integer with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The unsigned integer value to add.
			*/
			void AddUInt(string Name, u64 Value);

			/**
				@brief Adds (pushes) a double with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The double value to add.
			*/
			void AddDouble(string Name, double Value);
			
			/**
				@brief Adds (pushes) a boolean with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The boolean value to add.
			*/
			void AddBoolean(string Name, bool Value);
			
			/**
				@brief Adds (pushes) an API JS function with the specified property name to the object.
				@param Name The name the value will have.
				@param Value The function to add.
			*/
			void AddFunction(string Name, NativeFunction Value);
			
			/**
				@brief Adds (pushes) an undefined value with the specified property name to the object.
				@param Name The name the undefined will have.
			*/
			void AddUndefined(string Name);
			
			/**
				@brief Adds (pushes) a null value with the specified property name to the object.
				@param Name The name the null will have.
			*/
			void AddNull(string Name);

			/**
				@brief Adds (pushes) a NaN value with the specified property name to the object.
				@param Name The name the NaN will have.
			*/
			void AddNaN(string Name);

			void InitializeAddingArray(string Name, Array &Value);
			void FinalizeAddingArray();

			void InitializeAddingObject(string Name, Object &Value);
			void FinalizeAddingObject();

			/**
				@brief Finishes the object (closes it).
				@note This should NOT be CALLED if using it from a \ref FunctionHandler. 
			*/
			void Close();

			/**
				@brief Gets the property named as the given name as a string. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as a string.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			string GetString(string Name);

			/**
				@brief Gets the property named as the given name as a signed integer. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as a signed integer.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			s64 GetInt(string Name);

			/**
				@brief Gets the property named as the given name as an unsigned integer. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as an unsigned integer.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			u64 GetUInt(string Name);
			
			/**
				@brief Gets the property named as the given name as a double. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as a double.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			double GetDouble(string Name);

			/**
				@brief Gets the property named as the given name as a boolean. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as a boolean.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			bool GetBoolean(string Name);

			/**
				@brief Gets the property named as the given name as an array. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as an array.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			Array GetArray(string Name);

			/**
				@brief Gets the property named as the given name as an object. No conversions are done to get it.
				@param Name The name of the property.
				@return The property as an object.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			Object GetObject(string Name);

			/**
				@brief Gets the JSON string representation of this object.
				@return The JSON representation as a string.
				@note This should ONLY be CALLED if using it from a \ref FunctionHandler. 
			*/
			string AsJSON();
			int GetId();

		private:
			NativeContext Context;
			duk_idx_t objidx;
			string tempname;
			bool start;
			bool padd;
			u32 idx;
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
			void AddString(string Value);
			
			/**
				@brief Passes a signed integer as an argument for the callback function.
				@param Value The signed integer to pass.
				@note The value is added as the next argument.
			*/
			void AddInt(s64 Value);

			/**
				@brief Passes an unsigned integer as an argument for the callback function.
				@param Value The unsigned integer to pass.
				@note The value is added as the next argument.
			*/
			void AddUInt(u64 Value);

			/**
				@brief Passes a double as an argument for the callback function.
				@param Value The double to pass.
				@note The value is added as the next argument.
			*/
			void AddDouble(double Value);

			/**
				@brief Passes a boolean as an argument for the callback function.
				@param Value The boolean to pass.
				@note The value is added as the next argument.
			*/
			void AddBoolean(bool Value);

			/**
				@brief Passes undefined as an argument for the callback function.
				@note The value is added as the next argument.
			*/
			void AddUndefined();
			
			/**
				@brief Passes null as an argument for the callback function.
				@note The value is added as the next argument.
			*/
			void AddNull();

			/**
				@brief Passes NaN as an argument for the callback function.
				@note The value is added as the next argument.
			*/
			void AddNaN();
			
			/**
				@brief Calls the function with currently added arguments (in order). Like doing "Func(Args);".
				@note This should be called after adding all the arguments in order.
			*/
			void Call();
			
			/**
				@brief Like \ref Call, but like calling it with "new" keyword. Like doing "new Func(Args);".
				@note This should be called after adding all the arguments in order.
			*/
			void CallNew();
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
			u32 GetArgc();
			
			/**
				@brief Checks if the passed argument number are equal OR BIGGER than the specified number.
				@param Argc The desired number of passed arguments.
				@return If the argument number is equal or bigger.
			*/
			bool CheckArgcEqualOrBigger(u32 Argc);
			
			/**
				@brief Same as \ref CheckArgcEqualOrBigger, but only if they're equal (not bigger).
				@param Argc The desired number of passed arguments.
				@return If the argument number is equal.
			*/
			bool CheckArgcEqual(u32 Argc);

			/**
				@brief Gets the type of the argument located at the given position. If there's no argument, \ref None is returned.
				@param Index The position of the argument.
				@return The type of the argument.
			*/
			Type GetArgumentType(u32 Index);
			
			/**
				@brief Checks if the type of the argument located at the given position is a specific type.
				@param Index The position of the argument.
				@param ArgType The type to check with.
				@return The type of the argument.
			*/
			bool CheckArgumentType(u32 Index, Type ArgType);

			/**
				@brief Checks if the the function was called as a constructor call.
				@return Whether it's a constructor call or not.
				@note `func()` is a normal call, while `new func()` is a constructor call This is a wrapper for duktape's `duk_is_constructor_call(ctx)`.
			*/
			bool IsConstructorCall();
			
			/**
				@brief Gets the argument located at the given position as a string.
				@param Index The position of the argument.
				@return The argument as a string.
			*/
			string GetString(u32 Index);
			
			/**
				@brief Gets the argument located at the given position as a signed integer.
				@param Index The position of the argument.
				@return The argument as a signed integer.
			*/
			s64 GetInt(u32 Index);

			/**
				@brief Gets the argument located at the given position as an unsigned integer.
				@param Index The position of the argument.
				@return The argument as an unsigned integer.
			*/
			u64 GetUInt(u32 Index);

			/**
				@brief Gets the argument located at the given position as a double.
				@param Index The position of the argument.
				@return The argument as a double.
			*/
			double GetDouble(u32 Index);

			/**
				@brief Gets the argument located at the given position as a boolean.
				@param Index The position of the argument.
				@return The argument as a boolean.
			*/
			bool GetBoolean(u32 Index);

			/**
				@brief Gets the argument located at the given position as a callback (function as argument).
				@param Index The position of the argument.
				@return The argument as a \ref Brew::API::Callback.
			*/
			Callback GetCallback(u32 Index);

			/**
				@brief Gets the argument located at the given position as an object.
				@param Index The position of the argument.
				@return The argument as a \ref Brew::API::Object.
			*/
			Object GetObject(u32 Index);

			/**
				@brief Gets the argument located at the given position as an array.
				@param Index The position of the argument.
				@return The argument as a \ref Brew::API::Array.
			*/
			Array GetArray(u32 Index);
			
			/**
				@brief Returns a string. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The string to return.
			*/
			void ReturnString(string Value);
			
			/**
				@brief Returns a signed integer. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The signed integer to return.
			*/
			void ReturnInt(s64 Value);
			
			/**
				@brief Returns an unsigned integer. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The unsigned integer to return.
			*/
			void ReturnUInt(u64 Value);
			
			/**
				@brief Returns a double. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The double to return.
			*/
			void ReturnDouble(double Value);

			/**
				@brief Returns a boolean. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The boolean to return.
			*/
			void ReturnBoolean(bool Value);

			/**
				@brief Returns an array. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The array to return.
				@note Add the elements to the array AFTER calling this function!
			*/
			void ReturnArray(Array &Value);

			/**
				@brief Returns an object. Will only return the variable if the return code of the JS function is \ref Variable.
				@param Value The object to return.
				@note Add the elements to the object AFTER calling this function!
			*/
			void ReturnObject(Object &Value);

			/**
				@brief Returns undefined. Will only return the variable if the return code of the JS function is \ref Variable.
			*/
			void ReturnUndefined();
			
			/**
				@brief Returns null. Will only return the variable if the return code of the JS function is \ref Variable.
			*/
			void ReturnNull();

			/**
				@brief Returns NaN. Will only return the variable if the return code of the JS function is \ref Variable.
			*/
			void ReturnNaN();

		protected:
			NativeContext Context;
			int count;
	};

	/// An extended \ref FunctionHandler to be used in \ref Brew::API::Class - related API JS functions.
	class ClassHandler : public FunctionHandler
	{
		public:

			/**
				@brief Creates a \ref ClassHandler.
				@param Context The context to use it with.
			*/
			ClassHandler(NativeContext Context);
			
			/**
				@brief Sets an internal property to a string. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The string value to set.
			*/
			void SetPropertyString(string Name, string Value);
			
			/**
				@brief Sets an internal property to a signed integer. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The signed integer value to set.
			*/
			void SetPropertyInt(string Name, s64 Value);
			
			/**
				@brief Sets an internal property to an unsigned integer. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The unsigned integer value to set.
			*/
			void SetPropertyUInt(string Name, u64 Value);
			
			/**
				@brief Sets an internal property to a double. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The double value to set.
			*/
			void SetPropertyDouble(string Name, double Value);
			
			/**
				@brief Sets an internal property to a boolean. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The boolean value to set.
			*/
			void SetPropertyBoolean(string Name, bool Value);

			/**
				@brief Sets an internal property to undefined. Like doing "this.<Name> = undefined;".
				@param Name The name of the property to set to undefined.
			*/
			void SetPropertyUndefined(string Name);
			
			/**
				@brief Sets an internal property to null. Like doing "this.<Name> = null;".
				@param Name The name of the property to set to null.
			*/
			void SetPropertyNull(string Name);

			/**
				@brief Sets an internal property to NaN. Like doing "this.<Name> = NaN;".
				@param Name The name of the property to set to NaN.
			*/
			void SetPropertyNaN(string Name);

			void InitializeSettingPropertyArray(string Name, Array &Value);
			void FinalizeSettingPropertyArray();

			void InitializetSettingPropertyObject(string Name, Object &Value);
			void FinalizeSettingPropertyObject();
			
			/**
				@brief Gets an internal property as a string. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a string.
			*/
			string GetPropertyString(string Name);
			
			/**
				@brief Gets an internal property as a signed integer. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a signed integer.
			*/
			s64 GetPropertyInt(string Name);

			/**
				@brief Gets an internal property as an unsigned integer. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as an unsigned integer.
			*/
			u64 GetPropertyUInt(string Name);

			/**
				@brief Gets an internal property as a double. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a double.
			*/
			double GetPropertyDouble(string Name);

			/**
				@brief Gets an internal property as a boolean. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a boolean.
			*/
			bool GetPropertyBoolean(string Name);
		private:
			int propcount;
			string tempname;
			int tempid;
	};

	/// Represents a JavaScript class, which can be added to a module.
	struct Class
	{
		/**
			@brief Creates a class with the class name and the constructor function.
			@param Name The name of the class.
			@param Constructor The API JS function of the class constructor.
		*/
		Class(string Name, NativeFunction Constructor);

		/**
			@brief Adds a string as a property with the given name.
			@param Name The name of the property to set.
			@param Value The string to set as property.
			@note The property WON'T be static.
		*/
		void AddString(string Name, string Value);
		
		/**
			@brief Adds a signed integer as a property with the given name.
			@param Name The name of the property to set.
			@param Value The signed integer to set as property.
			@note The property WON'T be static.
		*/
		void AddInt(string Name, s64 Value);

		/**
			@brief Adds an unsigned integer as a property with the given name.
			@param Name The name of the property to set.
			@param Value The unsigned integer to set as property.
			@note The property WON'T be static.
		*/
		void AddUInt(string Name, u64 Value);

		/**
			@brief Adds a double as a property with the given name.
			@param Name The name of the property to set.
			@param Value The double to set as property.
			@note The property WON'T be static.
		*/
		void AddDouble(string Name, double Value);

		/**
			@brief Adds a boolean as a property with the given name.
			@param Name The name of the property to set.
			@param Value The boolean to set as property.
			@note The property WON'T be static.
		*/
		void AddBoolean(string Name, bool Value);

		/**
			@brief Adds an API JS function as a property with the given name.
			@param Name The name of the property to set.
			@param Value The API JS function to set as property.
			@note The property WON'T be static.
		*/
		void AddFunction(string Name, NativeFunction Value);

		/**
			@brief Adds undefined as a property with the given name.
			@param Name The name of the property to set to undefined.
			@note The property WON'T be static.
		*/
		void AddUndefined(string Name);
		
		/**
			@brief Adds null as a property with the given name.
			@param Name The name of the property to set to null.
			@note The property WON'T be static.
		*/
		void AddNull(string Name);

		/**
			@brief Adds NaN as a property with the given name.
			@param Name The name of the property to set to NaN.
			@note The property WON'T be static.
		*/
		void AddNaN(string Name);
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
		void PushString(string Name, string Value);
		
		/**
			@brief Adds a variable containing a signed integer to the module.
			@param Name the variable has.
			@param Value the signed integer value of the variable.
		*/
		void PushInt(string Name, s64 Value);

		/**
			@brief Adds a variable containing an unsigned integer to the module.
			@param Name the variable has.
			@param Value the unsigned integer value of the variable.
		*/
		void PushUInt(string Name, u64 Value);

		/**
			@brief Adds a variable containing a double to the module.
			@param Name the variable has.
			@param Value the double value of the variable.
		*/
		void PushDouble(string Name, double Value);
		
		/**
			@brief Adds a variable containing a boolean to the module.
			@param Name the variable has.
			@param Value the boolean value of the variable.
		*/
		void PushBoolean(string Name, bool Value);
		
		/**
			@brief Adds a variable containing a function to the module.
			@param Name the variable has.
			@param Value the function value of the variable. Check \ref NativeFunction for more information about API JS functions.
		*/
		void PushFunction(string Name, NativeFunction Value);
		
		/**
			@brief Adds a variable containing an undefined value to the module.
			@param Name the undefined variable has.
		*/
		void PushUndefined(string Name);

		/**
			@brief Adds a variable containing a null value to the module.
			@param Name the null variable has.
		*/
		void PushNull(string Name);

		/**
			@brief Adds a variable containing a NaN value to the module.
			@param Name the NaN variable has.
		*/
		void PushNaN(string Name);
		
		/**
			@brief Adds a class object to the module.
			@param Value The class object to add.
		*/
		void PushClass(Class &Value);
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
			void PushString(string Name, string Value);
			
			/**
				@brief Adds a variable containing a signed integer to the global object.
				@param Name the variable has.
				@param Value the signed integer value of the variable.
			*/
			void PushInt(string Name, s64 Value);

			/**
				@brief Adds a variable containing an unsigned integer to the global object.
				@param Name the variable has.
				@param Value the unsigned integer value of the variable.
			*/
			void PushUInt(string Name, u64 Value);

			/**
				@brief Adds a variable containing a double to the global object.
				@param Name the variable has.
				@param Value the double value of the variable.
			*/
			void PushDouble(string Name, double Value);
			
			/**
				@brief Adds a variable containing a boolean to the global object.
				@param Name the variable has.
				@param Value the boolean value of the variable.
			*/
			void PushBoolean(string Name, bool Value);
			
			/**
				@brief Adds a variable containing a function to the global object.
				@param Name the variable has.
				@param Value the function value of the variable. Check \ref NativeFunction for more information about API JS functions.
			*/
			void PushFunction(string Name, NativeFunction Value);
			
			/**
				@brief Adds a variable containing an undefined value to the global object.
				@param Name the undefined variable has.
			*/
			void PushUndefined(string Name);

			/**
				@brief Adds a variable containing a null value to the global object.
				@param Name the null variable has.
			*/
			void PushNull(string Name);

			/**
				@brief Adds a variable containing a NaN value to the global object.
				@param Name the NaN variable has.
			*/
			void PushNaN(string Name);
			
			/**
				@brief Adds a module to the global object. It will be created as an object with the module's name.
				@param Value The module to create.
				@note This is not the way to add modules to \ref require function, check, \ref addModule.
			*/
			void PushModule(Module &Value);

			void InitializePushingObject(string Name, Object &Value);
			void FinalizePushingObject();

		private:
			string tempname;
			NativeContext Context;
	};
	
	/**
		@brief Creates a context using default heap.
		@note This is a wrapper for duktape's "duk_create_heap_default" function.
		@return The created context.
	*/
	NativeContext CreateContext();
	
	/**
		@brief Adds a module to the module list, so it can be included via \ref require function.
		@param Module The module to add to the module list.
		@note If there's already another module with the same name, then the module won't be added.
	*/
	void AddModule(Module &Module);

	/**
		@brief Aborts the execution throwing an error.
		@param Context The context to throw the error on.
		@param ErrorType The error to throw.
		@param Message the message of the thrown error.
	*/
	void ThrowError(NativeContext Context, Error ErrorType, string Message);

	/// Represents the global object of Brew.js API. Add manually variable to the global object via this class.
	static GlobalObject Global(NULL);

	/// Current version of Brew.js (it's on API.hpp to be accessed from built-in modules)
	static const string Version = "2.0 (UNRELEASED)";

	/// The list of modules which will be included when creating a Environment.
	static vector<API::Module> Modules;

	/// Global object elements

	/**
		@brief [Global] API JS Function: "function require(ModuleName) → Object"
		@note Imports (tries to import) a module. This description is for using the function with JavaScript.
	*/
	Function require(API::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function evalFile(File)"
		@note Evaluated a JavaScript file's source code. This description is for using the function with JavaScript.
	*/
	Function evalFile(API::NativeContext Context);
	
	/**
		@brief [Global] API JS Function: "function randRange(Min, Max) → Number"
		@note Generates a random (integer) number between a minimum and a maximum value. This description is for using the function with JavaScript.
	*/
	API::Function randRange(API::NativeContext Context);

	/**
		@brief Initializes global elements into the API.
		@param Context The context to laod the global elements to.
		@note This is internally called in the constructor of \ref Environment
	*/
	void InitializeGlobalObject(API::NativeContext Context);
}

#include "Modules/Node/Process.hpp"
#if __curOS == 0
	#include "Modules/NX/Console.hpp"
#elif __curOS == 1
	#include "Modules/CTR/Console.hpp"
#elif __curOS == 2
	#include "Modules/NTR/Console.hpp"
#endif