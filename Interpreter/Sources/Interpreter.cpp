
#include "Brew.js/Brew.js.h"
#include "json.hpp"
using namespace nlohmann;

static int selected = 0;
static bool selectt = false;
static int bck_offx = 60;
static int bck_offy = 230;
static int parts = 1;
static int cpart = 1;
static int banw = 350;
static int banh = 100;
static int partx = 615;
static int party = 675;

static string MainMenu = "romfs:/Graphics/MainMenu.png";
static string Information = "romfs:/Graphics/Information.png";
static string CodeEnd = "romfs:/Graphics/CodeEnd.png";
static string Banner = "romfs:/Graphics/Banner.png";

static SDL_Surface *sdl_MainMenu;
static SDL_Surface *sdl_Information;
static SDL_Surface *sdl_CodeEnd;
static SDL_Surface *sdl_Banner;

static SDL_Texture *sdlt_MainMenu;
static SDL_Texture *sdlt_Information;
static SDL_Texture *sdlt_CodeEnd;
static SDL_Texture *sdlt_Banner;

static TTF_Font *drawfnt15;
static TTF_Font *drawfnt25;

void end()
{
	Brew::Exit();
	exit(0);
}

void DrawText(TTF_Font *font, int x, int y, SDL_Color colour, const char *text)
{
	SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text, colour, 1280);
	SDL_SetSurfaceAlphaMod(surface, 255);
	SDL_Rect position = { x, y, surface->w, surface->h };
	SDL_BlitSurface(surface, NULL, sdl::sdl_surf, &position);
	SDL_FreeSurface(surface);
}

void DrawRect(int x, int y, int w, int h, SDL_Color colour)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	SDL_SetRenderDrawColor(sdl::sdl_render, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(sdl::sdl_render, &rect);
}

SDL_Surface *InitSurface(string Path)
{
	SDL_Surface *srf = IMG_Load(Path.c_str());
	SDL_Texture *tex;
	if(srf)
	{
		Uint32 colorkey = SDL_MapRGB(srf->format, 0, 0, 0);
		SDL_SetColorKey(srf, SDL_TRUE, colorkey);
		tex = SDL_CreateTextureFromSurface(sdl::sdl_render, srf);
	}
	return srf;
}

SDL_Texture *InitTexture(SDL_Surface *surf)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl::sdl_render, surf);
	return tex;
}

void DrawBackXY(SDL_Surface *surf, SDL_Texture *tex, int x, int y)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = surf->w;
	position.h = surf->h;
	SDL_RenderCopy(sdl::sdl_render, tex, NULL, &position);
}

void DrawBack(SDL_Surface *surf, SDL_Texture *tex)
{
	DrawBackXY(surf, tex, 0, 0);	
}

void DrawBanner(int x, int y)
{
    DrawBackXY(sdl_Banner, sdlt_Banner, x, y);
}

void redraw(vector<string> dirs)
{
	SDL_RenderClear(sdl::sdl_render);
	string parttxt = to_string(cpart) + " / " + to_string(parts);
	DrawBack(sdl_MainMenu, sdlt_MainMenu);
	DrawText(drawfnt25, partx, party, {255, 255, 255, 255}, parttxt.c_str());
	int noffy = bck_offy;
	int noffx = bck_offx;
	for(int i = 0; i < dirs.size(); i++)
	{
		DrawBanner(noffx, noffy);
		if(selectt && (i == selected)) DrawRect(noffx, noffy, banw, banh - 4, { 255, 194, 0, 100 });
		string dir = "sdmc:/switch/Brew.js/" + dirs[i];
		ifstream ifs(dir + "/package.json");
		json pack;
		ifs >> pack;
		ifs.close();
		string name = pack["name"].get<string>();
		string desc = pack["description"].get<string>();
		string vers = pack["version"].get<string>();
		DrawText(drawfnt25, noffx + 20, noffy + 15, {0, 0, 0, 255}, name.c_str());
		DrawText(drawfnt15, noffx + 25, noffy + 45, {0, 0, 0, 255}, desc.c_str());
		DrawText(drawfnt15, noffx + 25, noffy + 65, {0, 0, 0, 255}, vers.c_str());
		noffy += banh + 40;
		if((i + 1) % 9 == 0)
		{
			noffy = bck_offy;
			noffx += banw + 60;
		}
		else if ((i + 1) % 3 == 0)
		{
			noffy = bck_offy;
			noffx += banw + 40;
		}
		SDL_RenderPresent(sdl::sdl_render);
	}
}

string selectprj(vector<string> dirs)
{
	double data = dirs.size() / 9;
	double fpart;
	if(modf(data, &fpart) == 0) parts = fpart + 1;
	else parts = fpart;
	redraw(dirs);
	while(true)
	{
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if(k & KEY_TOUCH)
		{
			touchPosition tch;
			hidTouchRead(&tch, 0);
			int noffx = bck_offx;
			int noffy = bck_offy;
			bool detec = false;
			for(int i = 0; i < dirs.size(); i++)
			{
				for(int a = 0; a < banw; a++)
				{
					for(int b = 0; b < banh; b++)
					{
						int cx = a + noffx;
						int cy = b + noffy;
						if((cx == tch.px) && (cy == tch.py))
						{
							selected = i;
							selectt = true;
							redraw(dirs);
							SDL_Delay(300);
							return dirs[selected];
						}
					}
				}
				noffy += banh + 40;
				if((i + 1) % 9 == 0)
				{
					noffy = bck_offy;
					noffx += banw + 60;
				}
				else if ((i + 1) % 3 == 0)
				{
					noffy = bck_offy;
					noffx += banw + 40;
				}
			}
		}
		else if(k & KEY_ZL)
		{
			if(cpart > 1)
			{
				cpart--;
				bck_offx += 1220;
				redraw(dirs);
			}
		}
		else if(k & KEY_ZR)
		{
			if(cpart < parts)
			{
				cpart++;
				bck_offx -= 1220;
				redraw(dirs);
			}
		}
		else if(k & KEY_UP)
		{
			if(selectt)
			{
				int slect = (selected + 1 / cpart) - 1;
				if((slect != 0) && (slect != 3) && (slect != 6)) selected--;
			}
			else
			{
				selected = 0;
				selectt = true;
			}
			redraw(dirs);
		}
		else if(k & KEY_DOWN)
		{
			if(selectt)
			{
				int slect = (selected + 1 / cpart) - 1;
				if((slect != 2) && (slect != 5) && (slect != 8)) selected++;
			}
			else
			{
				selected = 0;
				selectt = true;
			}
			redraw(dirs);
		}
		else if(k & KEY_LEFT)
		{
			if(selectt)
			{
				int slect = (selected + 1 / cpart) - 1;
				if((slect != 0) && (slect != 1) && (slect != 2)) selected -= 3;
			}
			else
			{
				selected = 0;
				selectt = true;
			}
			redraw(dirs);
		}
		else if(k & KEY_RIGHT)
		{
			if(selectt)
			{
				int slect = (selected + 1 / cpart) - 1;
				if((slect != 6) && (slect != 7) && (slect != 8)) selected += 3;
			}
			else
			{
				selected = 0;
				selectt = true;
			}
			redraw(dirs);
		}
		else if(k & KEY_L || k & KEY_R)
		{
			while(true)
			{
				SDL_RenderClear(sdl::sdl_render);
				DrawBack(sdl_Information, sdlt_Information);
				hidScanInput();
				int k = hidKeysDown(CONTROLLER_P1_AUTO);
				if(k & KEY_B) break;
				else if((k & KEY_PLUS) || (k & KEY_MINUS)) end();
				SDL_RenderPresent(sdl::sdl_render);
			}
			redraw(dirs);
		}
		else if(k & KEY_A)
		{
			SDL_Delay(300);
			return dirs[selected];
		}
		else if(k & KEY_PLUS || k & KEY_MINUS) return ":exit:";
	}
}

int main()
{
	Brew::Init();

	sdl_Banner = InitSurface(Banner);
	sdl_CodeEnd = InitSurface(CodeEnd);
	sdl_Information = InitSurface(Information);
	sdl_MainMenu = InitSurface(MainMenu);

	sdlt_Banner = InitTexture(sdl_Banner);
	sdlt_CodeEnd = InitTexture(sdl_CodeEnd);
	sdlt_Information = InitTexture(sdl_Information);
	sdlt_MainMenu = InitTexture(sdl_MainMenu);

	drawfnt25 = TTF_OpenFont("romfs:/Fonts/Roboto-Regular.ttf", 25);
	drawfnt15 = TTF_OpenFont("romfs:/Fonts/Roboto-Regular.ttf", 15);

	string sjscode;
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
	if(dp)
	{
		while(true)
		{
			de = readdir(dp);
			if(de == NULL) break;
			struct stat st;
			string path = string(de->d_name);
			string fullp = "sdmc:/switch/Brew.js/" + path;
			if(stat(fullp.c_str(), &st) == 0)
			{
				if(st.st_mode & S_IFDIR)
				{
					ifstream ifs(fullp + "/package.json");
					if(ifs.good()) prjs.push_back(path);
					ifs.close();
				}
			}
		}
		closedir(dp);
		sort(prjs.begin(), prjs.end());
	}
	string prj = selectprj(prjs);
	if(prj == ":exit:") end();
	string dir = "sdmc:/switch/Brew.js/" + prj;
	ifstream ifs(dir + "/package.json");
	json pack;
	ifs >> pack;
	ifs.close();
	string src = pack["main"].get<string>();
	string res = Brew::EvaluateFile(dir + "/" + src);
	ofstream rst(dir + ".log");
	if(rst.good()) remove(res.c_str());
	rst.close();
	rst.open(dir + ".log");
	rst << res;
	rst.close();
	SDL_RenderClear(sdl::sdl_render);
	DrawBack(sdl_CodeEnd, sdlt_CodeEnd);
	DrawText(drawfnt25, 210, 345, {200, 200, 255, 255}, res.c_str());
	SDL_RenderPresent(sdl::sdl_render);
	while(true)
	{
		hidScanInput();
		int k = hidKeysDown(CONTROLLER_P1_AUTO);
		if((k & KEY_A) || (k & KEY_B)) goto start;
		else if((k & KEY_PLUS) || (k & KEY_MINUS)) break;
	}
	end();
}