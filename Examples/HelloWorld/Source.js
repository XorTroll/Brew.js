
/*  Brew.js example for Nintendo Switch: HelloWorld  */

var input = require("input");
var sdl = require("sdl");
var game = require("game");

// Create the Hello world text object
var text = new sdl.Text("Hello world!", 30);
text.show();

// Game's main loop
game.mainLoop(function()
{
    var key = input.getHeld();
    // If a is held...
    if(key == input.A)
    {
        // Get current X and Y values of our text
        var currentx = text.x();
        var currenty = text.y();
        // Set the new X and Y values, the current ones plus 1
        text.x(currentx + 1);
        text.y(currenty + 1);
    }
    else if(key == input.Plus) game.exitLoop();
});

// Execution ends