# Brew.js API for Nintendo Switch

  > This API and interpreter are part of Brew.js project.  
  Copyright © 2018, by XorTroll.

## [Module] `game`

- [Variable] `game.ProjectDir` : string
  > The directory of the current project, where the JS sources are.

- [Function] `game.mainLoop(Function)`
  > Enters the game's main loop, executing given function each frame.

- [Function] `game.exitLoop()`
  > Ends the game's main loop, only works if main loop is enabled.

- [Function] `game.exit()`
  > Ends the game's execution.

## [Module] `device`

- [Variable] `device.Platform` : string
  > The code name of the platform. (`"NX"` in this case)

- [Variable] `device.PlatformName` : string
  > The official name of the platform. (`"Nintendo Switch"` in this case)

- [Function] `device.language()` : string
  > The language of the Switch.

- [Function] `device.randomNumber(Max)` | `device.randomNumber(Min, Max)` : string
  > Gets a random integer (not decimal). Maximum and minimum numbers or just a maximum number can be given.

## [Module] `device.screen`

- [Variable] `device.screen.Width` : number
  > The width of the Switch's screen. (`1280` in this case)

- [Variable] `device.screen.Height` : number
  > The width of the Switch's screen. (`720` in this case)

## [Module] `input`

- [Variable list] Switch inputs : number (input ID)

  `input.None` , `input.A` , `input.B` , `input.X` , `input.Y` , `input.LStick` , `input.RStick` , `input.L` , `input.R` , `input.ZL` , `input.ZR` , `input.Plus` , `input.Minus` , `input.Up` , `input.Down` , `input.Left` , `input.Right` , `input.DPadLeft` , `input.DPadUp` , `input.DPadRight` , `input.DPadDown` , `input.LStickLeft` , `input.LStickUp` , `input.LStickRight` , `input.LStickDown` , `input.RStickLeft` , `input.RStickUp` , `input.RStickRight` , `input.RStickDown` , `input.SL` , `input.SR` , `input.Touch` , `input.JoyConRight` , `input.JoyConDown` , `input.JoyConUp` , `input.JoyConLeft`

  > All the input variables for the Switch.

- [Function] `input.checkPressed()` : number (input)
  > Returns the key being pressed.

- [Function] `input.checkHeld()` : number (input)
  > Returns the key being constanty pressed.

- [Function] `input.checkReleased()` : number (input)
  > Returns the key being released.

- [Function] `input.touchData()` : object (`input.touchData().X` , `input.touchData().Y`)
  > Returns the X and Y coordinates if the screen was touched.

## [Module] `fs`

- [Function] `fs.exists(Path)` : boolean
  > Checks if the given path exists or not.

- [Function] `fs.isFile(Path)` : boolean
  > Checks if the given path is a file or not.

- [Function] `fs.isDirectory(Path)` : boolean
  > Checks if the given path is a directory or not.

- [Function] `fs.readText(Path)` : string
  > Returns the given file's content as a string.

- [Function] `fs.writeText(Path)`
  > Writes given text to a file, and it is created if it doesn't exist.

- [Function] `fs.pathSDMC(Path)` : string
  > Formats the given path as if it was in the SD. (like adding `"sdmc:/"`)

- [Function] `fs.pathRomFS(Path)` : string
  > Formats the given path as if it was in the RomFS. (like adding `"romfs:/"`)

- [Function] `fs.readConfigFile(Path, Section, Name)` : string
  > Reads the value of a section of a config file (like INI), `undefined` is returned if any errors happen.

## [Module] `gfx`

- [Function] `gfx.setBackground(Path)` | `gfx.setBackground(Color)`
  > Sets the background of the screen. Possible arguments: path to an image, a color object. Color is always asked or given as an object like this: `{R: 0, G: 0, B: 0}`.

## [Module] `gfx.text`

- [Variable list] Text fonts : number (font ID)

  `gfx.text.InterUIMedium20` , `gfx.text.InterUIMedium28` , `gfx.text.InterUIMedium30` , `gfx.text.InterUIMedium42` , `gfx.text.InterUIRegular14` , `gfx.text.InterUIRegular18` , `gfx.text.InterUIRegular20` , `gfx.text.InterUIRegular24` , `gfx.text.Tahoma12` , `gfx.text.Tahoma24`
  > Different fonts, to be used with `Text` objects.

- [Function] `gfx.text.create(Text, X, Y, Depth, Color, Font)` : number (text ID)
  > This function must be used to create any `gfx.text.Text` object. Color is always asked or given as an object like this: `{R: 0, G: 0, B: 0}`.

### [Class] `gfx.text.Text`

- [Variable] `(gfx.text.Text).ID` : number
  > Its identifier, randomly generated with creating object.

- [Function] `(gfx.text.Text).show()`
  > Shows the text, if it isn't shown.

- [Function] `(gfx.text.Text).hide()`
  > Hides the text, if it isn't hidden.

- [Function] `(gfx.text.Text).x()` | `(gfx.text.Text).x(X)` : number
  > If no argument is passed it returns the text's X coordinate, otherwise it sets the X coordinate with the given value.

- [Function] `(gfx.text.Text).y()` | `(gfx.text.Text).y(Y)` : number
  > If no argument is passed it returns the text's Y coordinate, otherwise it sets the Y coordinate with the given value.

- [Function] `(gfx.text.Text).content()` | `(gfx.text.Text).content(Text)` : string
  > If no argument is passed it returns the text's text content, otherwise it sets the content with the given string.

- [Function] `(gfx.text.Text).font()` | `(gfx.text.Text).font(Font)` : number
  > If no argument is passed it returns the text's font, otherwise it sets the font with the given font (from `gfx.text` fonts).

- [Function] `(gfx.text.Text).color()` | `(gfx.text.Text).color(Color)` : number
  > If no argument is passed it returns the text's color, otherwise it sets the font with the given color. Color is always asked or given as an object like this: `{R: 0, G: 0, B: 0}`.

## [Module] `gfx.object`

- [Function] `gfx.object.create(Path)` | `gfx.object.create(Color, Width, Height)` : number (object ID)
  > This function must be used to create any `gfx.text.Text` object. It can be created by a path to an image (BMP, PNG or JPG) or a rectangle with color, width and height. Color is always asked or given as an object like this: `{R: 0, G: 0, B: 0}`.

### [Class] `gfx.object.Object`

- [Variable] `(gfx.object.Object).ID` : number
  > Its identifier, randomly generated with creating object.

- [Function] `(gfx.object.Object).show()`
  > Shows the object, if it isn't shown.

- [Function] `(gfx.object.Object).hide()`
  > Hides the object, if it isn't hidden.

- [Function] `(gfx.object.Object).isShown()` : boolean
  > Returns true if the object is shown, otherwise false.

- [Function] `(gfx.object.Object).x()` | `(gfx.object.Object).x(X)` : number
  > If no argument is passed it returns the text's X coordinate, otherwise it sets the X coordinate with the given value.

- [Function] `(gfx.object.Object).y()` | `(gfx.object.Object).y(Y)` : number
  > If no argument is passed it returns the text's Y coordinate, otherwise it sets the Y coordinate with the given value.

- [Function] `(gfx.object.Object).depth()` | `(gfx.object.Object).depth(Depth)` : string
  > If no argument is passed it returns the object's depth value, otherwise it sets it with the given number.

- [Function] `(gfx.object.Object).horizontalSpeed()` | `(gfx.object.Object).horizontalSpeed(Speed)` : number
  > If no argument is passed it returns the object's horizontal speed, otherwise it sets it with the given number. Speeds only work if the object is shown.

- [Function] `(gfx.object.Object).checkTouch()` : boolean
  > Returns true if the object is touched, otherwise false.

- [Function] `(gfx.object.Object).checkCollision(Object)` : boolean
  > Returns true if the object is colliding with the other object given, otherwise false.

- [Function] `(gfx.object.Object).checkLeftCollision()` : boolean
  > Returns true if the object is colliding with the left side of the screen, otherwise false.

- [Function] `(gfx.object.Object).checkRightCollision()` : boolean
  > Returns true if the object is colliding with the right side of the screen, otherwise false.

- [Function] `(gfx.object.Object).checkTopCollision()` : boolean
  > Returns true if the object is colliding with the top side of the screen, otherwise false.

- [Function] `(gfx.object.Object).checkBottomCollision()` : boolean
  > Returns true if the object is colliding with the bottom side of the screen, otherwise false.

## [Module] `gfx.dialog`

- [Function] `gfx.dialog.keyboardInput()` : string
  > Loads a custom-made keyboard asking for input (I know, it's a bit poor) and waits until `OK` option is selected, and returns the typed text.
