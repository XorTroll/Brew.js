
#pragma once
#include <bjs/js/Types.hpp>
#include <bjs/js/Class.hpp>
#include <string>

namespace bjs::js
{
	/// Class holding a module which can be added and required via Brew.js API.
	struct Module
	{
		/**
			@brief Creates a module from a name.
			@param Module The name the module will have. It will be accessed via that name if it's added to the module list.
		*/
		Module(std::string Module);

		/**
			@brief Adds a variable containing a std::string to the module.
			@param Name the variable has.
			@param Value the std::string value of the variable.
		*/
		void PushString(std::string Name, std::string Value);
		
		/**
			@brief Adds a variable containing a signed integer to the module.
			@param Name the variable has.
			@param Value the signed integer value of the variable.
		*/
		void PushInt(std::string Name, s64 Value);

		/**
			@brief Adds a variable containing an unsigned integer to the module.
			@param Name the variable has.
			@param Value the unsigned integer value of the variable.
		*/
		void PushUInt(std::string Name, u64 Value);

		/**
			@brief Adds a variable containing a double to the module.
			@param Name the variable has.
			@param Value the double value of the variable.
		*/
		void PushDouble(std::string Name, double Value);
		
		/**
			@brief Adds a variable containing a boolean to the module.
			@param Name the variable has.
			@param Value the boolean value of the variable.
		*/
		void PushBoolean(std::string Name, bool Value);
		
		/**
			@brief Adds a variable containing a function to the module.
			@param Name the variable has.
			@param Value the function value of the variable. Check NativeFunction for more information about API JS functions.
		*/
		void PushFunction(std::string Name, NativeFunction Value);
		
		/**
			@brief Adds a variable containing an undefined value to the module.
			@param Name the undefined variable has.
		*/
		void PushUndefined(std::string Name);

		/**
			@brief Adds a variable containing a null value to the module.
			@param Name the null variable has.
		*/
		void PushNull(std::string Name);

		/**
			@brief Adds a variable containing a NaN value to the module.
			@param Name the NaN variable has.
		*/
		void PushNaN(std::string Name);
		
		/**
			@brief Adds a class object to the module.
			@param Value The class object to add.
		*/
		void PushClass(Class &Value);
		std::map<std::string, std::string> GetStrings();
		std::map<std::string, s64> GetInts();
		std::map<std::string, u64> GetUInts();
		std::map<std::string, double> GetDoubles();
		std::map<std::string, bool> GetBooleans();
		std::map<std::string, NativeFunction> GetFunctions();
		std::vector<std::string> GetUndefineds();
		std::vector<std::string> GetNulls();
		std::vector<std::string> GetNaNs();
		std::vector<Class> GetClasses();
		std::string GetName();
	private:
		std::map<std::string, std::string> strs;
		std::map<std::string, s64> ints;
		std::map<std::string, u64> uints;
		std::map<std::string, double> dbls;
		std::map<std::string, bool> bls;
		std::map<std::string, NativeFunction> fncs;
		std::vector<std::string> udfs;
		std::vector<std::string> nlls;
		std::vector<std::string> nans;
		std::vector<Class> clss;
		std::string name;
	};
}