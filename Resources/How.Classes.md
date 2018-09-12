# Creating classes

Brew.js API provides the option to create native classes. For instance, the class `fs.Stats` from Node.js is also present in Brew.js, and it was created this way.

## Procedure

- The class object

  Brew.js has a Class class: `Brew::API::Class`, which is the base of making classes via the API. First of all, we need to create the class. We need a name and a constructor API JS function to create it. Here's an example, using and without using lambda functions:

  ``` cpp
  Brew::API::Function MyClass_Ctor (Brew::API::NativeContext Context)
  {
      // For class-related API JS functions we need to use ClassHandler!
      Brew::API::ClassHandler handler(Context);

      // ClassHandler inherits FunctionHandler
      string arg1 = handler.getString(0);

      // Set the property inside the class
      handler.setPropertyString("sample", arg1);

      // Always return "Void" in constructors!
      return Brew::API::Return::Void;
  }

  Brew::API::Class MyClass()
  {
      Brew::API::Class classobj("MyClass", MyClass_Ctor);

      // Add other stuff to the class object...

      return classobj;
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
  Brew::API::Module mymod("myModule");

  // Add the class from the function above
  mymod.pushClass(MyClass());

  // Add the module to the modules object
  Brew::API::addModule(mymod);
  ```

  This way, after adding the module to Brew.js's API's modules object, it can be called from JavaScript when source code is evaluated from a `Environment`:

  ``` js
  const myMod = require("myModule");
  var instance = new myMod.MyClass("arg1string");
  ```

## Important notes

- Always use `Brew::API::ClassHandler` with any API JS functions related to classes, like class members or constructors. This is important because they won't work properly using `Brew::API::FunctionHandler` or even without using neither.