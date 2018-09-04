# Brew.js - C++ userland JavaScript API for Nintendo homebrew, based on Node.js

## The engine: duktape

   - Duktape is an embeddable Javascript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).
   - It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.
   - You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could fuck up the execution, so be sure to know what you're doing.

## Basic examples of API usage

 - If you just want to evaluate quickly some sources, it's easy to do:

    ``` cpp
    #include <Brew.js/Brew.hpp>
    ```

   - First of all, include the main header to use Brew.js

   ``` cpp
   Brew::API::NativeContext context = Brew::API::createContext();
   Brew::Environment myenv(context);
   ```

   - Then, we create a environment, and context to use with our environment.

   ``` cpp
   Brew::ExecutionResult res1 = myenv.evaluateSourceCode("var x = 9;");
   Brew::executionResult res2 = myenv.evaluateFile("sdmc:/test.js");
   vector<Brew::Projects> projects = myenv.getProjectList("sdmc:/Projects");
   Brew::ExecutionResult res3 = myenv.evaluateProject(projects[0]);
   ```

   - There are 3 possibilities of evaluating code:
     - Evaluating directly source code
     - Evaluating code from a file
     - Evaluating a Brew.js/Node.js project

     All of them return a execution result, which has information such as if there was any error or the result's type.

- In case you want to extend the API:

    ``` cpp
    #include <Brew.js/Brew.hpp>
    ```

   - First of all, include the main header to use Brew.js

   ``` cpp
   Brew::API::Function myFunction(Brew::API::NativeContext Context)
   {
       Brew::API::FunctionHandler handler(Context);
       if(handler.checkArgc(2))
       {
           string one = handler.getString(0);
           string two = handler.getString(0);
           string result = one + two;
           handler.pushString(result);
       }
       else handler.throwError(Brew::Error::ReferenceError, "This needs two strings!");
       return Brew::API::Return::Variable;
   }
   ```

   - To understand this API JS function better, here is the same function in JavaScript:

   ``` js
   function myFunction()
   {
       // Note that FunctionHandler::checkArgc() means "higher or equal"
       // Use FunctionHandler::checkArgcEqual() for only equal.
       if(arguments.length >= 2)
       {
           var one = arguments[0];
           var two = arguments[1];
           var result = one + two;
           return result;
       }
       else throw new ReferenceError("This needs two strings!");
   }
   ```

   - For more specific information, check the documentation of the different C++ elements.

   - **IMPORTANT**: Don't add anything to the global object after having created a environment. Modules can be added as they are processed on runtime, but the global object is created and initialized when creating a environment.

   - Then, after making our API JS function, we should add it to the JS context. How do we do that? There are several possibilities:

   ``` cpp
   // - Option 1: Create a module and add it to 'require' module list
   // First we need to create a module and add the function
   Brew::API::Module myModule("mymodule");
   myModule.pushFunction("myFunction", myFunction);
   Brew::API::addModule(myModule);
   // Now we could do: require('mymodule').myFunction('one', 'two')

   // - Option 2: Create a module and add it as a global object
   // We start the same way as in option 1
   Brew::API::Module myModule("mymodule");
   myModule.pushFunction("myFunction", myFunction);
   // But we add the module to the global object
   Brew::API::Global.pushModule(myModule);
   // Now we could do: mymodule.myFunction('one', 'two')

   // - Option 3: Add the function directly to the global object
   Brew::API::Global.pushFunction("myFunction", myFunction);
   ```

   - Note that it's preferred to add it via a module, as it's still unsafe and usually fails to add objects directly to the global object.

   - You can also add different elements to the global object or to modules:

   ``` cpp
   Brew::API::Module example("example");
   // require('example').Name -> XorTroll
   example.pushString("Name", "XorTroll");
   // require('example').IsBoy -> true
   example.pushBoolean("IsBoy", true);
   // require('example').Year -> 2018
   example.pushUInt("Year", 2018);
   // require('example').Angle -> 60.058
   example.pushDouble("Angle", 60.058);
   Brew::API::addModule(example);
   ```

   - For more information, browse the documentation below.

## Used libraries

   - duktape | C JavaScript engine | by svaarala
   - bitmap_image | BMP decoding | by ArashPartow
   - jsoncpp | JSON parsing for C++ | by nlohmann
   - nanojpeg | JPEG decoding | by Martin J. Fiedler
   - upng | PNG decoding | by elanthis
   - sf2dlib | 3DS GPU rendering via citro3d | by xerpi

### Go to [my web page](https://xortroll.github.io/brew.js) to see all the documentation!