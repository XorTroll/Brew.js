
/**

    @file FS.hpp
    @brief Brew.js API - Node.js modules - FS
    @author XorTroll
    @copyright Brew.js project

*/

#pragma once
#include <Brew.js/API.hpp>

namespace Brew::BuiltIn::FS
{
    /**
        @brief API JS Function: "fs.readFile(File) → String"
        @note Gets the contents of a file as a string. This description is for using the function with JavaScript.
    */
    API::Function readFile(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.writeFile(File, Text)"
        @note Writes given text to a file. This description is for using the function with JavaScript.
    */
    API::Function writeFile(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.exists(File) → Boolean"
        @note Returns whether given file exists or not. This description is for using the function with JavaScript.
    */
    API::Function exists(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.mkdir(Dir)"
        @note Creates a directory. This description is for using the function with JavaScript.
    */
    API::Function mkdir(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.unlink(File)"
        @note Deletes a file. This description is for using the function with JavaScript.
    */
    API::Function unlink(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.rmdir(Dir)"
        @note Deletes a directory. This description is for using the function with JavaScript.
    */
    API::Function rmdir(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.readdir(Dir) → Array [String]"
        @note Gets all files and subdirectories inside a directory. This description is for using the function with JavaScript.
    */
    API::Function readdir(API::NativeContext Context);
    
    /**
        @brief API JS Function: "fs.rename(File)"
        @note Renames a file. This description is for using the function with JavaScript.
    */
    API::Function rename(API::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isDirectory() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isDirectory(API::NativeContext Context);
    
    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isFile() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isFile(API::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isFIFO() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isFIFO(API::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isBlockDevice() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isBlockDevice(API::NativeContext Context);
    
    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isCharacterDevice() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isCharacterDevice(API::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isSocket() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isSocket(API::NativeContext Context);

    /**
        @brief API JS Class method (fs.Stats): "fs.Stats.isSymbolicLink() → Boolean"
        @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Function Stats_isSymbolicLink(API::NativeContext Context);

    /**
        @brief API JS Class - constructor: "new fs.Stats(Path)"
        @note This class is similar to Node.js's one. This description is for using the class with JavaScript.
    */
    API::Class Stats();

    /**
        @brief Gets the Brew.js module of fs.
        @return The module.
    */
    API::Module Initialize();
}