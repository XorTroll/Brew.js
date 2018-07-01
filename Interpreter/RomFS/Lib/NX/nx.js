
/**
    @file Brew.js, Switch API → Module → nx
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module nx
    @description [Switch API] Console's system managing module
*/

/**
    Represents the width of the console's screen.
    @var {Number} ScreenWidth
*/
module.exports.ScreenWidth = 1280;

/**
    Represents the height of the console's screen.
    @var {Number} ScreenHeight
*/
module.exports.ScreenHeight = 720;

/**
    Represents the current language of the system.
    @var {String} Language
*/
module.exports.Language = __nx__language();

/**
    Returns the list of all savedata's title IDs of the system.
    @function savesIDList
    @return {Number[]} A number array with all the title IDs found.
*/
module.exports.savesIDList = function()
{
    var count = __nx__savesIDList_count();
    var ids = [];
    for(var i = 0; i < count; i++)
    {
        var id = __nx__savesIDList(i);
        ids.push(id);
    }
    return ids;
};

/**
    Creates a title ID object from a numeric ID. (only works with titles which are installed, otherwise it will return undefined)
    @class
    @classdesc Class representing a title ID. (only works with titles which are installed)
    @param {Number} ID The numeric title ID to parse.
    @property {Number} ID The numeric title ID of the parsed title.
    @property {Object} NACP The object of the parsed NACP data of the title.
    @property {Object[]} NACP.Entries An object array containing 16 NACP entries of each language. (some of them might not be defined)
    @property {String} NACP.Entries.Name The title's name of a nacp entry.
    @property {String} NACP.Entries.Author The title's author of a nacp entry.
*/
module.exports.Title = function(ID)
{
    this.ID = ID;
    var nacp = {};
    nacp.Version = __nx__Title_nacp_version(ID);
    nacp.Entries = [];
    for(var i = 0; i < 16; i++)
    {
        var nacpen = {};
        nacpen.Name = __nx__Title_nacp_entry_name(ID, i);
        nacpen.Author = __nx__Title_nacp_entry_author(ID, i);
        nacp.Entries.push(nacpen);
    }
    this.NACP = nacp;
};

/**
    Reads the data from a file as a string.
    @example
    var nx = require("nx");
var tit = new nx.Title(0x01007EF00011E000); // BoTW ID
tit.dumpIcon("sdmc:/ZeldaIcon.jpg");
    @param {String} Path The file's path
    @return {String} The file's data, as a string
*/
module.exports.Title.prototype.dumpIcon = function(Path)
{
    __nx__Title_icon(this.ID, Path);
};