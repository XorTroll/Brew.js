
#pragma once
#include <bjs/js/Types.hpp>
#include <string>

namespace bjs::js
{
    /// Represents a JavaScript class, which can be added to a module.
    struct Class
    {
        /**
            @brief Creates a class with the class name and the constructor function.
            @param Name The name of the class.
            @param Constructor The API JS function of the class constructor.
        */
        Class(std::string Name, NativeFunction Constructor);

        /**
            @brief Adds a std::string as a property with the given name.
            @param Name The name of the property to set.
            @param Value The std::string to set as property.
            @note The property WON'T be static.
        */
        void AddString(std::string Name, std::string Value);
        
        /**
            @brief Adds a signed integer as a property with the given name.
            @param Name The name of the property to set.
            @param Value The signed integer to set as property.
            @note The property WON'T be static.
        */
        void AddInt(std::string Name, int Value);

        /**
            @brief Adds an unsigned integer as a property with the given name.
            @param Name The name of the property to set.
            @param Value The unsigned integer to set as property.
            @note The property WON'T be static.
        */
        void AddUInt(std::string Name, u32 Value);

        /**
            @brief Adds a double as a property with the given name.
            @param Name The name of the property to set.
            @param Value The double to set as property.
            @note The property WON'T be static.
        */
        void AddDouble(std::string Name, double Value);

        /**
            @brief Adds a boolean as a property with the given name.
            @param Name The name of the property to set.
            @param Value The boolean to set as property.
            @note The property WON'T be static.
        */
        void AddBoolean(std::string Name, bool Value);

        /**
            @brief Adds an API JS function as a property with the given name.
            @param Name The name of the property to set.
            @param Value The API JS function to set as property.
            @note The property WON'T be static.
        */
        void AddFunction(std::string Name, NativeFunction Value);

        /**
            @brief Adds undefined as a property with the given name.
            @param Name The name of the property to set to undefined.
            @note The property WON'T be static.
        */
        void AddUndefined(std::string Name);
        
        /**
            @brief Adds null as a property with the given name.
            @param Name The name of the property to set to null.
            @note The property WON'T be static.
        */
        void AddNull(std::string Name);

        /**
            @brief Adds NaN as a property with the given name.
            @param Name The name of the property to set to NaN.
            @note The property WON'T be static.
        */
        void AddNaN(std::string Name);

        /**
            @brief Gets the constructor function of this class.
            @note The property WON'T be static.
            @return The function of the constructor.
        */
        NativeFunction GetConstructorFunction();
        std::map<std::string, std::string> GetStrings();
        std::map<std::string, int> GetInts();
        std::map<std::string, u32> GetUInts();
        std::map<std::string, double> GetDoubles();
        std::map<std::string, bool> GetBooleans();
        std::map<std::string, NativeFunction> GetFunctions();
        std::vector<std::string> GetUndefineds();
        std::vector<std::string> GetNulls();
        std::vector<std::string> GetNaNs();
        std::string GetName();
    private:
        NativeFunction ctor;
        std::map<std::string, std::string> strs;
        std::map<std::string, int> ints;
        std::map<std::string, u32> uints;
        std::map<std::string, double> dbls;
        std::map<std::string, bool> bls;
        std::map<std::string, NativeFunction> fncs;
        std::vector<std::string> udfs;
        std::vector<std::string> nlls;
        std::vector<std::string> nans;
        std::string name;
    };
}