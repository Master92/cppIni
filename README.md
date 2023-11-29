# cppIni - A C++20 library for reading and writing INI files

[![Build](https://github.com/Master92/cppIni/actions/workflows/build.yaml/badge.svg)](https://github.com/Master92/cppIni/actions/workflows/build.yaml)
![License](https://img.shields.io/github/license/Master92/cppIni)
![GitHub stars](https://img.shields.io/github/stars/Master92/cppIni?label=%E2%AD%90%20Stars)

## Features

This project is a library for reading and writing INI files. It is written in C++20 and uses the STL.
It is tested with GCC 13, Clang 16 and MSVC 19.36. It should work with any compiler that supports C++20.

## Usage

``` cpp  
#include <cppIni/cppIni.hpp>

File ini("test.ini");
const auto intValue = init.get<int>("section", "key");
const auto newValue = 42;
ini.set("section", "key", newValue);
```

## License

cppIni is licensed under the MIT license. See [COPYING](https://github.com/Master92/cppIni/COPYING) for more information.
