
/**
    @file Brew.js, Switch API → Module → input
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module input
    @description [Switch API] Hid inputs managing module
*/

/**
    Represents when no key is pressed.
    @var {Number} None
*/
module.exports.None = 0;

/**
    Represents when the A button is pressed.
    @var {Number} A
*/
module.exports.A = 1;

/**
    Represents when the B button is pressed.
    @var {Number} B
*/
module.exports.B = 2;

/**
    Represents when the X button is pressed.
    @var {Number} X
*/
module.exports.X = 4;

/**
    Represents when the Y button is pressed.
    @var {Number} Y
*/
module.exports.Y = 8;

/**
    Represents when the L stick is pressed.
    @var {Number} LStick
*/
module.exports.LStick = 16;

/**
    Represents when the R stick is pressed.
    @var {Number} RStick
*/
module.exports.RStick = 32;

/**
    Represents when the L button is pressed.
    @var {Number} L
*/
module.exports.L = 64;

/**
    Represents when the R button is pressed.
    @var {Number} R
*/
module.exports.R = 128;

/**
    Represents when the ZL button is pressed.
    @var {Number} ZL
*/
module.exports.ZL = 256;

/**
    Represents when the ZR button is pressed.
    @var {Number} ZR
*/
module.exports.ZR = 512;

/**
    Represents when the + button is pressed.
    @var {Number} Plus
*/
module.exports.Plus = 1024;

/**
    Represents when the - button is pressed.
    @var {Number} Minus
*/
module.exports.Minus = 2048;

/**
    Represents when the D-pad is moved left.
    @var {Number} DPadLeft
*/
module.exports.DPadLeft = 4096;

/**
    Represents when the D-pad is moved up.
    @var {Number} DPadUp
*/
module.exports.DPadUp = 8192;

/**
    Represents when the D-pad is moved right.
    @var {Number} DPadRight
*/
module.exports.DPadRight = 16384;

/**
    Represents when the D-pad is moved down.
    @var {Number} DPadDown
*/
module.exports.DPadDown = 32768;

/**
    Represents when the left stick is moved left.
    @var {Number} LStickLeft
*/
module.exports.LStickLeft = 65536;

/**
    Represents when the left stick is moved up.
    @var {Number} LStickUp
*/
module.exports.LStickUp = 131072;

/**
    Represents when the left stick is moved right.
    @var {Number} LStickRight
*/
module.exports.LStickRight = 262144;

/**
    Represents when the left stick is moved left.
    @var {Number} LStickLeft
*/
module.exports.LStickDown = 524288;

/**
    Represents when the right stick is moved left.
    @var {Number} RStickLeft
*/
module.exports.RStickLeft = 1048576;

/**
    Represents when the right stick is moved left.
    @var {Number} RStickLeft
*/
module.exports.RStickUp = 2097152;

/**
    Represents when the right stick is moved right.
    @var {Number} RStickRight
*/
module.exports.RStickRight = 4194304;

/**
    Represents when the right stick is moved down.
    @var {Number} RStickDown
*/
module.exports.RStickDown = 8388608;

/**
    Represents when the SL button is pressed.
    @var {Number} SL
*/
module.exports.SL = 16777216;

/**
    Represents when the SR button is pressed.
    @var {Number} SR
*/
module.exports.SR = 33554432;

/**
    Represents when the screen is touched.
    @var {Number} Touch
*/
module.exports.Touch = 67108864;

/**
    Represents when any directional button is moved up. (D-pad, JoySticks...)
    @var {Number} Up
*/
module.exports.Up = 8192 | 131072 | 2097152;

/**
    Represents when any directional button is moved down. (D-pad, JoySticks...)
    @var {Number} Down
*/
module.exports.Down = 32768 | 524288 | 8388608;

/**
    Represents when any directional button is moved left. (D-pad, JoySticks...)
    @var {Number} Left
*/
module.exports.Left = 4096 | 65536 | 1048576;

/**
    Represents when any directional button is moved right. (D-pad, JoySticks...)
    @var {Number} Right
*/
module.exports.Right = 16384 | 262144 | 4194304;

/**
    Gets the input being held.
    @function getHeld
    @example
    var input = require("input");
var ipt = input.getHeld();
if(ipt == input.A)
{
    // A is held
}
    @return {Number} The held input
*/
module.exports.getHeld = function()
{
    var player = 10;
    if(arguments.length >= 1)
    {
        player = arguments[0];
    }
    var key = __input__getHeld(player);
    return key;
};

/**
    Gets the input being pressed.
    @function getPressed
    @example
    var input = require("input");
var ipt = input.getPressed();
if(ipt == input.B)
{
    // B is pressed
}
    @return {Number} The held pressed
*/
module.exports.getPressed = function()
{
    var player = 10;
    if(arguments.length >= 1)
    {
        player = arguments[0];
    }
    var key = __input__getPressed(player);
    return key;
};

/**
    Gets the input being released.
    @function getReleased
    @example
    var input = require("input");
var ipt = input.getReleased();
if(ipt == input.X)
{
    // X is released
}
    @return {Number} The released input
*/
module.exports.getReleased = function()
{
    var player = 10;
    if(arguments.length >= 1)
    {
        player = arguments[0];
    }
    var key = __input__getReleased(player);
    return key;
};

/**
    Gets the number of touches being pressed.
    @function getTouchNumber
    @example
    var input = require("input");
var tchs = input.getTouchNumber();
for(var i = 0; i < tchs; i++)
{
    var ctch = input.getTouch(i); // Get all the touches
}
    @return {Number} The number of touches
*/
module.exports.getTouchNumber = function()
{
    var tchno = __input__getTouchNumber();
    return tchno;
};

/**
    Gets the first/only touch being pressed.
    @function getTouch
    @param {Number} [TouchNumber] The touch you want to get, otherwise it will be the first one.
    @example
    var input = require("input");
var tch = input.getTouch(); // same as "getTouch(0);"
var tx = tch.X;
var ty = tch.Y;
    @return {Object} The touch object
*/
module.exports.getTouch = function()
{
    var num = 0;
    if(arguments.length >= 1) num = arguments[0];
    var tch = {};
    tch.X = __input__getTouch_x(num);
    tch.Y = __input__getTouch_y(num);
    return tch;
};

/**
    Gets all the touches being pressed.
    @function getTouches
    @example
    var input = require("input");
var tchs = input.getTouches();
var tch0 = tchs[0]; // The first touch
    @return {Object[]} The touch objects
*/
module.exports.getTouches = function()
{
    var tch = [];
    var tchno = __input__getTouchNumber();
    for(var i = 0; i < tchno; i++)
    {
        var ctch = {};
        ctch.X = __input__getTouch_x(i);
        ctch.Y = __input__getTouch_y(i);
        tch[i] = ctch;
    }
    return tch;
};