
// Brew.js example for Nintendo Switch: Inputs

// We create a text object (check HelloWorld example for simple drawing)
var filetext = gfx.text.create("Input!", 10, 10, 0, { R: 0, G: 0, B: 0 }, gfx.text.Tahoma12);
// And we show it.
filetext.show();
// Main loop
game.mainLoop(function()
{
    // We get the held input.
    var key = input.checkHeld();
    // We set the pressed key (its numeric id) as the text of the text.
    filetext.content(key);
    // If the + (plus) button is held we stop the main loop.
    if(key == input.Plus) game.exitLoop();
});

// Execution ends.