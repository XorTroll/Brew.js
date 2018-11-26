# Brew.js - C++ high-level JavaScript API for Nintendo 3DS/Switch

- Development version: **2.0 (only commits, still not released)**

- Latest released version: [1.0](https://github.com/XorTroll/bjs/releases/latest), very outdated, highly recommended to wait until 2.0 release

## The engine: duktape

- Duktape is an embeddable JavaScript engine, with a focus on portability and compact footprint, as it's described on its [GitHub page](https://github.com/svaarala/duktape).

- It supports ES5 almost entirely, and a few elements of ES6. It has Node.js Buffer bindings and TypedArray from ES2015, plus a partial support of ECMAScript E6 and E7.

- You can use duktape's functions from the code along with Brew.js API, but it's very unstable and it could fuck up the execution, so be sure to know what you're doing.

## Platform support

- Brew.js supports Nintendo 3DS (via libcrtu) and Nintendo Switch (libnx)

- Originally, more platforms were going to be supported, but I've reached the conclusion that it's better yo focus and add more support on currently most important platforms.

- That's why Nintendo DS support was deleted.

## Basic examples of API usage

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

## Used extra libraries

- **duktape** - lightweight C JavaScript engine, by svaarala
- **bitmap_image** - C++ BMP image decoding and encoding, by ArashPartow
- **jsoncpp** - modern JSON for C++, by nlohmann
- **nanojpeg** - C JPEG image decoding and encoding, by Martin J. Fiedler
- **upng**, C PNG image decoding and encoding, by elanthis
- **sf2dlib**, C 3DS GPU rendering libraried (using citro3d), by xerpi