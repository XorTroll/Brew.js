# Welcome to Brew.js! - Nintendo 3DS version

**Welcome to the documentation of Brew.js for Nintendo 3DS!**

## The engine: duktape

   - Duktape is an embeddable Javascript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).
   - It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.
   - You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could crash the execution, so be sure to know what you're doing. It's not recommended to do mix duktape functions with Brew.js API.

## Nintendo 3DS (`CTR`) specifications

 - Nintendo 3DS build of Brew.js libs has some specific modules:
   - `ctr` - Basic functions and constants for the Nintendo 3DS system
   - `sf2d` - Wrapper for sf2dlib, using GPU rendering via citro3d and citro2d (required both libs)
   - Platform-specific `console` and `input` modules
   - Platform-specific details in `os`, `path` and `process` Node.js modules.

## Simple JavaScript execution

 - If you just want to evaluate JavaScript without messing up with the C++ API, that's easy to do:

   - First of all, include Brew.js's main header to use it:

   ``` cpp
   #include <Brew.js/Brew.hpp>
   ```

   - Then create a environment and the native context to use with the environment:

   ``` cpp
   Brew::API::NativeContext context = Brew::API::createContext();
   Brew::Environment myenv(context);
   ```

   - There are 3 possibilities of evaluating code:
     - Evaluating directly source code
     - Evaluating source code from a file
     - Evaluating a Node-like (package.json) project

     All of them return a execution result, which has information about the result of the execution (result as string, original result type, success / failure...)

   ``` cpp
   Brew::ExecutionResult res1 = myenv.evaluateSourceCode("var num = 9;");
   Brew::executionResult res2 = myenv.evaluateFile("sdmc:/test.js");
   vector<Brew::Projects> projects = myenv.getProjectList("sdmc:/Projects");
   Brew::ExecutionResult res3 = myenv.evaluateProject(projects[0]);
   ```

   - For more information, browse the documentation.