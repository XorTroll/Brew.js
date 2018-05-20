
// Brew.js example for Nintendo Switch: Hello World

/*
    No need to call something like "gfx.init()", gfx already started

    We create the text with text "Hello world!",
    using Tahoma font (see API for more fonts),
    with size 15 (Tahoma only allows 12 and 24 sizes, so if it's bigger than 12 24 is used),
    coordinates X: 20 and Y: 20,
    and RGB color green (R: 0, G: 255, B: 0).
*/
var text = gfx.createText("Hello world!", gfx.font.Tahoma, 15, 20, 20, 0, 255, 0);
// We show the text
gfx.showText(text);

// Main loop
while(true)
{
    // Flush data on main loop, should ALWAYS be done to avoid lag or gfx errors
    game.flush();
}

// Execution ends