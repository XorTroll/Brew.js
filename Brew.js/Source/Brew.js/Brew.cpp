#include <Brew.js/Brew.hpp>

namespace Brew
{
	EvaluationResult::EvaluationResult(API::Type Type, string Result, int Code)
	{
		this->type = Type;
		this->sres = Result;
		this->res = Code;
	}

	string EvaluationResult::AsString()
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

	API::Type EvaluationResult::GetType()
	{
		return type;
	}

	bool EvaluationResult::IsUndefined()
	{
		return (type == API::Type::Undefined);
	}

	Environment::Environment(API::NativeContext Context)
	{
		srand(time(NULL));
		this->ctx = Context;
		API::InitializeGlobalObject(Context);
		API::Module fs = BuiltIn::FS::Initialize();
		API::AddModule(fs);
		API::Module os = BuiltIn::OS::Initialize();
		API::AddModule(os);
		API::Module path = BuiltIn::Path::Initialize();
		API::AddModule(path);
		API::Module process = BuiltIn::Process::Initialize();
		API::AddModule(process);
		#if __curOS == 0
			romfsInit();
			API::Module nx = BuiltIn::NX::Initialize();
			API::AddModule(nx);
			API::Module gfx = BuiltIn::Gfx::Initialize();
			API::AddModule(gfx);
			API::Module input = BuiltIn::Input::Initialize();
			API::AddModule(input);
			API::Module pegaswitch = BuiltIn::PegaSwitch::Initialize();
			API::AddModule(pegaswitch);
			API::Module sdl2 = BuiltIn::SDL2::Initialize();
			API::AddModule(sdl2);
		#elif __curOS == 1
			romfsInit();
			API::Module ctr = BuiltIn::CTR::Initialize();
			API::AddModule(ctr);
			API::Module input = BuiltIn::Input::Initialize();
			API::AddModule(input);
			API::Module sf2d = BuiltIn::SF2D::Initialize();
			API::AddModule(sf2d);
		#elif __curOS == 2
			nitroFSInit(NULL);
			fatInitDefault();
			API::Module gfx = BuiltIn::Gfx::Initialize();
			API::AddModule(gfx);
			API::Module input = BuiltIn::Input::Initialize();
			API::AddModule(input);
			API::Module ntr = BuiltIn::NTR::Initialize();
			API::AddModule(ntr);
		#endif
	}

	Environment::~Environment()
	{
		#if (__curOS == 0) || (curOS == 1)
			romfsExit();
		#endif
		API::Modules.clear();
		duk_destroy_heap(this->ctx);
	}

	void Environment::LoadNewContext(API::NativeContext NewContext)
	{
		this->ctx = NewContext;
		API::InitializeGlobalObject(NewContext);
	}

	vector<Project> Environment::GetProjectList(string ProjectsDirectory)
	{
		vector<Project> prjs;
		DIR *dir = opendir(ProjectsDirectory.c_str());
		if(dir)
		{
			struct dirent *dent;
			struct stat st;
			while(true)
			{
				dent = readdir(dir);
				if(dent == NULL) break;
				string path = string(dent->d_name);
				string fullp = ProjectsDirectory + "/" + path;
				if(stat(fullp.c_str(), &st) == 0)
				{
					if(st.st_mode & S_IFDIR)
					{
						ifstream ifs(fullp + "/package.json");
						if(ifs.good())
						{
							json packreader;
							ifs >> packreader;
							string name = packreader["name"].get<string>();
							string desc = packreader["description"].get<string>();
							string vers = packreader["version"].get<string>();
							string src = packreader["main"].get<string>();
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

	EvaluationResult Environment::EvaluateSourceCode(string Source)
	{
		int res = duk_peval_string(this->ctx, Source.c_str());
		API::Type tt = (API::Type)duk_get_type(this->ctx, -1);
		duk_safe_to_string(this->ctx, -1);
		string sres = string(duk_get_string(this->ctx, -1));
		duk_pop(this->ctx);
		return EvaluationResult(tt, sres, res);
	}

	EvaluationResult Environment::EvaluateFile(string Path)
	{
		string dir = Path.substr(0, Path.find_last_of("/\\"));
		ifstream ifs(Path);
		if(ifs.good())
		{
			stringstream strm;
			strm << ifs.rdbuf();
			EvaluationResult rst = this->EvaluateSourceCode(strm.str());
			ifs.close();
			return rst;
		}
		else
		{
			this->res = CreateError(Error::FileNotFound);
			ifs.close();
			return EvaluationResult(API::Type::None, "Empty execution result", -6);
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

	string Environment::GetError()
	{
		string err = "Unknown error (not a Brew.js error?)";
		int modl = R_MODULE(this->res);
		if(modl == Brew::Module)
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
					err = "Unknown error (error code " + to_string(static_cast<u32>(desc)) + ")";
					break;
			}
		}
		return err;
	}

	OS GetCurrentOS()
	{
		return static_cast<OS>(__curOS);
	}

	Result CreateError(Error ErrorType)
	{
		return MAKERESULT(Brew::Module, static_cast<u32>(ErrorType));
	}
}