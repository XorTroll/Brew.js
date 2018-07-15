
/*  Brew.js example for Nintendo Switch: AudioPlayer  */

var sdl = require("sdl");
var input = require("input");
var game = require("game");

// Greenish blue background
sdl.setBackground({ R: 0, G: 128, B: 255, A: 255 });

// Information text
var txt = new sdl.Text("Press A to play/stop.\nPress B to pause/resume.\nPress X to restart.\nEnjoy scripting!", "romfs:/Fonts/Roboto-Regular.ttf", 35);
txt.color({ R: 255, G: 255, B: 255, A: 200 });
txt.show();

// Open audio file: Homebrew Channel Theme ♪♪♪
sdl.openAudio(__dirname + "/Theme.mp3");

// Game's main loop
game.mainLoop(function()
{
    var key = input.getPressed();
    // Play/stop audio if A is pressed
    if(key == input.A)
    {
        var play = sdl.playingAudio();
        if(play) sdl.stopAudio();
        else sdl.playAudio(false);
    }
    // Pause/resume audio if B is pressed
    else if(key == input.B)
    {
        var pause = sdl.pausedAudio();
        if(pause) sdl.resumeAudio();
        else sdl.pauseAudio();
    }
    // Restart audio if X is pressed
    else if(key == input.X)
    {
        sdl.restartAudio();
    }
    else if(key == input.Plus) game.exitLoop();
});
