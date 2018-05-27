
// Brew.js example for Nintendo Switch: PONG game (a bit poor)

// We set the background to a greenish blue color.
gfx.setBackground({ R: 32, G: 128, B: 255 });

// This vars will hold the scores.
var p1 = 0;
var p2 = 0;

// This will print the score.
var text = gfx.text.create("Score", 400, 300, 1, { R: 200, G: 200, B: 255 }, gfx.text.InterUIMedium28); 
text.show();

// We create the player 1 bar. (a rectangle)
var bar1 = gfx.object.create({ R: 255, G: 255, B: 255 }, 75, 400);
bar1.x(100);
bar1.y(200);
bar1.show();

// We create the player 2 bar. (a rectangle again)
var bar2 = gfx.object.create({ R: 255, G: 255, B: 255 }, 75, 400);
bar2.x(1105);
bar2.y(200);
bar2.show();

// We create the ball, a small square.
var ball = gfx.object.create({ R: 255, G: 255, B: 255 }, 50, 50);
ball.depth(1);
ball.x(1280 / 2);
ball.y(720 / 2);

// We set its speeds in order to achieve a random direction to start with.
var rnd = device.randomNumber(-6, 6);
ball.verticalSpeed(ball.verticalSpeed() + rnd);
ball.horizontalSpeed(15);
ball.show();

// The function to restart the game. It doesn't wait any seconds...
function restart()
{
    ball.x(1280 / 2);
    ball.y(720 / 2);
    var rnd = device.randomNumber(-6, 6);
    ball.verticalSpeed(ball.verticalSpeed() + rnd);
    ball.horizontalSpeed(15);
}

game.mainLoop(function()
{
    // We check the held inputs. L and ZL move the left bar, while R and ZR move the right bar.
    var key = input.checkHeld();
    if(key == input.L)
    {
        var cy = bar1.y();
        if(!bar1.checkTopCollision()) bar1.y(cy - 10);
    }
    else if(key == input.ZL)
    {
        var cy = bar1.y();
        if(!bar1.checkBottomCollision()) bar1.y(cy + 10);
    }
    else if(key == input.R)
    {
        var cy = bar2.y();
        if(!bar2.checkTopCollision()) bar2.y(cy - 10);
    }
    else if(key == input.ZR)
    {
        var cy = bar2.y();
        if(!bar2.checkBottomCollision()) bar2.y(cy + 10);
    }
    else if(key == input.Plus) game.exitLoop();
    // If the ball collides with any of the bars...
    if(ball.checkCollision(bar1) || ball.checkCollision(bar2))
    {
        var rnd = device.randomNumber(-6, 6);
        ball.verticalSpeed(ball.verticalSpeed() + rnd);
        ball.horizontalSpeed(-ball.horizontalSpeed());
    }
    // If the ball collides with the top or bottom part of the screen...
    if(ball.checkTopCollision() || ball.checkBottomCollision())
    {
        ball.verticalSpeed(-ball.verticalSpeed());
    }
    // If the ball collides with the left part of the screen...
    else if(ball.checkLeftCollision())
    {
        p2++;
        restart();
    }
    // If the ball collides with the right part of the screen...
    else if(ball.checkRightCollision())
    {
        p1++;
        restart();
    }
    // Update every frame the score.
    text.content("Score: " + p1 + " - " + p2);
});