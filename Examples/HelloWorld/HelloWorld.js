
// Brew.js example for Nintendo Switch: Hello World

// We set the background with a full blue color
gfx.setBackground({ R: 0, G: 0, B: 255 });
// We create the text with text, X and Y coords, depth, color and font.
var text = gfx.text.create("Hello world!", 10, 10, 0, { R: 255, G: 64, B: 0 }, gfx.text.Tahoma24);
// We show the text.
text.show();

// Main loop
game.mainLoop(function()
{
    // We check pressed input.
    var key = input.checkPressed();
    // We get current X and Y values of our text.
    if(key == input.A)
    {
        // We create a random color and set it to the text.
        var r = device.randomNumber(0, 255);
        var g = device.randomNumber(0, 255);
        var b = device.randomNumber(0, 255);
        var color = { R: r, G: g, B: b };
        text.color(color);
    }
    // If + is pressed, the main loop ends.
    else if(key == input.Plus) game.exitLoop();
});

// Execution ends