# Brew.js - C++ high-level JavaScript API for Nintendo homebrew

- Development version: **2.0 (early commits, still not released)**

- Latest released version: [1.0](https://github.com/XorTroll/bjs/releases/latest)

## The engine: duktape

- Duktape is an embeddable JavaScript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).

- It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.

- You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could fuck up the execution, so be sure to know what you're doing.

## Basic examples of API usage

- If you just want to evaluate JavaScript without messing up with the C++ API, that's easy to do:

- First of all, include Brew.js's main header to use it:

``` cpp
#include <bjs/bjs.hpp>
```

- Then create a environment and the native context to use with the environment:

``` cpp
bjs::js::NativeContext context = bjs::js::CreateContext();
bjs::Environment myenv(context);
```

- There are 3 possibilities of evaluating code:

  - Evaluating directly source code
  - Evaluating source code from a file
  - Evaluating a Node-like (package.json) project

  All of them return a execution result, which has information about the result of the execution (result as string, original result type, success / failure...)

``` cpp
bjs::EvaluationResult res1 = myenv.EvaluateSourceCode("var num = 9;");
bjs::EvaluationResult res2 = myenv.EvaluateFile("sdmc:/test.js");
vector<bjs::Project> projects = myenv.GetProjectList("sdmc:/Projects");
bjs::EvaluationResult res3 = myenv.EvaluateProject(projects[0]);
```

- For more information, check the documentation.

## Embedded libraries

- **duktape** - lightweight C JavaScript engine, by svaarala
- **bitmap_image** - C++ BMP image decoding and encoding, by ArashPartow
- **jsoncpp** - modern JSON for C++, by nlohmann
- **nanojpeg** - C JPEG image decoding and encoding, by Martin J. Fiedler
- **upng**, C PNG image decoding and encoding, by elanthis
- **sf2dlib**, C 3DS GPU rendering libraried (using citro3d), by xerpi