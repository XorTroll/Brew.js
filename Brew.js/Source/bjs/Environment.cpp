#include <bjs/Environment.hpp>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>

namespace bjs
{
	EvaluationResult::EvaluationResult(js::Type Type, std::string Result, int Code)
	{
		this->type = Type;
		this->sres = Result;
		this->res = Code;
	}

	std::string EvaluationResult::AsString()
	{
		return this->sres;
	}

	bool EvaluationResult::IsSuccess()
	{
		return (this->res == 0);
	}

	bool EvaluationResult::IsFailure()
	{
		return (this->res != 0);
	}

	int EvaluationResult::GetErrorCode()
	{
		return this->res;
	}

	js::Type EvaluationResult::GetType()
	{
		return type;
	}

	bool EvaluationResult::IsUndefined()
	{
		return (type == js::Type::Undefined);
	}

	Environment::Environment(js::NativeContext Context, bool AutoInitialize) : global(NULL)
	{
		srand(time(NULL));
		this->einit = false;
		this->ctx = Context;
		if(AutoInitialize) this->Initialize(true);
	}

	Environment::~Environment()
	{
		#if (bjsPlatformIndex == 0) || (curOS == 1)
			romfsExit();
		#endif
		this->mods.clear();
		duk_destroy_heap(this->ctx);
	}

	void Environment::Initialize(bool UseBuiltInModules)
	{
		if(!this->einit)
		{
			this->global = js::InitializeGlobal(this->ctx);
			if(UseBuiltInModules)
			{
				js::Module console = node::console::CreateModule();
				this->global.PushModule(console);
				js::Module fs = node::fs::CreateModule();
				js::AddModule(fs);
				js::Module os = node::os::CreateModule();
				js::AddModule(os);
				js::Module path = node::path::CreateModule();
				js::AddModule(path);
				js::Module process = node::process::CreateModule();
				this->global.PushModule(process);
				#ifdef bjsPlatformLibNX
					romfsInit();
					js::Module input = libnx::input::CreateModule();
					js::AddModule(input);
					js::Module nx = libnx::nx::CreateModule();
					js::AddModule(nx);
					js::Module pegaswitch = libnx::pegaswitch::CreateModule();
					js::AddModule(pegaswitch);
				#elif defined bjsPlatformLibCTRU
					romfsInit();
					js::Module ctr = libctru::ctr::CreateModule();
					js::AddModule(ctr);
					js::Module input = libctru::input::CreateModule();
					js::AddModule(input);
					js::Module sf2d = libctru::sf2d::CreateModule();
					js::AddModule(sf2d);
				#endif
			}
			this->einit = true;
		}
	}

	std::vector<Project> Environment::GetProjectList(std::string ProjectsDirectory)
	{
		std::vector<Project> prjs;
		DIR *dir = opendir(ProjectsDirectory.c_str());
		if(dir)
		{
			struct dirent *dent;
			struct stat st;
			while(true)
			{
				dent = readdir(dir);
				if(dent == NULL) break;
				std::string path = std::string(dent->d_name);
				std::string fullp = ProjectsDirectory + "/" + path;
				if(stat(fullp.c_str(), &st) == 0)
				{
					if(st.st_mode & S_IFDIR)
					{
						std::ifstream ifs(fullp + "/package.json");
						if(ifs.good())
						{
							nlohmann::json packreader;
							ifs >> packreader;
							std::string name = packreader["name"].get<std::string>();
							std::string desc = packreader["description"].get<std::string>();
							std::string vers = packreader["version"].get<std::string>();
							std::string src = packreader["main"].get<std::string>();
							Project proj = { name, desc, vers, src, fullp };
							prjs.push_back(proj);
						}
						else this->res = CreateError(Error::PackageJsonNotFound);
						ifs.close();
					}
				}
				else this->res = CreateError(Error::ProjectFolderNotFound);
			}
		}
		else this->res = CreateError(Error::ProjectsPathNotFound);
		closedir(dir);
		return prjs;
	}

	EvaluationResult Environment::EvaluateSourceCode(std::string Source)
	{
		int res = duk_peval_string(this->ctx, Source.c_str());
		js::Type tt = (js::Type)duk_get_type(this->ctx, -1);
		duk_safe_to_string(this->ctx, -1);
		std::string sres = std::string(duk_get_string(this->ctx, -1));
		duk_pop(this->ctx);
		return EvaluationResult(tt, sres, res);
	}

	EvaluationResult Environment::EvaluateFile(std::string Path)
	{
		std::string dir = Path.substr(0, Path.find_last_of("/\\"));
		std::ifstream ifs(Path);
		if(ifs.good())
		{
			std::stringstream strm;
			strm << ifs.rdbuf();
			EvaluationResult rst = this->EvaluateSourceCode(strm.str());
			ifs.close();
			return rst;
		}
		else
		{
			this->res = CreateError(Error::FileNotFound);
			ifs.close();
			return EvaluationResult(js::Type::None, "Empty execution result", -6);
		}
		return EvaluationResult(js::Type::None, "Empty execution result", -6);
	}

	EvaluationResult Environment::EvaluateProject(Project Project)
	{
		return this->EvaluateFile(Project.Directory + "/" + Project.EntrySource);
	}

	Result Environment::GetResult()
	{
		return this->res;
	}

	std::string Environment::GetError()
	{
		std::string err = "Unknown error (possibly not a Brew.js error)";
		int modl = ((this->res) & 0x1ff);
		if(modl == ResultModule)
		{
			Error desc = static_cast<Error>(((this->res) >> 9) & 0x1fff);
			switch(desc)
			{
				case Error::FileNotFound:
					err = "A file was not found.";
					break;
				case Error::EntrySourceNotFound:
					err = "Project's entry source file was not found.";
					break;
				case Error::PackageJsonNotFound:
					err = "Project's 'package.json' project file was not found.";
					break;
				default:
					err = "Unknown error (error code " + std::to_string(static_cast<u32>(desc)) + ")";
					break;
			}
		}
		return err;
	}

	js::GlobalObject Environment::GetGlobalObject()
	{
		return this->global;
	}

	void Environment::SetGlobalObject(js::GlobalObject &Global)
	{
		this->global = Global;
	}

	std::vector<js::Module> Environment::GetModuleList()
	{
		return this->mods;
	}

	void Environment::SetModuleList(std::vector<js::Module> Modules)
	{
		this->mods = Modules;
	}

	Result CreateError(Error ErrorType)
	{
		return (((ResultModule) & 0x1ff) || (((static_cast<u32>(ErrorType)) & 0x1fff) << 9));
	}
}