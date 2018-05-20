
// Brew.js example for Nintendo Switch: File System

// Our file, located at the SD card, same as "sdmc:/sample.txt".
var ourfile = fs.pathSDMC("sample.txt");
// We create this file (if doesn't exist) and write down the platform's name.
fs.writeText(ourfile, device.PlatformName);
// If the previous file exists (obviously, it does)
if(fs.exists(ourfile))
{
    // Same as above, but with a new file
    var ourfile2 = fs.pathSDMC("sample2.txt");
    // We write that ourfile exists to ourfile2.
    fs.writeText(ourfile2, "ourfile exists!");
    // We read the text ourfile has, because we know it exists.
    var ourfiletext = fs.readText(ourfile);
    // We create a text object with that text (check HelloWorld example for simple drawing)
    var filetext = gfx.createText(ourfiletext, gfx.font.Tahoma, 15, 20, 20, 255, 255, 255);
    // And we show it.
    gfx.showText(filetext);
}

// Main loop
while(true)
{
    // Flush data on main loop, should ALWAYS be done to avoid lag or gfx errors.
    game.flush();
}

// Execution ends