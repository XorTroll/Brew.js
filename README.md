# Brew.js API for Nintendo Switch

  > This API and interpreter are part of Brew.js project.  
  Copyright © 2018, by XorTroll.

## [Module] `game`

- [Variable] `game.ProjectDir` : string
  > The directory of the current project, where the JS sources are.

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

- [Function] `fs.readConfigFile(Path, Section, Name)` : string
  > Reads the value of a section of a config file (like INI), `undefined` is returned if any errors happen.

## [Module] `console`

  > For the moment (and probably forever) console module has been disabled due to problems with graphics and printing.

## [Module] `gfx`

- [Function] `gfx.spriteFromBMP(Path)` : number (sprite ID)
  > Creates a sprite from a bitmap file.

- [Function] `gfx.spriteFromJPEG(Path)` : number (sprite ID)
  > Creates a sprite from a jpeg/jpg file.

- [Function] `gfx.spriteFromPNG(Path)` : number (sprite ID)
  > Creates a sprite from a png file.

- [Function] `gfx.showSprite(Sprite)`
  > Shows the given sprite into the screen.

- [Function] `gfx.setSpriteX(Sprite, X)`
  > Sets X coordinate onto given sprite.

- [Function] `gfx.setSpriteY(Sprite, Y)`
  > Sets Y coordinate onto given sprite.

- [Function] `gfx.setSpriteXY(Sprite, X, Y)`
  > Sets X and Y coordinates onto given sprite.

- [Function] `gfx.createText(Text, Font, Size, X, Y, R, G, B)` : number (text ID)
  > Creates a text, with a font (recommended to use `gfx.font` module fonts), specific font size, specified X and Y coordinates and the color made using R, G, and B values.

- [Function] `gfx.showText(Text)` : number (text ID)
  > Shows the given text into the screen.

- [Function] `gfx.setTextX(Text, X)`
  > Sets X coordinate onto given text.

- [Function] `gfx.setTextY(Text, Y)`
  > Sets Y coordinate onto given text.

- [Function] `gfx.setTextXY(Text, X, Y)`
  > Sets X and Y coordinates onto given text.

- [Function] `gfx.setTextColor(Text, R, G, B)`
  > Sets R, G and B values (color values) onto given text.

- [Function] `gfx.setTextFont(Text, Font, Size)`
  > Sets the font (recommended to use `gfx.font` module fonts) and its size onto given text.

- [Function] `gfx.setTextContent(Text, Content)`
  > Sets the content (the text) onto given text.

## [Module] `gfx.font`

- [Variable] `gfx.font.InterUIRegular` : number (font ID)
  > The InterUI font (to be used with texts), regular style.

- [Variable] `gfx.font.InterUIMedium` : number (font ID)
  > The InterUI font (to be used with texts), medium style.

- [Variable] `gfx.font.Tahoma` : number (font ID)
  > The Tahoma font (to be used with texts), normal style.