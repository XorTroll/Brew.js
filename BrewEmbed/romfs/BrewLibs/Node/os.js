
/**
    @file Brew.js, Node API → Module → os
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module os
    @description [Node API] Operating system's module (Nintendo's Switch OS)
*/

/**
    Character representing the end of line.
    @var {String} EOL
*/
module.exports.EOL = "\n";

/**
    The endianess this OS has. (little endian)
    @function endianess
    @return {String} The endianess
*/
module.exports.endianess = function()
{
    return "LE";
};

/**
    The architecture this OS has.
    @function arch
    @return {String} The architecture
*/
module.exports.arch = function()
{
    return "aarch64";
};

/**
    The platform of this OS.
    @function platform
    @return {String} The platform
*/
module.exports.platform = function()
{
    return "NX64";
};

/**
    Returns the homedir of this OS. (the SD card's root)
    @function homedir
    @return {String} The homedir
*/
module.exports.homedir = function()
{
    return "sdmc:/";
};