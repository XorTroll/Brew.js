
/**
    @file Brew.js, Node API → Module → path
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module path
    @description [Node API] Path managing module
*/

/**
    Commom delimiter of paths.
    @var {String} delimiter
 */
module.exports.delimiter = ":";

/**
    Common separator of paths.
    @var {String} sep
 */
module.exports.sep = "/";

/**
    Joins different paths to form a new path.
    @function join
    @param {...String} Paths All the paths to join
    @return {String} The new formed path
    @example
    var path = require("path");
var newpath = path.join("sdmc:", "switch/", "Brew.js"); // should return "sdmc:/switch/Brew.js"
*/
module.exports.join = function()
{
    var pth;
    for(var i = 0; i < arguments.length; i++)
    {
        var arg = arguments[i];
        if(typeof arg == "string")
        {
            var lch = arg.slice(-1);
            if(lch == "/") pth += arg;
            else if(lch == "\\") pth += arg.slice(0, -1);
            else pth += arg + "/";
        }
    }
    if(pth.slice(-1) == "/") pth = pth.slice(0, -1);
    return pth;
};

// TODO:
// module.exports.basename(Path)
// module.exports.dirname(Path)
// module.exports.extname(Path)
// module.exports.isAbsolute(Path)
// module.exports.normalize(Path)
// module.exports.parse(Path)