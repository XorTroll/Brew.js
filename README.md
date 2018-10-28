# Brew.js - C++ high-level JavaScript API for Nintendo homebrew

- Current dev version: **2.0 (early commits, no released)**

- Latest released version: [1.0](https://github.com/XorTroll/Brew.js/releases/latest)

## The engine: duktape

   - Duktape is an embeddable Javascript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).
   - It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.
   - You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could fuck up the execution, so be sure to know what you're doing.

## Basic examples of API usage

 - If you just want to evaluate quickly some sources, it's easy to do:

   - First of all, include Brew.js's main header to use it:

   ``` cpp
   #include <Brew.js/Brew.hpp>
   ```

   - Then create a environment and the native context to use with the environment:

   ``` cpp
   Brew::API::NativeContext context = Brew::API::CreateContext();
   Brew::Environment myenv(context);
   ```

   - There are 3 possibilities of evaluating code:
     - Evaluating directly source code
     - Evaluating source code from a file
     - Evaluating a Node-like (package.json) project

     All of them return a execution result, which has information about the result of the execution (result as string, original result type, success / failure...)

   ``` cpp
   Brew::EvaluationResult res1 = myenv.EvaluateSourceCode("var num = 9;");
   Brew::EvaluationResult res2 = myenv.EvaluateFile("sdmc:/test.js");
   vector<Brew::Project> projects = myenv.GetProjectList("sdmc:/Projects");
   Brew::EvaluationResult res3 = myenv.EvaluateProject(projects[0]);
   ```

   - For more information, browse the documentation.

## Used libraries

   - duktape | C JavaScript engine | by svaarala
   - bitmap_image | BMP decoding | by ArashPartow
   - jsoncpp | JSON parsing for C++ | by nlohmann
   - nanojpeg | JPEG decoding | by Martin J. Fiedler
   - upng | PNG decoding | by elanthis
   - sf2dlib | 3DS GPU rendering via citro3d | by xerpi