
// Brew.js example for Nintendo Switch: Inputs

// We enter the main loop directly in this case
while(true)
{
    // We get the pressed input
    var key = input.checkPressed();
    // If the + (plus) button is held...
    if(key == input.Plus)
    {
        // We stop the main loop, so the code ends
        break;
    }
    // Flush data on main loop, should ALWAYS be done to avoid lag or gfx errors
    game.flush();
}

// Execution ends