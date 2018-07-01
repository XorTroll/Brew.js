
/**
    @file Brew.js, Switch API → Module → game
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module game
    @description [Switch API] Game's execution managing module
*/

module.exports.__loop = false;

/**
    Starts the application's main loop.
    @function mainLoop
    @param {Function} Loop The function to execute at each iteration of the game loop.
    @example
    var game = require("game");
game.mainLoop(function()
{
    // this will be executed every iteration
});
*/
module.exports.mainLoop = function(Loop)
{
    module.exports.__loop = true;
    while(module.exports.__loop)
    {
        __flush();
        Loop();
    }
};

/**
    Exits the application's main loop.
    @function exitLoop
    @example
    var game = require("game");
game.mainLoop(function()
{
    game.exitLoop(); // only one iteration, it exists instantly
});
*/
module.exports.exitLoop = function()
{
    module.exports.__loop = false;
};