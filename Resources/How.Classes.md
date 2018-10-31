# Brew.js - Creating classes

Brew.js API provides the option to create native classes. For instance, the class `fs.Stats` from Node.js is also present in Brew.js, and it was created this way.

## Procedure

- The class object

  Brew.js has a JavaScript class interpretation via `bjs::js::Class` object, which is the base of making classes via the API. First of all, we need to create the class. We need a name and a constructor API JS function to create it. Here's an example of a constructor:

  ``` cpp
  bjs::js::Function MyClass_Ctor(bjs::js::NativeContext Context)
  {
      // For class-related API JS functions we need to use ClassHandler!
      bjs::js::ClassHandler handler(Context);

      // ClassHandler inherits from FunctionHandler
      string arg1 = handler.GetString(0);

      // Set the property inside the class
      handler.SetPropertyString("sample", arg1);

      // Always return "Void" in constructors!
      return bjs::js::Return::Void;
  }
  ```

  Here you have the same code/function in plain JavaScript:

  ``` js
  var MyClass = function()
  {
      var arg1 = arguments[0];
      this.sample = arg1;
  }
  ```

  As you can see, making a class via the API is a bit longer than making it on plain JS. Then, to add it inside a module, it's as simple as this:

  ``` cpp
  // Create the module object
  bjs::js::Module mymod("myModule");

  // Create the class
  bjs::js::Class cMyClass("MyClass", MyClass_Ctor);

  // Add the class from the function above
  mymod.PushClass(cMyClass);

  // Add the module to the modules object
  bjs::modules::Add(mymod);
  ```

  This way, after adding the module to Brew.js's API's modules object, it can be called from JavaScript when the source code is evaluated using a `bjs::Environment`:

  ``` js
  const myMod = require("myModule");
  var instance = new myMod.MyClass("arg1string");
  ```

## Important notes

- Always use `bjs::js::ClassHandler` with any API JS functions related to classes, like class members or constructors. This is important because they won't work properly using `bjs::js::FunctionHandler` or even without using neither.