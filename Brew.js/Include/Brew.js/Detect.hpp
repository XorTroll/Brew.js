
/**

    @file Detect.hpp
    @brief OS detector of Brew.js - only add platform-specific modules
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#if defined __SWITCH__
	#define __curOS 0
	#include <switch.h>
	#include <SDL2/SDL.h>
#elif defined _3DS || defined ARM11
	#define __curOS 1
	#include <3ds.h>
	#include <citro2d.h>
	#include <citro3d.h>
	#define R_SUCCEEDED(res) ((res) == 0)
	#define R_FAILED(res) ((res) != 0)
	#define R_MODULE(res) ((res) & 0x1FF)
	#define R_DESCRIPTION(res) (((res) >> 9) & 0x1FFF)
	#define MAKERESULT(module, description) ((((module) & 0x1FF)) | ((description) & 0x1FFF) << 9)
#elif defined ARM9
	#define __curOS 2
	#include <nds.h>
	#include <fat.h>
	#include <filesystem.h>
	#define R_SUCCEEDED(res) ((res) == 0)
	#define R_FAILED(res) ((res) != 0)
	#define R_MODULE(res) ((res) & 0x1FF)
	#define R_DESCRIPTION(res) (((res) >> 9) & 0x1FFF)
	#define MAKERESULT(module, description) ((((module) & 0x1FF)) | ((description) & 0x1FFF) << 9)
	typedef u32 Result;
#else
	#error "No OS was found.\n__SWITCH__ should be defined with libnx, ARM11 or _3DS with libctru, ARM9 with libnds."
#endif