
// Brew.js example for Nintendo Switch: Sprites

// The path to the logo image, at the project's directory.
var logopath = game.ProjectDir + "/Brew.png";
// We create the logo using that path.
var logo = gfx.object.create(logopath);
// We set X and Y coords to 20.
logo.x(20);
logo.y(20);
// We show the image.
logo.show();

// Main loop
game.mainLoop(function()
{

});

// Execution ends