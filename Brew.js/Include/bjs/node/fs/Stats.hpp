
#pragma once
#include <bjs/js.hpp>

namespace bjs::node::fs
{
    namespace Stats
    {
        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isDirectory() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isDirectory(js::NativeContext Context);
        
        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isFile() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isFile(js::NativeContext Context);

        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isFIFO() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isFIFO(js::NativeContext Context);

        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isBlockDevice() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isBlockDevice(js::NativeContext Context);
        
        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isCharacterDevice() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isCharacterDevice(js::NativeContext Context);

        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isSocket() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isSocket(js::NativeContext Context);

        /**
            @brief API JS Class method (fs.Stats): "fs.Stats.isSymbolicLink() → Boolean"
            @note This method is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Function isSymbolicLink(js::NativeContext Context);

        /**
            @brief API JS Class - constructor: "new fs.Stats(Path)"
            @note This class is similar to Node.js's one. This description is for using the class with JavaScript.
        */
        js::Class CreateClass();
    }
}