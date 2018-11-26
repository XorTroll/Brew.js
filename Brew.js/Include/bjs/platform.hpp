
/**

    @file Platform.hpp
    @brief Platform detector and specifier for Brew.js
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#if defined bjsPlatformLibNX
	#define bjsPlatformIndex 0
	#include <switch.h>
#elif defined bjsPlatformLibCTRU
	#define bjsPlatformIndex 1
	#include <3ds.h>
	#include <citro2d.h>
	#include <citro3d.h>
#else
	#if !defined bjsPlatformIndex
		#ifdef __SWITCH__
			#define bjsPlatformLibNX
			#define bjsPlatformIndex 0
			#include <switch.h>
		#elif defined ARM11 || defined _3DS
			#define bjsPlatformLibCTRU
			#define bjsPlatformIndex 1
			#include <3ds.h>
			#include <citro2d.h>
			#include <citro3d.h>
		#else
			#error "No platform was detected or specified. Define 'bjsPlatformLibCTRU' or 'bjsPlatformLibNX' to select a platform."
		#endif
	#endif
#endif

namespace bjs
{
	enum class Platform
	{
		LibNX,
		LibCTRU,
	};

	Platform GetCurrentPlatform();
}