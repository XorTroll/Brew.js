
/*  Brew.js example for Nintendo Switch: FileSystem  */

var fs = require("fs");
var os = require("os");
var input = require("input");
var sdl = require("sdl");
var game = require("game");

// Our file, located at the SD card
var ourfile = "sdmc:/sample.txt";
// We create this file (if doesn't exist) and write down the Switch's architecture ("aarch64")
fs.writeFile(ourfile, os.arch());
// If the previous file exists (obviously, it does)
if(fs.exists(ourfile))
{
    // Same as above, but with a new file
    var ourfile2 = "sdmc:/sample2.txt";
    // We write that ourfile exists to ourfile2
    fs.writeFile(ourfile2, "ourfile exists!");
    // We read the text ourfile has, because we know it exists
    var ourfiletext = fs.readFile(ourfile);
    // We create a text with that text
    var filetext = new sdl.Text(ourfiletext, 20);
    // And we show it.
    filetext.show();
}

// Game's main loop
game.mainLoop(function()
{
    var key = input.getPressed();
    if(key == input.Plus) game.exitLoop();
});

// Execution ends