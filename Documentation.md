# Brew.js API for Nintendo Switch

  > Part of Brew.js project  
  > Copyright © 2018, by XorTroll

## [Module] `game`

- [Function] `game.exit()`
  > Ends the game's execution.

- [Function] `game.flush()`
  > Flushes the graphic data of current game's execution. Must be called from main loop if used.

## [Module] `device`

- [Variable] `device.Platform` : string
  > The code name of the platform. (`"NX"` in this case)

- [Variable] `device.PlatformName` : string
  > The official name of the platform. (`"Nintendo Switch"` in this case)

- [Function] `device.language()` : string
  > The language of the Switch.

## [Module] `device.screen`

- [Variable] `device.screen.Width` : number
  > The width of the Switch's screen. (`1280` in this case)

- [Variable] `device.screen.Height` : number
  > The width of the Switch's screen. (`720` in this case)

## [Module] `input`

- [Variable] Switch inputs : number

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

# [Module] `console`

- [Function] `console.init()`
  > Initializes the console.

- [Function] `console.log(...)`
  > Prints all the given arguments to the console.

- [Function] `console.logLine(...)`
  > Like `console.log()` but it adds a final line.

- [Function] `console.clear()`
  > Clears / resets the console.

## [Module] `gfx`

- [Function] `gfx.loadBitmap(Path)` : number (bitmap ID)
  > Loads image data from a BMP file and stores it. The ID is returned.

- [Function] `gfx.showBitmap(Bitmap)`
  > Shows the given bitmap ID into the screen.

- [Function] `gfx.setBitmapX(Bitmap, X)`
  > Sets X coordinate onto given bitmap ID.

- [Function] `gfx.setBitmapY(Bitmap, Y)`
  > Sets Y coordinate onto given bitmap ID.

- [Function] `gfx.setBitmapXY(Bitmap, X, Y)`
  > Sets X and Y coordinates onto given bitmap ID.