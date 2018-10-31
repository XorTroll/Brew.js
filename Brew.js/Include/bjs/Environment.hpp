
/**

    @file Environment.hpp
    @brief Brew.js environments, the high-level JavaScript evaluation contexts
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

#include <bjs/Modules/Modules.hpp>
#include <bjs/Modules/Node/FS.hpp>
#include <bjs/Modules/Node/OS.hpp>
#include <bjs/Modules/Node/Path.hpp>
#include <bjs/Modules/Node/Process.hpp>

#if __curOS == 0
	#include <bjs/Modules/NX/Console.hpp>
	#include <bjs/Modules/NX/NX.hpp>
	#include <bjs/Modules/NX/Gfx.hpp>
	#include <bjs/Modules/NX/Input.hpp>
	#include <bjs/Modules/NX/PegaSwitch.hpp>
	#include <bjs/Modules/NX/SDL2.hpp>
#elif __curOS == 1
	#include <bjs/Modules/CTR/Console.hpp>
	#include <bjs/Modules/CTR/CTR.hpp>
	#include <bjs/Modules/CTR/Input.hpp>
	#include <bjs/Modules/CTR/SF2D.hpp>
#elif __curOS == 2	
	#include <bjs/Modules/NTR/Console.hpp>
	#include <bjs/Modules/NTR/Gfx.hpp>
	#include <bjs/Modules/NTR/Input.hpp>
	#include <bjs/Modules/NTR/NTR.hpp>
#endif

namespace bjs
{
	/// The possible operating systems on which Brew.js can run.
	enum class OS
	{
		None = -1,  ///< No module found (Brew.js should error anyway)
		NX = 0,     ///< Nintendo Switch
		CTR = 1,    ///< Nintendo 3DS
		NTR = 2,    ///< Nintendo DS
	};

	/// Represents a JavaScript project which can be executed via an \ref Environment.
	struct Project
	{
		std::string Name;         ///< The name of the project.
		std::string Description;  ///< The description of the project.
		std::string Version;      ///< The version of the project.
		std::string EntrySource;  ///< The entrypoint source file of the project.
		std::string Directory;    ///< The directory where the project is located.
	};

	/// Represents the result for evaluating a JavaScript source, file or project.
	class EvaluationResult
	{
		public:

			/**
				@brief Creates a \ref EvaluationResult from a type, a result string and an error code.
				@param Type The returning type of the execution.
				@param Result The result as a string.
				@param Code The result code of the execution.
				@note This results should only be obtained from evaluation functions from \ref Environment.
			*/
			EvaluationResult(js::Type Type, std::string Result, int Code);
			
			/**
				@brief Gets the result of the execution as a string.
				@return The result string.
			*/
			std::string AsString();
			
			/**
				@brief Returns whether the execution went ok or had any errors.
				@return Whether the result was successful or not.
			*/
			bool IsSuccess();

			/**
				@brief Returns whether the execution went ok or had any errors.
				@return Whether the result was successful or not.
			*/
			bool IsFailure();

			/**
				@brief Gets the duktape error code from the execution.
				@return The error code.
			*/
			int GetErrorCode();

			/**
				@brief Gets the original type of the execution result.
				@return The type of the result.
			*/
			js::Type GetType();

			/**
				@brief Gets whether the result was undefined. (result is usually undefined if everything went OK)
				@return Whether the result was undefined.
			*/
			bool IsUndefined();
		
		private:
			js::Type type;
			std::string sres;
			int res;
	};
	
	/// Possible Brew.js errors related to evaluating files or projects.
	enum class Error
	{
		FileNotFound,          ///< The selected file was not found.
		ProjectsPathNotFound,  ///< The folder containing projects was not found.
		ProjectFolderNotFound, ///< The folder containing the project files was not found.
		EntrySourceNotFound,   ///< The entrypoint source file was not found.
		PackageJsonNotFound,   ///< Project file "package.json" file was not found, so it's not a valid project.
	};

	/// Class holding a Brew.js environment, which is a wrapper for a context with some evaluation functions.
	class Environment
	{
		public:

			/**
				@brief Creates a \ref Environment with a context to use.
				@param Context The context to use.
				@param AutoInitialize Shall the context initialize automatically? (loading built-in modules)
			*/
			Environment(js::NativeContext Context, bool AutoInitialize);
			~Environment();

			void Initialize(bool UseBuiltInModules);
			
			/**
				@brief Gets a vector list of projects from the given directory.
				@param ProjectsDirectory The directory to search projects on.
			*/
			std::vector<Project> GetProjectList(std::string ProjectsDirectory);
			
			/**
				@brief Evaluates source code and returns its execution result.
				@param Source JavaScript source code to evaluate.
				@return The result of the evaluation.
			*/
			EvaluationResult EvaluateSourceCode(std::string Source);
			
			/**
				@brief Evaluates source code from a file and returns its execution result.
				@param Path File to evaluate its source code.
				@return The result of the evaluation.
			*/
			EvaluationResult EvaluateFile(std::string Path);
			
			/**
				@brief Evaluates source code from a project and returns its execution result.
				@param Project Project to evaluate from.
				@return The result of the evaluation.
			*/
			EvaluationResult EvaluateProject(Project Project);
			
			/**
				@brief Gets the error string from the current error \ref Result.
				@return The error as a string.
			*/
			std::string GetError();

			/**
				@brief Gets the latest \ref Result of the \ref Environment.
				@return The result of the \ref Environment.
			*/
			Result GetResult();
			js::GlobalObject GetGlobalObject();
			void SetGlobalObject(js::GlobalObject &Global);
			std::vector<js::Module> GetModuleList();
			void SetModuleList(std::vector<js::Module> Modules);
			bool HasInitialized();
		private:
			std::vector<js::Module> mods;
			js::GlobalObject global;
			js::NativeContext ctx;
			Result res;
			bool einit;
	};

	/**
		@brief Gets the current operating system (Nintendo platform).
		@return The current \ref OS.
	*/
	OS GetCurrentOS();
	
	/**
		@brief Creates a Result (native from libctru or libnx, implemented here for libnds) from a Brew.js error type.
		@return The result from that error type.
	*/
	Result CreateError(Error ErrorType);

	static const u32 ResultModule = 716;
}