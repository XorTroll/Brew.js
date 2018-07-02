# Brew.js for Nintendo Switch

## What is __Brew.js__?
- Brew.js is a JavaScript API for Nintendo systems, Nintendo Switch in this case, plus an interpreter to execute sources written in JS. Currently only Switch is supported, but a 3DS version well be released soon.

## What do I need to create Brew.js JavaScript scripts?
- The interpreter (NRO ROM, loaded from Homebrew) on the [releases page](https://github.com/BrewJS/Switch/releases/latest)
- Documentation at [Brew.js's website](https://brewjs.github.io)
- An IDE or text editor to write code (I suggest using [Visual Studio Code](https://code.visualstudio.com/download) or [Notepad++](https://notepad-plus-plus.org/download))
- Now you can create your projects. Save all the scripts and "package.json" meta file in a folder, and put it on "sdmc:/switch/Brew.js/<project>" to run it. Enjoy scripting!

## How can I embed Brew.js interpreter on my C++ project?
Just download "BrewEmbed.zip", which is a sample of Brew.js embedding, and use it as the base of your project.
