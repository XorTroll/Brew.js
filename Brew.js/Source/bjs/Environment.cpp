#include <bjs/Environment.hpp>

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
		#if (__curOS == 0) || (curOS == 1)
			romfsExit();
		#endif
		this->mods.clear();
		duk_destroy_heap(this->ctx);
	}

	void Environment::Initialize(bool UseBuiltInModules)
	{
		if(!this->einit)
		{
			this->global = js::GlobalObject(this->ctx);
			js::Object exports(this->ctx);
			this->global.InitializePushingObject("exports", exports);
			this->global.FinalizePushingObject();
			js::Object module(this->ctx);
			this->global.InitializePushingObject("module", module);
			js::Object moduleexports(this->ctx);
			module.InitializeAddingObject("exports", moduleexports);
			module.FinalizeAddingObject();
			this->global.FinalizePushingObject();
			if(UseBuiltInModules)
			{
				this->global.PushFunction("require", modules::require);
				this->global.PushFunction("evalFile", modules::evalFile);
				this->global.PushFunction("randRange", modules::randRange);
				js::Module console = modules::Console::Initialize();
				this->global.PushModule(console);
				js::Module process = modules::Process::Initialize();
				this->global.PushModule(process);
				js::Module fs = modules::FS::Initialize();
				modules::Add(fs);
				js::Module os = modules::OS::Initialize();
				modules::Add(os);
				js::Module path = modules::Path::Initialize();
				modules::Add(path);
				#if __curOS == 0
					romfsInit();
					js::Module nx = modules::NX::Initialize();
					modules::Add(nx);
					js::Module gfx = modules::Gfx::Initialize();
					modules::Add(gfx);
					js::Module input = modules::Input::Initialize();
					modules::Add(input);
					js::Module pegaswitch = modules::PegaSwitch::Initialize();
					modules::Add(pegaswitch);
					js::Module sdl2 = modules::SDL2::Initialize();
					modules::Add(sdl2);
				#elif __curOS == 1
					romfsInit();
					js::Module ctr = modules::CTR::Initialize();
					modules::Add(ctr);
					js::Module input = modules::Input::Initialize();
					modules::Add(input);
					js::Module sf2d = modules::SF2D::Initialize();
					modules::Add(sf2d);
				#elif __curOS == 2
					nitroFSInit(NULL);
					fatInitDefault();
					js::Module gfx = modules::Gfx::Initialize();
					modules::Add(gfx);
					js::Module input = modules::Input::Initialize();
					modules::Add(input);
					js::Module ntr = modules::NTR::Initialize();
					modules::Add(ntr);
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
		int modl = R_MODULE(this->res);
		if(modl == ResultModule)
		{
			Error desc = static_cast<Error>(R_DESCRIPTION(this->res));
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

	OS GetCurrentOS()
	{
		return static_cast<OS>(__curOS);
	}

	Result CreateError(Error ErrorType)
	{
		return MAKERESULT(ResultModule, static_cast<u32>(ErrorType));
	}
}