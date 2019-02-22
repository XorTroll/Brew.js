
/**

    @file Brew.js
    @brief Main header of Brew.js
    @author XorTroll
    @copyright Brew.js project

*/

/*

	 * Brew.js - C++ high-level JavaScript API for Nintendo 3DS/Switch *

	Using duktape engine (https://duktape.org), base of the JavaScript API.

	FAQ:
	
	- Which ECMAScript version if supported?
	  According to duktape's documentation, duktape's JavaScript runs like ES5 (although some API of ES5 is not supported).
	  It also has some ES6 features. read more about this on duktape's GitHub: https://github.com/svaarala/duktape/blob/master/README.md

	- Which Nintendo OSs are supported?
	  Currently Brew.js runs under Nintendo Switch, Nintendo 3DS and Nintendo DS. More platforms are planned, like Wii or GBA,
	  but we plan to fully support these 3 current OSs before starting with a new one.

	- Do I have to add something extra to embed Brew.js on my project?
	  Theoretically no. Brew.js automatically should detect the current OS we're compiling for, and it'll error if it fails detecting it.
	  Everything should be automatically done.

	- Where can I get more info or docs?
	  You have the documentations and the repository here: https://XorTroll.github.io/Brew.js
	
*/

#pragma once
#include <bjs/js.hpp>
#include <bjs/lib.hpp>
#include <bjs/platform.hpp>
#include <bjs/Environment.hpp>