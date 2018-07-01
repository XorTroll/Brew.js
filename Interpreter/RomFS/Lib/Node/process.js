
/**
    @file Brew.js, Node API → Module → process
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module process
    @description [Node API] Processes managing module
*/

/**
    The architecture of the current system. (same as os.arch())
    @var {String} arch
*/
module.exports.arch = "aarch64";

/**
    Returns the current working directory.
    @function cwd
    @return {String} The current working directory
*/
module.exports.cwd = function()
{
    var ccwd = "sdmc:/";
    return ccwd;
};

/**
    Exits current script execution.
    @function exit
    @param {Number} Code The code to exit with, recommended to use 0
    @example
    var process = require("process");
process.exit(0); // success exit code
*/
module.exports.exit = function(Code)
{
    __process__exit(Code);
};