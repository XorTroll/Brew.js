#include "Brew.hpp"

int berror = 516;

Brew::OS Brew::currentOS()
{
	return (Brew::OS)__curOS;
}

Brew::ExecutionResult::ExecutionResult(Brew::API::Type Type, string Result, int Code)
{
	this->type = Type;
	this->sres = Result;
	this->res = Code;
}

string Brew::ExecutionResult::resultString()
{
	return sres;
}

bool Brew::ExecutionResult::isOK()
{
	return (res == 0);
}

int Brew::ExecutionResult::nativeCode()
{
	return res;
}

Brew::API::Type Brew::ExecutionResult::getType()
{
	return type;
}

bool Brew::ExecutionResult::isUndefined()
{
	return (type == Brew::API::Type::Undefined);
}

Brew::Environment::Environment(Brew::API::NativeContext Context)
{
	srand(time(NULL));
	this->ctx = Context;
	API::initializeGlobalObject(this->ctx);
	API::addModule(BuiltIn::FS::initModule());
	API::addModule(BuiltIn::OS::initModule());
	API::addModule(BuiltIn::Path::initModule());
	API::addModule(BuiltIn::Process::initModule());
	#if __curOS == 0

		romfsInit();
		API::addModule(BuiltIn::NX::initModule());
		API::addModule(BuiltIn::Gfx::initModule());
		API::addModule(BuiltIn::Input::initModule());

	#elif __curOS == 1

		romfsInit();
		API::addModule(BuiltIn::CTR::initModule());
		API::addModule(BuiltIn::Input::initModule());
		API::addModule(BuiltIn::SF2D::initModule());

	#elif __curOS == 2

		nitroFSInit(NULL);
		fatInitDefault();
		API::addModule(BuiltIn::Gfx::initModule());
		API::addModule(BuiltIn::Input::initModule());
		API::addModule(BuiltIn::NTR::initModule());

	#endif
}

Brew::Environment::~Environment()
{
	#if __curOS == 0 || curOS == 1

		romfsExit();

	#endif
	API::Modules.clear();
	duk_destroy_heap(this->ctx);
}

vector<Brew::Project> Brew::Environment::getProjectList(string ProjectsDirectory)
{
	vector<Brew::Project> prjs;
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
						Brew::Project proj = { name, desc, vers, src, fullp };
						prjs.push_back(proj);
					}
					else this->res = createError(Brew::Error::PackageJsonNotFound);
					ifs.close();
				}
			}
		}
	}
	else this->res = createError(Brew::Error::ProjectsPathNotFound);
	closedir(dir);
	return prjs;
}

Brew::ExecutionResult Brew::Environment::evaluateSourceCode(string Source)
{
	int res = duk_peval_string(this->ctx, Source.c_str());
	Brew::API::Type tt = (Brew::API::Type)duk_get_type(this->ctx, -1);
	duk_safe_to_string(this->ctx, -1);
	string sres = string(duk_get_string(this->ctx, -1));
	duk_pop(this->ctx);
	return ExecutionResult(tt, sres, res);
}

Brew::ExecutionResult Brew::Environment::evaluateFile(string Path)
{
	string dir = Path.substr(0, Path.find_last_of("/\\"));
	//API::Global.pushString("__dirname", dir);
	//API::Global.pushString("__filename", Path);
	ifstream ifs(Path);
	if(ifs.good())
	{
		stringstream strm;
		strm << ifs.rdbuf();
		Brew::ExecutionResult rst = this->evaluateSourceCode(strm.str());
		ifs.close();
		return rst;
	}
	else
	{
		this->res = createError(Brew::Error::FileNotFound);
		ifs.close();
		return ExecutionResult(Brew::API::Type::None, "Empty execution result", -6);
	}
}

Brew::ExecutionResult Brew::Environment::evaluateProject(Brew::Project Project)
{
	return this->evaluateFile(Project.Directory + "/" + Project.EntrySource);
}

Result Brew::Environment::getResult()
{
	return this->res;
}

string Brew::Environment::getError()
{
	string err = "Unknown error (not a Brew.js error?)";
	int modl = R_MODULE(this->res);
	if(modl == berror)
	{
		u32 desc = R_DESCRIPTION(this->res);
		switch(desc)
		{
			case Brew::Error::FileNotFound:
				err = "A file was not found.";
				break;
			case Brew::Error::EntrySourceNotFound:
				err = "Project's entry source file was not found.";
				break;
			case Brew::Error::PackageJsonNotFound:
				err = "Project's 'package.json' project file was not found.";
				break;
			default:
				err = "Unknown error (error code " + to_string(desc) + ")";
				break;
		}
	}
	return err;
}

Result Brew::createError(Brew::Error ErrorType)
{
	return MAKERESULT(berror, (u32)ErrorType);
}