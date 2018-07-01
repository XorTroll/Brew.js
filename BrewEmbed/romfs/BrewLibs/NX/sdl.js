
/**
    @file Brew.js, Switch API → Module → sdl
    @version 1.0
    @author XorTroll
    @copyright 2018
    @module sdl
    @description [Switch API] Graphics management module, using SDL2 library
*/

/**
    Sets the screen's background to a image.
    @function setBackground
    @param {String} Path The path to the image ti set as background.
*/

/**
    Sets the screen's background to a screen color (R, G, B, A).
    @function setBackground
    @param {Object[]} Color The RGBA color to use as plain background.
    @param {Number} Color.R The red part of the RGA color.
    @param {Number} Color.G The green part of the RGA color.
    @param {Number} Color.B The blue part of the RGA color.
    @param {Number} Color.A The alpha channel of the RGA color.
    @example
    var sdl = require("sdl");
sdl.setBackground({ R: 255, G: 0, B: 0, A: 255 }); // Completely red background
*/
module.exports.setBackground = function()
{
    if(arguments.length >= 1)
    {
        var arg = arguments[0];
        if(typeof arg == "string")
        {
            __sdl__setBackground(arg);
        }
        else
        {
            var cr = arg.R;
            var cg = arg.G;
            var cb = arg.B;
            var ca = arg.A;
            __sdl__setBackground(cr, cg, cb, ca);
        }
    }
}

/**
    Sets the gravity for the elements which use physics.
    @function setGravity
    @param {Number} Gravity The numeric value of the new gravity.
    @example
    var sdl = require("sdl");
sdl.setGravity(2); // By default is 0, no decimal numbers are supported for the moment
*/
module.exports.setGravity = function(Gravity)
{
    __sdl__setGravity(Gravity);
}

/**
    Sets the FPS value for rendering.
    @function setFPS
    @param {Number} FPS The new FPS numeric value to set.
    @example
    var sdl = require("sdl");
sdl.setFPS(30); // 60 by default, beware of lag!
*/
module.exports.setFPS = function(FPS)
{
    __sdl__setFPS(FPS);
}

/**
    Creates a graphic object as a text.
    @class
    @classdesc Class representing a graphic text object.
    @param {String} Text The text string which will be shown.
    @param {String} [FontPath] The path to the TTF font, otherwise Roboto will be used.
    @param {Number} [FontSize] The size of the font, 20 by default.
    @property {Number} ID The internal ID of the element, a random number.
*/
module.exports.Text = function()
{
    var argc = arguments.length;
    if(argc == 1)
    {
        var txt = arguments[0];
        var fnt = "romfs:/Fonts/Roboto-Regular.ttf";
        var fntsize = 20;
        this.ID = __sdl__Text_create(txt, 0, 0, 0, 0, 0, 0, 255, fnt, fntsize);
    }
    else if(argc >= 2)
    {
        var txt = arguments[0];
        var fnt = "romfs:/Fonts/Roboto-Regular.ttf";
        var fntsize = arguments[1];
        this.ID = __sdl__Text_create(txt, 0, 0, 0, 0, 0, 0, 255, fnt, fntsize);
    }
    else if(argc >= 3)
    {
        var txt = arguments[0];
        var fnt = arguments[1];
        var fntsize = arguments[2];
        this.ID = __sdl__Text_create(txt, 0, 0, 0, 0, 0, 0, 255, fnt, fntsize);
    }
};

/**
    Shows the text, if it's hidden.
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.show();
*/
module.exports.Text.prototype.show = function()
{
    __sdl__Text_show(this.ID);
};

/**
    Hides the text, if it's shown.
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.hide();
*/
module.exports.Text.prototype.hide = function()
{
    __sdl__Text_hide(this.ID);
};

/**
    Returns whether the text is shown or not.
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
var sh = txt.isShown();
    @return {Boolean} Whether it's shown or not.
*/
module.exports.Text.prototype.isShown = function()
{
    var sh = __sdl__Text_isShown(this.ID);
    return sh;
};

/**
    Gets or sets the X coordinate of the text.
    @param {Number} [NewX] The new X coordinate to set
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.x(50); // Set X to 50
var cx = txt.x(); // Get current X, should return 50
    @return {Number} The current X coordinate, if no argument was passed.
*/
module.exports.Text.prototype.x = function()
{
    if(arguments.length >= 1)
    {
        var x = arguments[0];
        __sdl__Text_x(this.ID, x);
    }
    else
    {
        var x = __sdl__Text_x(this.ID);
        return x;
    }
};

/**
    Gets or sets the Y coordinate of the text.
    @param {Number} [NewY] The new Y coordinate to set
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.y(50); // Set Y to 10
var cy = txt.y(); // Get current Y, should return 10
    @return {Number} The current Y coordinate, if no argument was passed.
*/
module.exports.Text.prototype.y = function()
{
    if(arguments.length >= 1)
    {
        var y = arguments[0];
        __sdl__Text_y(this.ID, y);
    }
    else
    {
        var y = __sdl__Text_y(this.ID);
        return y;
    }
};

/**
    Gets or sets the depth of the text.
    @param {Number} [NewDepth] The new depth to set
        @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.depth(3); // Set depth layer to 3
var dp = txt.depth(); // Get current depth layer, should return 3
    @return {Number} The current depth, if no argument was passed.
*/
module.exports.Text.prototype.depth = function()
{
    if(arguments.length >= 1)
    {
        var dpth = arguments[0];
        __sdl__Text_depth(this.ID, dpth);
    }
    else
    {
        var dpth = __sdl__Text_depth(this.ID);
        return dpth;
    }
};

/**
    Gets or sets the text string of the text.
    @param {String} [NewText] The new text string to set
    @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.text("NewText"); // Set its text to "NewText"
var ctxt = txt.text(); // Get current text, should return "NewText"
    @return {String} The current text string, if no argument was passed.
*/
module.exports.Text.prototype.text = function()
{
    if(arguments.length >= 1)
    {
        var txt = arguments[0].toString();
        __sdl__Text_text(this.ID, txt);
    }
    else
    {
        var txt = __sdl__Text_text(this.ID);
        return txt;
    }
};

/**
    Gets or sets the RGBA color of the text.
    @param {Object} [NewColor] The new RGBA color to set.
    @param {Number} [NewColor.R] The new RGBA color's red value.
    @param {Number} [NewColor.G] The new RGBA color's green value.
    @param {Number} [NewColor.B] The new RGBA color's blue value.
    @param {Number} [NewColor.A] The new RGBA color's alpha value.
        @example
    var sdl = require("sdl");
var txt = new sdl.Text("Text", 30);
txt.color({ R: 0, G: 255, B: 0, A: 255 }); // Set X to a green color
var clr = txt.color(); // Get current X, should return the coloe set above
    @return {Object} The current RGBA color, if no argument was passed.
*/
module.exports.Text.prototype.color = function()
{
    if(arguments.length >= 1)
    {
        var red = arguments[0].R;
        var green = arguments[0].G;
        var blue = arguments[0].B;
        var alpha = arguments[0].A;
        __sdl__Text_color(this.ID, red, green, blue, alpha);
    }
    else
    {
        var red = __sdl__Text_color_r(this.ID);
        var green = __sdl__Text_color_g(this.ID);
        var blue = __sdl__Text_color_b(this.ID);
        var alpha = __sdl__Text_color_a(this.ID);
        var clr = {R: red, G: green, B: blue, A: alpha};
        return clr;
    }
};

/**
    Creates a graphic object as an image.
    @class
    @classdesc Class representing a graphic object.
    @param {String} Path The path to the image (PNG or JPEG) to set as the object.
    @param {Number} [X] The X coordinate of the object.
    @param {Number} [Y] The Y coordinate of the object.
    @property {Number} ID The internal ID of the object, a random number.
*/
module.exports.Object = function()
{
    if(arguments.length == 1)
    {
        var path = arguments[0];
        this.ID = __sdl__Object_create(path, 0, 0, 0);
    }
    else if(arguments.length == 3)
    {
        var path = arguments[0];
        var x = arguments[1];
        var y = arguments[2];
        this.ID = __sdl__Object_create(path, x, y, 0);
    }
};

/**
    Shows the object, if it's hidden.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.show();
*/
module.exports.Object.prototype.show = function()
{
    __sdl__Object_show(this.ID);
};

/**
    Hides the object, if it's shown.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.hide();
*/
module.exports.Object.prototype.hide = function()
{
    __sdl__Object_hide(this.ID);
};

/**
    Returns whether the object is shown or not.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg", 20, 20);
var sh = obj.isShown();
    @return {Boolean} Whether it's shown or not.
*/
module.exports.Object.prototype.isShown = function()
{
    var sh = __sdl__Object_isShown(this.ID);
    return sh;
};

/**
    Gets or sets the X coordinate of the object.
    @param {Number} [NewX] The new X coordinate to set
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.x(50); // Set X to 50
var cx = obj.x(); // Get current X, should return 50
    @return {Number} The current X coordinate, if no argument was passed.
*/
module.exports.Object.prototype.x = function()
{
    if(arguments.length >= 1)
    {
        var x = arguments[0];
        __sdl__Object_x(this.ID, x);
    }
    else
    {
        var x = __sdl__Object_x(this.ID);
        return x;
    }
};

/**
    Gets or sets the Y coordinate of the object.
    @param {Number} [NewY] The new Y coordinate to set
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.y(50); // Set Y to 20
var cy = obj.y(); // Get current Y, should return 20
    @return {Number} The current Y coordinate, if no argument was passed.
*/
module.exports.Object.prototype.y = function()
{
    if(arguments.length >= 1)
    {
        var y = arguments[0];
        __sdl__Object_y(this.ID, y);
    }
    else
    {
        var y = __sdl__Object_y(this.ID);
        return y;
    }
};


/**
    Gets or sets the depth of the object.
    @param {Number} [NewDepth] The new depth to set
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.depth(2); // Set depth layer to 2
var dp = obj.depth(); // Get current depth layer, should return 2
    @return {Number} The current depth, if no argument was passed.
*/
module.exports.Object.prototype.depth = function()
{
    if(arguments.length >= 1)
    {
        var dpth = arguments[0];
        __sdl__Object_depth(this.ID, dpth);
    }
    else
    {
        var dpth = __sdl__Object_depth(this.ID);
        return dpth;
    }
};

/**
    Gets or sets the horizontal speed of the object.
    @param {Number} [NewHspeed] The new horizontal speed to set
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.hspeed(7); // Set horizontal speed to 7
var hs = obj.hspeed(); // Get current horizontal speed, should return 7
    @return {Number} The current horizontal speed, if no argument was passed.
*/
module.exports.Object.prototype.hspeed = function()
{
    if(arguments.length >= 1)
    {
        var hs = arguments[0];
        __sdl__Object_hspeed(this.ID, hs);
    }
    else
    {
        var hs = __sdl__Object_hspeed(this.ID);
        return hs;
    }
};

/**
    Gets or sets the vertical speed of the object.
    @param {Number} [NewVspeed] The new vertical speed to set
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.vspeed(12); // Set vertical speed to 12
var vs = obj.vspeed(); // Get current vertical speed, should return 12
    @return {Number} The current vertical speed, if no argument was passed.
*/
module.exports.Object.prototype.vspeed = function()
{
    if(arguments.length >= 1)
    {
        var vs = arguments[0];
        __sdl__Object_vspeed(this.ID, vs);
    }
    else
    {
        var vs = __sdl__Object_vspeed(this.ID);
        return vs;
    }
};

/**
    Gets or sets whether the object uses physical behaviour. (it's affected by gravity, hspeed, vspeed)
    @param {Boolean} [Physics] Whether it will use physical behaviour.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
obj.usesPhysics(true); // Now this object uses physics
var phys = obj.usesPhysics(); // Get whether it uses physics, should return true
    @return {Boolean} Whether it uses physical behaviour, if no argument was passed.
*/
module.exports.Object.prototype.usesPhysics = function()
{
    if(arguments.length >= 1)
    {
        var ph = arguments[0];
        __sdl__Object_usesPhysics(this.ID, ph);
    }
    else
    {
        var ph = __sdl__Object_usesPhysics(this.ID);
        return ph;
    }
};

/**
    Checks whether this object is colliding with another object.
    @param {sdl.Object} [Other] The other object to check collision.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
var obj2 = new sdl.Object("sdmc:/Image2.png");
var ocoll = obj.checkCollide(obj2);
    @return {Boolean} Whether both objects are colliding or not.
*/
module.exports.Object.prototype.checkCollide = function(Other)
{
    if(this.ID == Other.ID) return false;
    var coll = __sdl__Object_checkCollide(this.ID, Other.ID);
    return coll;
};

/**
    Checks whether this object is colliding with the left side of the screen.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
var lcoll = obj.leftCollide(obj);
    @return {Boolean} Whether it's colliding with the left side of the screen.
*/
module.exports.Object.prototype.leftCollide = function()
{
    var coll = __sdl__Object_leftCollide(this.ID);
    return coll;
};

/**
    Checks whether this object is colliding with the right side of the screen.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
var rcoll = obj.rightCollide(obj);
    @return {Boolean} Whether it's colliding with the right side of the screen.
*/
module.exports.Object.prototype.rightCollide = function()
{
    var coll = __sdl__Object_rightCollide(this.ID);
    return coll;
};

/**
    Checks whether this object is colliding with the top side of the screen.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
var tcoll = obj.topCollide(obj);
    @return {Boolean} Whether it's colliding with the top side of the screen.
*/
module.exports.Object.prototype.topCollide = function()
{
    var coll = __sdl__Object_topCollide(this.ID);
    return coll;
};

/**
    Checks whether this object is colliding with the bottom side of the screen.
    @example
    var sdl = require("sdl");
var obj = new sdl.Object("sdmc:/Image.jpg");
var bcoll = obj.bottomCollide(obj);
    @return {Boolean} Whether it's colliding with the bottom side of the screen.
*/
module.exports.Object.prototype.bottomCollide = function()
{
    var coll = __sdl__Object_bottomCollide(this.ID);
    return coll;
};

/**
    Opens an audio file to be used as audio.
    @function openAudio
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.mp3");
    @param {String} Path The path to the audio file. (MP3, WAV, OGG or FLAC formats work)
*/
module.exports.openAudio = function(Path)
{
    __sdl__openAudio(Path);
};

/**
    Plays previously opened audio.
    @function playAudio
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.mp3");
sdl.playAudio(true); // Play looping
    @param {Boolean} Loop Whether it'll play on loop, otherwise it'll be played once.
*/
module.exports.playAudio = function(Loop)
{
    __sdl__playAudio(Loop);
}

/**
    Pauses the audio output if it's being played.
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.mp3");
sdl.pauseAudio();
    @function pauseAudio
*/
module.exports.pauseAudio = function()
{
    __sdl__pauseAudio();
}

/**
    Resumes tha audio if it's been paused.
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.wav");
sdl.resumeAudio();
    @function resumeAudio
*/
module.exports.resumeAudio = function()
{
    __sdl__resumeAudio();
};

/**
    Gets whether the audio output is paused or not.
    @function pausedAudio
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.flac");
var pause = sdl.pausedAudio();
    @return {Boolean} Whether the audio output is paused or not.
*/
module.exports.pausedAudio = function()
{
    var paused = __sdl__pausedAudio();
    return paused;
}

/**
    Gets whether the audio output is being played or not.
    @function playingAudio
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.wav");
var play = sdl.playingAudio();
    @return {Boolean} Whether the audio output is being played or not.
*/
module.exports.playingAudio = function()
{
    var playing = __sdl__playingAudio();
    return playing;
}

/**
    Stops audio output if it's being played.
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.mp3");
sdl.stopAudio();
    @function stopAudio
*/
module.exports.stopAudio = function()
{
    __sdl__stopAudio();
}

/**
    Rewinds the audio output, so it starts playing from the start again.
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.ogg");
sdl.restartAudio();
    @function restartAudio
*/
module.exports.restartAudio = function()
{
    __sdl__restartAudio();
}

/**
    Gets the audio format of the opened audio file.
    @function typeAudio
    @example
    var sdl = require("sdl");
sdl.openAudio("sdmc:/Song.wav");
var type = sdl.typeAudio(); // Should return "WAV"
    @return {String} The audio format of the file.
*/
module.exports.typeAudio = function()
{
    var type = __sdl__typeAudio();
    return type;
}