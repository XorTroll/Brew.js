
#pragma once
#include <bjs/js.hpp>

namespace bjs::node::fs
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
}