
/*  Brew.js example for Nintendo Switch: PONG  */

var sdl = require("sdl");
var game = require("game");
var input = require("input");

// This vars will hold the scores.
var p1 = 0;
var p2 = 0;

sdl.setFPS(80);

// This will print the score.
var text = new sdl.Text("Score", 30);
text.x(300);
text.y(300);
text.depth(1);
text.color({ R: 60, G: 60, B: 60, A: 255 });
text.show();

// We create the player 1 bar. (a rectangle)
var bar1 = new sdl.Object(__dirname + "/Bar1.png");
bar1.x(100);
bar1.y(200);
bar1.show();

// We create the player 2 bar. (a rectangle again)
var bar2 = new sdl.Object(__dirname + "/Bar2.png");
bar2.x(1105);
bar2.y(200);
bar2.show();

// We create the ball, a small square.
var ball = new sdl.Object(__dirname + "/Ball.png");
ball.usesPhysics(true);
ball.depth(1);
ball.x(640);
ball.y(360);
// We set its speeds in order to achieve a random direction to start with.
var rnd = randRange(-2, 2);
ball.vspeed(rnd);
ball.hspeed(3);
ball.show();

// The function to restart the game. It doesn't wait any seconds...
function restart()
{
    ball.x(1280 / 2);
    ball.y(720 / 2);
    var rnd = randRange(-2, 2);
    ball.vspeed(ball.vspeed() + rnd);
    ball.hspeed(3);
}

game.mainLoop(function()
{
    // Get held inputs: L and ZL move the left bar, while R and ZR move the right bar.
    var key = input.getHeld();
    if(key == input.L)
    {
        var cy = bar1.y();
        if(!bar1.topCollide()) bar1.y(cy - 4);
    }
    else if(key == input.ZL)
    {
        var cy = bar1.y();
        if(!bar1.bottomCollide()) bar1.y(cy + 4);
    }
    else if(key == input.R)
    {
        var cy = bar2.y();
        if(!bar2.topCollide()) bar2.y(cy - 4);
    }
    else if(key == input.ZR)
    {
        var cy = bar2.y();
        if(!bar2.bottomCollide()) bar2.y(cy + 4);
    }
    else if(key == input.Plus) game.exitLoop();

    // If the ball collides with any of the bars...
    if(ball.checkCollide(bar1))
    {
        var rnd = randRange(-2, 2);
        ball.vspeed(ball.vspeed() + rnd);
        if(ball.hspeed() < 0) ball.hspeed(-ball.hspeed());
    }
    else if(ball.checkCollide(bar2))
    {
        var rnd = randRange(-2, 2);
        ball.vspeed(ball.vspeed() + rnd);
        if(ball.hspeed() > 0) ball.hspeed(-ball.hspeed());
    }
    // If the ball collides with the top or bottom part of the screen...
    if(ball.topCollide() || ball.bottomCollide())
    {
        ball.vspeed(-ball.vspeed());
    }
    // If the ball collides with the left part of the screen...
    else if(ball.leftCollide())
    {
        p2++;
        restart();
    }
    // If the ball collides with the right part of the screen...
    else if(ball.rightCollide())
    {
        p1++;
        restart();
    }
    // Update every frame the score.
    text.text("Score: " + p1 + " - " + p2);
});