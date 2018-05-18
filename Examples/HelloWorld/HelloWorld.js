console.init(); // Start console
console.logLine("Hello world from JS!"); // prints single line (+ line) to screen
console.logLine("Current platform: ", device.Platform, ", also known as ", device.PlatformName) // Print platform data

while(true)
{
    game.flush(); // flush data on main loop
}