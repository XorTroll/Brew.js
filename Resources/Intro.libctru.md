# Welcome to Brew.js

**Welcome to the documentation of Brew.js for Nintendo 3DS!**

## The engine: duktape

- Duktape is an embeddable JavaScript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).

- It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.

- You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could crash the execution, so be sure to know what you're doing. It's not recommended to do mix duktape functions with Brew.js API.

## Nintendo 3DS (`CTR`) specifications

- Nintendo 3DS version of Brew.js libs has some specific modules:
  - `ctr` - Basic functions and constants for the Nintendo 3DS system

  - `sf2d` - Wrapper for sf2dlib, using GPU rendering via citro3d and citro2d (both libraries are required)

  - Platform-specific `console` and `input` modules

  - Platform-specific details in `os`, `path` and `process` Node.js modules.

## Simple JavaScript evaluation

- If you just want to evaluate JavaScript without messing up with the C++ API, that's easy to do:

- First of all, include Brew.js's main header to use it. It may look like a JS file, but it is not:

``` cpp
#include <bjs/brew.js>
```

- Then, create a environment and the native context to use within the environment:

``` cpp
bjs::js::NativeContext context = bjs::js::CreateContext();
bjs::Environment myenv(context);
```

- There are 3 possibilities of evaluating JS source code:

  - From a std::string
  - From an external file
  - From a Node-like project (a folder with a package.json file)

  All of them return a execution result, which has information about the result of the execution (result string, original result type, whether it succeeded or failed...)

``` cpp
bjs::EvaluationResult res1 = myenv.EvaluateSourceCode("var num = 9;");
bjs::EvaluationResult res2 = myenv.EvaluateFile("sdmc:/test.js");
std::vector<bjs::Project> projects = myenv.GetProjectList("sdmc:/Projects");
bjs::EvaluationResult res3 = myenv.EvaluateProject(projects[0]);
```

- For more information, check the [web documentation](https://xortroll.github.io/Brew.js/)