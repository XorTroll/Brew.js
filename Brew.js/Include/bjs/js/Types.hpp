
#pragma once
#include <bjs/js/JavaScript.hpp>
#include <string>
#include <map>
#include <vector>

namespace bjs::js
{
    class Object;
    
    /// Represents a JavaScript array. Usually used in FunctionHandler.
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
                @note Better to call FunctionHandler.getArray(...) instead!
            */
            Array(NativeContext Context, u32 Index);

            /**
                @brief Initializes internally the array.
                @note This should NOT be CALLED manually, as pushing it to FunctionHandler actually calls this.
            */
            void Initialize();
            
            /**
                @brief Adds (pushes) a std::string to the end of the array.
                @param Value The std::string value to add.
            */
            void AddString(std::string Value);
            
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
                @note This should NOT be CALLED if using it from a FunctionHandler. 
            */
            void Close();

            /**
                @brief Gets the argument at given index as a std::string. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as a std::string.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            std::string GetString(u32 Index);
            
            /**
                @brief Gets the argument at given index as a signed integer. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as a signed integer.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            s64 GetInt(u32 Index);

            /**
                @brief Gets the argument at given index as an unsigned integer. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as an unsigned integer.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            u64 GetUInt(u32 Index);

            /**
                @brief Gets the argument at given index as a double. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as a double.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            double GetDouble(u32 Index);

            /**
                @brief Gets the argument at given index as a boolean. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as a boolean.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            bool GetBoolean(u32 Index);

            /**
                @brief Gets the argument at given index as an array. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as an array.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            Array GetArray(u32 Index);

            /**
                @brief Gets the argument at given index as an object. No conversions are done to get it.
                @param Index The index to get the argument from.
                @return The argument as an object.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            Object GetObject(u32 Index);

            /**
                @brief Gets the JSON std::string representation of this array.
                @return The JSON representation as a std::string.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            std::string AsJSON();
            int GetId();
            NativeContext GetContext();
        private:
            NativeContext ctx;
            duk_idx_t arridx;
            u32 idx;
            bool start;
            bool padd;
            u32 uidx;
    };
    
    /// Represents a JavaScript object. Usually used in FunctionHandler.
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
                @note Better to call FunctionHandler.getObject(...) instead!
            */
            Object(NativeContext Context, u32 Index);

            /**
                @brief Initializes internally the array.
                @note This should NOT be CALLED manually, as pushing it to FunctionHandler actually calls this.
            */
            void Initialize();

            /**
                @brief Adds (pushes) a std::string with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The std::string value to add.
            */
            void AddString(std::string Name, std::string Value);
            
            /**
                @brief Adds (pushes) a signed integer with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The signed integer value to add.
            */
            void AddInt(std::string Name, s64 Value);
            
            /**
                @brief Adds (pushes) an unsigned integer with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The unsigned integer value to add.
            */
            void AddUInt(std::string Name, u64 Value);

            /**
                @brief Adds (pushes) a double with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The double value to add.
            */
            void AddDouble(std::string Name, double Value);
            
            /**
                @brief Adds (pushes) a boolean with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The boolean value to add.
            */
            void AddBoolean(std::string Name, bool Value);
            
            /**
                @brief Adds (pushes) an API JS function with the specified property name to the object.
                @param Name The name the value will have.
                @param Value The function to add.
            */
            void AddFunction(std::string Name, NativeFunction Value);
            
            /**
                @brief Adds (pushes) an undefined value with the specified property name to the object.
                @param Name The name the undefined will have.
            */
            void AddUndefined(std::string Name);
            
            /**
                @brief Adds (pushes) a null value with the specified property name to the object.
                @param Name The name the null will have.
            */
            void AddNull(std::string Name);

            /**
                @brief Adds (pushes) a NaN value with the specified property name to the object.
                @param Name The name the NaN will have.
            */
            void AddNaN(std::string Name);

            void InitializeAddingArray(std::string Name, Array &Value);
            void FinalizeAddingArray();

            void InitializeAddingObject(std::string Name, Object &Value);
            void FinalizeAddingObject();

            /**
                @brief Finishes the object (closes it).
                @note This should NOT be CALLED if using it from a FunctionHandler. 
            */
            void Close();

            /**
                @brief Gets the property named as the given name as a std::string. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as a std::string.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            std::string GetString(std::string Name);

            /**
                @brief Gets the property named as the given name as a signed integer. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as a signed integer.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            s64 GetInt(std::string Name);

            /**
                @brief Gets the property named as the given name as an unsigned integer. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as an unsigned integer.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            u64 GetUInt(std::string Name);
            
            /**
                @brief Gets the property named as the given name as a double. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as a double.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            double GetDouble(std::string Name);

            /**
                @brief Gets the property named as the given name as a boolean. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as a boolean.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            bool GetBoolean(std::string Name);

            /**
                @brief Gets the property named as the given name as an array. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as an array.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            Array GetArray(std::string Name);

            /**
                @brief Gets the property named as the given name as an object. No conversions are done to get it.
                @param Name The name of the property.
                @return The property as an object.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            Object GetObject(std::string Name);

            /**
                @brief Gets the JSON std::string representation of this object.
                @return The JSON representation as a std::string.
                @note This should ONLY be CALLED if using it from a FunctionHandler. 
            */
            std::string AsJSON();
            int GetId();
            NativeContext GetContext();
        private:
            NativeContext ctx;
            duk_idx_t objidx;
            std::string tempname;
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
                @note This should NOT be CALLED manually. use FunctionHandler::GetCallback(...) to get it.
            */
            Callback(NativeContext Context, u32 Index);
            
            /**
                @brief Passes a std::string as an argument for the callback function.
                @param Value The std::string to pass.
                @note The value is added as the next argument.
            */
            void AddString(std::string Value);
            
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
                @brief Like Call, but like calling it with "new" keyword. Like doing "new Func(Args);".
                @note This should be called after adding all the arguments in order.
            */
            void CallNew();
            NativeContext GetContext();
            std::vector<std::string> GetStringArguments();
            std::vector<s64> GetIntArguments();
            std::vector<u64> GetUIntArguments();
            std::vector<double> GetDoubleArguments();
            std::vector<bool> GetBooleanArguments();
            u32 GetUndefinedArgumentCount();
            u32 GetNullArgumentCount();
            u32 GetNaNArgumentCount();
        private:
            NativeContext ctx;
            std::map<u32, std::string> strs;
            std::map<u32, s64> ints;
            std::map<u32, u64> uints;
            std::map<u32, double> dbls;
            std::map<u32, bool> bls;
            std::vector<u32> udfs;
            std::vector<u32> nlls;
            std::vector<u32> nans;
            std::vector<u32> reg;
            u32 iter;
    };
}