
/**

    @file FS.hpp
    @brief Brew.js API - Node.js modules - FS
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <bjs/JavaScript.hpp>

namespace bjs::modules::FS
{
    /**
        @brief API JS Function: "fs.readFile(File) → String"
        @note Gets the contents of a file as a string. This description is for using the function with JavaScript.
    */
    js::Function readFile(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.writeFile(File, Text)"
        @note Writes given text to a file. This description is for using the function with JavaScript.
    */
    js::Function writeFile(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.exists(File) → Boolean"
        @note Returns whether given file exists or not. This description is for using the function with JavaScript.
    */
    js::Function exists(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.mkdir(Dir)"
        @note Creates a directory. This description is for using the function with JavaScript.
    */
    js::Function mkdir(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.unlink(File)"
        @note Deletes a file. This description is for using the function with JavaScript.
    */
    js::Function unlink(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.rmdir(Dir)"
        @note Deletes a directory. This description is for using the function with JavaScript.
    */
    js::Function rmdir(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.readdir(Dir) → Array [String]"
        @note Gets all files and subdirectories inside a directory. This description is for using the function with JavaScript.
    */
    js::Function readdir(js::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.rename(File)"
        @note Renames a file. This description is for using the function with JavaScript.
    */
    js::Function rename(js::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isDirectory() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isDirectory(js::NativeContext Context);
    
    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isFile() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isFile(js::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isFIFO() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isFIFO(js::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isBlockDevice() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isBlockDevice(js::NativeContext Context);
    
    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isCharacterDevice() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isCharacterDevice(js::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isSocket() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isSocket(js::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isSymbolicLink() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Function Stats_isSymbolicLink(js::NativeContext Context);

    /**
        @brief API JS Class - constructor: "new fs.Stats(Path)"
        @note This class is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    js::Class Stats();

    /**
        @brief Gets the Brew.js module of fs.
        @return The module.
    */
    js::Module Initialize();
}