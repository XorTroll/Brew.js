
/*  Brew.js example for Nintendo Switch: Touch  */

var input = require("input");
var sdl = require("sdl");
var game = require("game");

// Create a sample text object, with default font and size 30
var text = new sdl.Text("Touch the screen!", 30);
text.show();

// Game's main loop
game.mainLoop(function()
{
    // Get the pressed input
    var key = input.getHeld();
    // If the screen is touched...
    if(key == input.Touch)
    {
        // Get touch data, and set it as text's X and Y coordinates
        var tch = input.getTouch();
        text.x(tch.X);
        text.y(tch.Y);
    }
    if(key == input.Plus) game.exitLoop();
});

// Execution ends