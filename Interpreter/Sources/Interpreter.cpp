
#define NX
#include "Brew.js/Brew.js.h"

volatile int selected;
static int bck_offx = 75;
static int bck_offy = 290;

static string MainMenu = "romfs:/MainMenu.png";
static string Information = "romfs:/Information.png";
static string CodeEnd = "romfs:/CodeEnd.png";
static string NoSources = "romfs:/NoSources.png";

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
        return;
    }
    DrawImage(0, 0, w, h, raw, IMAGE_MODE_RGBA32);
}

string selectprj(vector<string> dirs)
{
	consoleClear();

	DrawBack(MainMenu);
	int noffy = bck_offy;
	int noffx = bck_offx;
	for(int i = 0; i < dirs.size(); i++)
	{
		if (i == selected)
			DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
		else
			DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
		noffy += 50;
		if ((i + 1) % 7 == 0)
		{
			noffy = bck_offy;
			noffx += 325;
		}
	}
	while(true)
	{
		gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if(k & KEY_DOWN)
		{
			selected++;
			if(selected >= dirs.size()) selected = 0;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			int noffx = bck_offx;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 50;
				if((i + 1) % 7 == 0)
				{
					noffy = bck_offy;
					noffx += 325;
				}
			}
		}
		else if(k & KEY_UP)
		{
			selected--;
			if(selected < 0 || selected > dirs.size()) selected = dirs.size() - 1;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			int noffx = bck_offx;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 50;
				if((i + 1) % 7 == 0)
				{
					noffy = bck_offy;
					noffx += 325;
				}
			}
		}
		else if(k & KEY_RIGHT)
		{
			selected += 7;
			if(selected < 0 || selected > dirs.size()) selected = dirs.size() - 1;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			int noffx = bck_offx;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 50;
				if((i + 1) % 7 == 0)
				{
					noffy = bck_offy;
					noffx += 325;
				}
			}
		}
		else if(k & KEY_LEFT)
		{
			selected -= 7;
			if(selected < 0 || selected > dirs.size()) selected = dirs.size() - 1;
			DrawBack(MainMenu);
			int noffy = bck_offy;
			int noffx = bck_offx;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
				else DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
				noffy += 50;
				if((i + 1) % 7 == 0)
				{
					noffy = bck_offy;
					noffx += 325;
				}
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
					int noffx = bck_offx;
					for(int i = 0; i < dirs.size(); i++)
					{
						if(i == selected) DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 194, 0, 255), dirs[i].c_str());
						else DrawText(InterUI_Medium_30, noffx, noffy, MakeColor(255, 255, 255, 255), dirs[i].c_str());
						noffy += 50;
						if((i + 1) % 7 == 0)
						{
							noffy = bck_offy;
							noffx += 325;
						}
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
	gfxInitDefault();
	romfsInit();
	string sjscode;
	ifstream ifs;
	vector<string> srcs, prjs;
	dirent *de;
	DIR *dp;
	goto start;
	start:
	sjscode = "";
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
	PreEvaluate();
	string res = Evaluate(sjscode);
	PostEvaluate();
	ofstream rst(dir + ".log");
	if(rst.good()) remove(res.c_str());
	rst.close();
	rst.open(dir + ".log");
	rst << res;
	rst.close();
	DrawBack(CodeEnd);
	DrawText(Tahoma_24, 335, 340, MakeColor(255, 255, 255, 255), res.c_str());
	while(true)
	{
		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if(k & KEY_B) goto start;
	}
	romfsExit();
	gfxExit();
	return 0;
}