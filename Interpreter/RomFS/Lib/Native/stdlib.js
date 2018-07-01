
/**
    @file Brew.js, Native API → Module → stdlib
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module stdlib
    @description [Native API] Stdlib module (wrapper of "stdlib.h" C library or "cstdlib" C++ library)
*/

/**
    Represents a NULL value, which is equivalent to (void*)0 in C, and just 0 or 0L in C++.
    @var {Number} NULL
*/
module.exports.NULL = 0;

/**
    Returns a random number.
    @function rand
    @return {Number} The random number
    @example
    var stdlib = require("stdlib");
var num = stdlib.rand();
*/
module.exports.rand = function()
{
    var rnd = __stdlib__rand();
    return rnd;
};

/**
    Converts a string to a floating number
    @function atof
    @param {String} Text The string to convert
    @return {Number} The converted number
    @example
    var stdlib = require("stdlib");
var num = stdlib.atof("3.1415");
*/
module.exports.atof = function(Text)
{
    var f = __stdlib__atof(Text);
    return f;
};

/**
    Converts a string to an integer
    @function atoi
    @param {String} Text The string to convert
    @return {Number} The converted number
    @example
    var stdlib = require("stdlib");
var num = stdlib.atof("50");
*/
module.exports.atoi = function(Text)
{
    var i = __stdlib__atoi(Text);
    return i;
};

/**
    Exits current script execution.
    @function exit
    @param {Number} Code The code to exit with, recommended to use 0
    @example
    var stdlib = require("stdlib");
stdlib.exit(0); // success exit code
*/
module.exports.exit = function(Code)
{
   __stdlib__exit(Code);
}