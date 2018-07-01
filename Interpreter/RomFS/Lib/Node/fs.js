
/**
    @file Brew.js, Node API → Module → fs
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module fs
    @description [Node API] FileSystem module
*/

/**
    Reads the data from a file as a string.
    @function readFile
    @param {String} Path The file's path
    @example
    var fs = require("fs");
var data = fs.readFile("sdmc:/Text.txt");
    @return {String} The file's data, as a string
*/
module.exports.readFile = function(Path)
{
    var data = __fs__readFile(Path);
    return data;
}

/**
    Writes text to a file.
    @function writeFile
    @param {String} Path The file's path
    @param {String} Data The text to write to the file
    @example
    var fs = require("fs");
fs.writeFile("sdmc:/Text.txt", "HelloFromTextFile!");
*/
module.exports.writeFile = function(Path, Data)
{
    __fs__writeFile(Path, Data);
}

/**
    Checks whether a file exists or not.
    @function exists
    @param {String} Path The file's path
    @example
    var fs = require("fs");
var exist = fs.exists("sdmc:/Text.txt");
    @return {Boolean} Whether the file exists or not
*/
module.exports.exists = function(Path)
{
    var ex = __fs__exists(Path);
    return ex;
};

/**
    Creates a directory. (if it doesn't exist already)
    @function mkdir
    @param {String} Dir The directory to create
    @example
    var fs = require("fs");
fs.mkdir("sdmc:/NewDir");
*/
module.exports.mkdir = function(Dir)
{
    __fs__mkdir(Dir);
};

/**
    Deletes/removes a directory. (if it exists)
    @function rmdir
    @param {String} Dir The directory to delete
    @example
    var fs = require("fs");
fs.rmdir("sdmc:/switch");
*/
module.exports.rmdir = function(Dir)
{
    __fs__rmdir(Dir);
};