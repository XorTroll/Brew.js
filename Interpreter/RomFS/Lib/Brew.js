
/*
    Brew.js API - JavaScript lib
    Entry script - the one loaded by the interpreter
*/

var module = {};
module.exports = {};

module.stdio = "stdio";
module.stdlib = "stdlib";

module.fs = "fs";
module.os = "os";
module.path = "path";
module.process = "process";

module.game = "game";
module.sdl = "sdl";
module.input = "input";
module.nx = "nx";

var randRange = function()
{
    var rand = 0;
    if(arguments.length >= 2)
    {
        var min = arguments[0];
        var max = arguments[1];
        rand = Math.floor(Math.random() * (max - min + 1)) + min;
    }
    else if(arguments.length == 1)
    {
        var max = arguments[0];
        rand = Math.floor(Math.random() * (max + 1));
    }
    return rand;
};

var require = function(Module)
{
    var path = Module;
    switch(Module.toLowerCase())
    {
        case "stdio":
        case "stdlib":
            path = "Native/" + Module.toLowerCase() + ".js";
            break;

        case "fs":
        case "os":
        case "path":
        case "process":
            path = "Node/" + Module.toLowerCase() + ".js";
            break;
        case "game":
        case "sdl":
        case "input":
        case "nx":
            path = "NX/" + Module.toLowerCase() + ".js";
            break;
    }
    module.exports = {};
    eval(__libAccessJS(path));
    var mod = module.exports;
    module.exports = {};
    return mod;
};