
// Brew.js example for Nintendo Switch: Sprites

// The path to the logo image, at the project's directory.
var logopath = game.ProjectDir + "/Brew.png";
// We create the logo using that path.
var logo = gfx.spriteFromPNG(logopath);
// We set X and Y coords to 20.
gfx.setSpriteXY(logo, 20, 20);
// We show the image. PNG allows RGBA!
gfx.showSprite(logo);

// Main loop
while(true)
{
    // Flush data on main loop, should ALWAYS be done to avoid lag or gfx errors
    game.flush();
}

// Execution ends