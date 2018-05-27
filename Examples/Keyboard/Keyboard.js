
// Brew.js example for Nintendo Switch: Keyboard

// We set the background with a full blue color
gfx.setBackground({ R: 0, G: 0, B: 255 });
// We create the text which will hold the data.
var text = gfx.text.create("Press A to enter input.", 10, 10, 0, { R: 255, G: 64, B: 0 }, gfx.text.Tahoma24);
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
        // We open the keyboard to ask for input.
        var txt = gfx.dialog.keyboardInput();
        // We set it as the text's text.
        text.content(txt);
    }
    // If + is pressed, the main loop ends.
    else if(key == input.Plus) game.exitLoop();
});

// Execution ends