
#define NX
#include "Brew.js/Brew.js.h"

volatile int selected;
static int bck_offx = 575;
static int bck_offy = 235;

static string MainMenu = "sdmc:/switch/Brew.js/Brew.js.data0";
static string Information = "sdmc:/switch/Brew.js/Brew.js.data1";
static string CodeEnd = "sdmc:/switch/Brew.js/Brew.js.data2";
static string NoSources = "sdmc:/switch/Brew.js/Brew.js.data3";

void DrawBack(string path)
{
    unsigned res;
    u8 *raw;
    unsigned w;
    unsigned h;
    res = lodepng_decode32_file(&raw, &w, &h, path.c_str());
    if(res)
    {
        consoleInit(NULL);
        cout << "decode error!";
        return;
    }
    DrawImage(0, 0, w, h, raw, IMAGE_MODE_RGBA32);
}

bool FileExists(string path)
{
	ifstream ifs(path);
	bool ex = ifs.good();
	ifs.close();
	return ex;
}

string selectprj(vector<string> dirs)
{
	consoleClear();

	DrawBack(MainMenu);
	int noffy = bck_offy;
	for(int i = 0; i < dirs.size(); i++)
	{
		if(i == selected) DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
		else DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
		noffy += 45;
	}
	while(true)
	{
		gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if(k & KEY_DDOWN)
		{
			selected++;
			if(selected >= dirs.size()) selected = 0;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 45;
			}
		}
		else if(k & KEY_DUP)
		{
			selected--;
			if(selected < 0) selected = dirs.size() - 1;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 45;
			}
		}
		else if(k & KEY_PLUS || k & KEY_MINUS)
		{
			DrawBack(Information);
			while(true)
			{
				gfxFlushBuffers();
				gfxSwapBuffers();
				gfxWaitForVsync();
				hidScanInput();
				int k = hidKeysDown(CONTROLLER_P1_AUTO);
				if(k & KEY_B)
				{
					DrawBack(MainMenu);
					int noffy = bck_offy;
					for(int i = 0; i < dirs.size(); i++)
					{
						if(i == selected) DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
						else DrawText(InterUI_Regular_24, bck_offx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
						noffy += 45;
					}
					break;
				}
			}
		}
		else if(k & KEY_A)
		{
			return dirs[selected];
		}
	}
}

int main()
{
	PreEvaluate();
	if(!FileExists(MainMenu))
	{
		consoleInit(NULL);
		cout << endl << endl;
		cout << "   Required file does not exist:" << endl;
		cout << "   " << MainMenu << endl;
		cout << endl << endl;
		cout << "   Press B to exit interpreter.";
		while(true)
		{
			gfxFlushBuffers();
			gfxSwapBuffers();
			gfxWaitForVsync();
			hidScanInput();
			int k = hidKeysDown(CONTROLLER_P1_AUTO);
			if(k & KEY_B)
			{
				PostEvaluate();
				return 0;
			}
		}
	}
	if(!FileExists(Information))
	{
		consoleInit(NULL);
		cout << endl << endl;
		cout << "   Required file does not exist:" << endl;
		cout << "   " << Information << endl;
		cout << endl << endl;
		cout << "   Press B to exit interpreter.";
		while(true)
		{
			gfxFlushBuffers();
			gfxSwapBuffers();
			gfxWaitForVsync();
			hidScanInput();
			int k = hidKeysDown(CONTROLLER_P1_AUTO);
			if(k & KEY_B) return 0;
		}
	}
	if(!FileExists(CodeEnd))
	{
		consoleInit(NULL);
		cout << endl << endl;
		cout << "   Required file does not exist:" << endl;
		cout << "   " << CodeEnd << endl;
		cout << endl << endl;
		cout << "   Press B to exit interpreter.";
		while(true)
		{
			gfxFlushBuffers();
			gfxSwapBuffers();
			gfxWaitForVsync();
			hidScanInput();
			int k = hidKeysDown(CONTROLLER_P1_AUTO);
			if(k & KEY_B) return 0;
		}
	}
	if(!FileExists(NoSources))
	{
		consoleInit(NULL);
		cout << endl << endl;
		cout << "   Required file does not exist:" << endl;
		cout << "   " << NoSources << endl;
		cout << endl << endl;
		cout << "   Press B to exit interpreter.";
		while(true)
		{
			gfxFlushBuffers();
			gfxSwapBuffers();
			gfxWaitForVsync();
			hidScanInput();
			int k = hidKeysDown(CONTROLLER_P1_AUTO);
			if(k & KEY_B) return 0;
		}
	}
	string sjscode;
	ifstream ifs;
	vector<string> srcs, prjs;
	dirent *de;
	DIR *dp;
	goto start;
	start:
	srcs = vector<string>();
	prjs = vector<string>();
	selected = 0;
	dp = opendir("sdmc:/switch/Brew.js/");
	if (dp)
	{
		while (true)
		{
			de = readdir(dp);
			if(de == NULL) break;
			struct stat st;
			string path = string(de->d_name);
			string fullp = "sdmc:/switch/Brew.js/" + path;
			if(stat(fullp.c_str(), &st) == 0)
			{
				if(st.st_mode & S_IFDIR) prjs.push_back(path);
			}
		}
		closedir(dp);
		sort(prjs.begin(), prjs.end());
	}
	string dir = "sdmc:/switch/Brew.js/" + selectprj(prjs);
	sjscode += "game.ProjectDir='" + dir + "';";
	dp = opendir(dir.c_str());
	if (dp)
	{
		while (true)
		{
			de = readdir(dp);
			if(de == NULL) break;
			string file = string(de->d_name);
			if(file.substr(file.find_last_of(".") + 1) == "js") srcs.push_back(file);
		}
		closedir(dp);
		sort(srcs.begin(), srcs.end());
	}
	if(srcs.size() > 0) for(int i = 0; i < srcs.size(); i++)
	{
		ifs.open(dir + "/" + srcs[i]);
		if(ifs.good())
		{
			stringstream strm;
			strm << ifs.rdbuf();
			sjscode += strm.str();
		}
		ifs.close();
	}
	else
	{
		DrawBack(NoSources);
		while(true)
		{
			gfxFlushBuffers();
			gfxSwapBuffers();
			gfxWaitForVsync();
			hidScanInput();
			int k = hidKeysDown(CONTROLLER_P1_AUTO);
			if(k & KEY_B) goto start;
		}
	}
	for(int a = 0; a < 1280; a++)
	{
		for(int b = 0; b < 720; b++)
		{
			DrawPixel(a, b, MakeColor(0, 0, 0, 255));
		}
	}
	string res = Evaluate(sjscode);
	ofstream rst(dir + ".log");
	if(rst.good()) remove(res.c_str());
	rst.close();
	rst.open(dir + ".log");
	rst << res;
	rst.close();
	DrawBack(CodeEnd);
	while(true)
	{
		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if(k & KEY_A) goto start;
		else if(k & KEY_B)
		{
			PostEvaluate();
			return 0;
		}
	}
	PostEvaluate();
	return 0;
}