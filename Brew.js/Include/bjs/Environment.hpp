
/**

    @file Environment.hpp
    @brief Brew.js environments, high-level JavaScript evaluation support
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/js.hpp>
#include <bjs/node.hpp>
#ifdef bjsPlatformLibNX
	#include <bjs/platform/libnx.hpp>
#elif defined bjsPlatformLibCTRU
	#include <bjs/platform/libctru.hpp>
#endif

namespace bjs
{
	typedef u32 Result;

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
			Environment(js::NativeContext Context, bool AutoInitialize = true);
			~Environment();

			void Initialize(bool UseBuiltInModules = true);
			
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
			js::GlobalObject *GetGlobalObject();
			void SetGlobalObject(js::GlobalObject *Global);
			std::vector<js::Module> GetModuleList();
			void SetModuleList(std::vector<js::Module> Modules);
			bool HasInitialized();
		private:
			std::vector<js::Module> mods;
			js::GlobalObject *global;
			js::NativeContext ctx;
			Result res;
			bool einit;
	};
	
	/**
		@brief Creates a Result from a Brew.js error type.
		@return The result from that error type.
	*/
	Result CreateError(Error ErrorType);

	static const u32 ResultModule = 716;
}