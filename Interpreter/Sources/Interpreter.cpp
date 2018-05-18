
#define NX
#include "Brew.js/Brew.js.h"

volatile int selected;

string selectprj(vector<string> dirs)
{
	consoleClear();
	cout << endl << endl;
	cout << "  --  " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " interpreter  --" << endl << endl;
	cout << endl << " Found projects (" << CONSOLE_GREEN << dirs.size() << CONSOLE_RESET << "):                    (Press " << CONSOLE_CYAN << "+" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "-" << CONSOLE_RESET << " to display control info)" << endl << endl;
	for(int i = 0; i < dirs.size(); i++)
	{
		if(i == selected) cout << CONSOLE_RED << " -- " << CONSOLE_RESET << dirs[i] << endl;
		else cout << " -- " << dirs[i] << endl;
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
			consoleClear();
			cout << endl << endl;
			cout << "  --  " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " interpreter  --" << endl << endl;
			cout << endl << " Found projects (" << CONSOLE_GREEN << dirs.size() << CONSOLE_RESET << "):                    (Press " << CONSOLE_CYAN << "+" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "-" << CONSOLE_RESET << " to display control info)" << endl << endl;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) cout << CONSOLE_RED << " -- " << CONSOLE_RESET << dirs[i] << endl;
				else cout << " -- " << dirs[i] << endl;
			}
		}
		else if(k & KEY_DUP)
		{
			selected--;
			if(selected < 0) selected = dirs.size() - 1;
			consoleClear();
			cout << endl << endl;
			cout << "  --  " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " interpreter  --" << endl << endl;
			cout << endl << " Found projects (" << CONSOLE_GREEN << dirs.size() << CONSOLE_RESET << "):                    (Press " << CONSOLE_CYAN << "+" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "-" << CONSOLE_RESET << " to display control info)" << endl << endl;
			for(int i = 0; i < dirs.size(); i++)
			{
				if(i == selected) cout << CONSOLE_RED << " -- " << CONSOLE_RESET << dirs[i] << endl;
				else cout << " -- " << dirs[i] << endl;
			}
		}
		else if(k & KEY_PLUS || k & KEY_MINUS)
		{
			consoleClear();
			cout << endl << endl;
			cout << "     --  " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " interpreter information  --" << endl << endl << endl;
			cout << "   A " << CONSOLE_BLUE << "project folder" << CONSOLE_RESET << " must contain " << CONSOLE_YELLOW << "JS" << CONSOLE_RESET << " files. (*." << CONSOLE_YELLOW << "js" << CONSOLE_RESET << ")" << endl << endl;
			cout << endl;
			cout << "    - Press " << CONSOLE_CYAN << "A" << CONSOLE_RESET << " to interpretate a project folder." << endl << endl;
			cout << "    - Use " << CONSOLE_CYAN << "Up" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "Down" << CONSOLE_RESET << " to move around " << CONSOLE_BLUE << "project folders" << CONSOLE_RESET << "." << endl << endl;
			cout << "    - Press " << CONSOLE_CYAN << "+" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "-" << CONSOLE_RESET << " to display this info." << endl << endl;
			cout << endl;
			cout << "   This interpreter is part of " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " project." << endl << endl;
			cout << "   Copyright 2018 by " << CONSOLE_CYAN << "X" << CONSOLE_RESET << "or" << CONSOLE_CYAN << "T" << CONSOLE_RESET << "roll." << endl << endl;
			cout << endl;
			cout << "   Now, press " << CONSOLE_CYAN << "B" << CONSOLE_RESET << " to return to the menu." << endl << endl;
			while(true)
			{
				gfxFlushBuffers();
				gfxSwapBuffers();
				gfxWaitForVsync();
				hidScanInput();
				int k = hidKeysDown(CONTROLLER_P1_AUTO);
				if(k & KEY_B)
				{
					consoleClear();
					cout << endl << endl;
					cout << "  --  " << CONSOLE_GREEN << "Brew" << CONSOLE_RESET << "." << CONSOLE_GREEN << "js" << CONSOLE_RESET << " interpreter  --" << endl << endl;
					cout << endl << " Found projects (" << CONSOLE_GREEN << dirs.size() << CONSOLE_RESET << "):                    (Press " << CONSOLE_CYAN << "+" << CONSOLE_RESET << " or " << CONSOLE_CYAN << "-" << CONSOLE_RESET << " to display control info)" << endl << endl;
					for(int i = 0; i < dirs.size(); i++)
					{
						if(i == selected) cout << CONSOLE_RED << " -- " << CONSOLE_RESET << dirs[i] << endl;
						else cout << " -- " << dirs[i] << endl;
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
	consoleInit(NULL);
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
	consoleClear();
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
		consoleClear();
		cout << endl;
		cout << "   No source files were found in selected project folder." << endl;
		cout << endl;
		cout << "   Press " << CONSOLE_CYAN << "B" << CONSOLE_RESET << " to return to the menu." << endl << endl;
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
	Evaluate(sjscode);
	PostEvaluate();
	return 0;
}