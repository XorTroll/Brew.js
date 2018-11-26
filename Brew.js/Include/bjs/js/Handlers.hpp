
#pragma once
#include <bjs/js/Types.hpp>
#include <bjs/js/JavaScript.hpp>
#include <string>

namespace bjs::js
{
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
				@brief Same as CheckArgcEqualOrBigger, but only if they're equal (not bigger).
				@param Argc The desired number of passed arguments.
				@return If the argument number is equal.
			*/
			bool CheckArgcEqual(u32 Argc);

			/**
				@brief Gets the type of the argument located at the given position. If there's no argument, None is returned.
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
				@brief Gets the argument located at the given position as a std::string.
				@param Index The position of the argument.
				@return The argument as a std::string.
			*/
			std::string GetString(u32 Index);
			
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
				@return The argument as a bjs::js::Callback.
			*/
			Callback GetCallback(u32 Index);

			/**
				@brief Gets the argument located at the given position as an object.
				@param Index The position of the argument.
				@return The argument as a bjs::js::Object.
			*/
			Object GetObject(u32 Index);

			/**
				@brief Gets the argument located at the given position as an array.
				@param Index The position of the argument.
				@return The argument as a bjs::js::Array.
			*/
			Array GetArray(u32 Index);
			
			/**
				@brief Returns a std::string. Will only return the variable if the return code of the JS function is Variable.
				@param Value The std::string to return.
			*/
			void ReturnString(std::string Value);
			
			/**
				@brief Returns a signed integer. Will only return the variable if the return code of the JS function is Variable.
				@param Value The signed integer to return.
			*/
			void ReturnInt(s64 Value);
			
			/**
				@brief Returns an unsigned integer. Will only return the variable if the return code of the JS function is Variable.
				@param Value The unsigned integer to return.
			*/
			void ReturnUInt(u64 Value);
			
			/**
				@brief Returns a double. Will only return the variable if the return code of the JS function is Variable.
				@param Value The double to return.
			*/
			void ReturnDouble(double Value);

			/**
				@brief Returns a boolean. Will only return the variable if the return code of the JS function is Variable.
				@param Value The boolean to return.
			*/
			void ReturnBoolean(bool Value);

			/**
				@brief Returns an array. Will only return the variable if the return code of the JS function is Variable.
				@param Value The array to return.
				@note Add the elements to the array AFTER calling this function!
			*/
			void ReturnArray(Array &Value);

			/**
				@brief Returns an object. Will only return the variable if the return code of the JS function is Variable.
				@param Value The object to return.
				@note Add the elements to the object AFTER calling this function!
			*/
			void ReturnObject(Object &Value);

			/**
				@brief Returns undefined. Will only return the variable if the return code of the JS function is Variable.
			*/
			void ReturnUndefined();
			
			/**
				@brief Returns null. Will only return the variable if the return code of the JS function is Variable.
			*/
			void ReturnNull();

			/**
				@brief Returns NaN. Will only return the variable if the return code of the JS function is Variable.
			*/
			void ReturnNaN();
			NativeContext GetContext();
		protected:
			NativeContext ctx;
			int count;
	};

	/// An extended FunctionHandler to be used in bjs::js::Class - related API JS functions.
	class ClassHandler : public FunctionHandler
	{
		public:

			/**
				@brief Creates a ClassHandler.
				@param Context The context to use it with.
			*/
			ClassHandler(NativeContext Context);
			
			/**
				@brief Sets an internal property to a std::string. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The std::string value to set.
			*/
			void SetPropertyString(std::string Name, std::string Value);
			
			/**
				@brief Sets an internal property to a signed integer. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The signed integer value to set.
			*/
			void SetPropertyInt(std::string Name, s64 Value);
			
			/**
				@brief Sets an internal property to an unsigned integer. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The unsigned integer value to set.
			*/
			void SetPropertyUInt(std::string Name, u64 Value);
			
			/**
				@brief Sets an internal property to a double. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The double value to set.
			*/
			void SetPropertyDouble(std::string Name, double Value);
			
			/**
				@brief Sets an internal property to a boolean. Like doing "this.<Name> = <Value>;".
				@param Name The name of the property to set.
				@param Value The boolean value to set.
			*/
			void SetPropertyBoolean(std::string Name, bool Value);

			/**
				@brief Sets an internal property to undefined. Like doing "this.<Name> = undefined;".
				@param Name The name of the property to set to undefined.
			*/
			void SetPropertyUndefined(std::string Name);
			
			/**
				@brief Sets an internal property to null. Like doing "this.<Name> = null;".
				@param Name The name of the property to set to null.
			*/
			void SetPropertyNull(std::string Name);

			/**
				@brief Sets an internal property to NaN. Like doing "this.<Name> = NaN;".
				@param Name The name of the property to set to NaN.
			*/
			void SetPropertyNaN(std::string Name);

			void InitializeSettingPropertyArray(std::string Name, Array &Value);
			void FinalizeSettingPropertyArray();

			void InitializetSettingPropertyObject(std::string Name, Object &Value);
			void FinalizeSettingPropertyObject();
			
			/**
				@brief Gets an internal property as a std::string. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a std::string.
			*/
			std::string GetPropertyString(std::string Name);
			
			/**
				@brief Gets an internal property as a signed integer. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a signed integer.
			*/
			s64 GetPropertyInt(std::string Name);

			/**
				@brief Gets an internal property as an unsigned integer. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as an unsigned integer.
			*/
			u64 GetPropertyUInt(std::string Name);

			/**
				@brief Gets an internal property as a double. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a double.
			*/
			double GetPropertyDouble(std::string Name);

			/**
				@brief Gets an internal property as a boolean. Like getting "this.<Name>;".
				@param Name The name of the property to get.
				@return The property as a boolean.
			*/
			bool GetPropertyBoolean(std::string Name);
		private:
			int propcount;
			std::string tempname;
			int tempid;
	};
}