# cppIni - A C++20 library for reading and writing INI files

Branch | Status                                                                                                                                                                | Coverage
--- |-----------------------------------------------------------------------------------------------------------------------------------------------------------------------| ---
`master` | [![Build](https://github.com/Master92/cppIni/actions/workflows/build.yaml/badge.svg?branch=master)](https://github.com/Master92/cppIni/actions/workflows/build.yaml)  | [![codecov](https://codecov.io/gh/Master92/cppIni/branch/master/graph/badge.svg?token=V66BUECAMV)](https://codecov.io/gh/Master92/cppIni)
`develop` | [![Build](https://github.com/Master92/cppIni/actions/workflows/build.yaml/badge.svg?branch=develop)](https://github.com/Master92/cppIni/actions/workflows/build.yaml) | [![codecov](https://codecov.io/gh/Master92/cppIni/branch/develop/graph/badge.svg?token=V66BUECAMV)](https://codecov.io/gh/Master92/cppIni/tree/develop)

[![Release](https://img.shields.io/github/v/tag/Master92/cppIni?label=release)](https://github.com/Master92/cppIni/releases)
![License](https://img.shields.io/github/license/Master92/cppIni)
![GitHub stars](https://img.shields.io/github/stars/Master92/cppIni?label=%E2%AD%90%20Stars)

## Features

This project is a library for reading and writing INI files. It is written in C++20 and uses the STL.
It is tested with GCC 13, Clang 16 and MSVC 19.36. It should work with any compiler that supports C++20.

### Reading and manipulating INI files

The library is able to read INI files and parses them automatically. It is able to read the following types:

- `bool`              
- `char`              
- `short`             
- `int`               
- `long`              
- `long long`         
- `unsigned char`     
- `unsigned short`    
- `unsigned int`      
- `unsigned long`     
- `unsigned long long`
- `float`             
- `double`            
- `long double`       
- `std::string`       
- `std::string_view`  
- `const char*`

Accessing a value is done with the `get` template-function. It takes the section and the key as parameters and returns
the value as the specified type `T`. If the value does not exist, the default value (`T()`) is returned.

Setting a value is done with the `set` template-function. It takes the section, the key and the value as parameters.
The value is converted to a string and written to the file. If the section or the key does not exist, it is created.
On every write, the file is completely rewritten.

## Usage

### C++:

``` cpp  
#include <cppIni/cppIni.hpp>

File ini("test.ini");
const auto intValue = ini.get<int>("section", "key");
const auto newValue = 42;
ini.set("section", "key", newValue);
```

### C:

``` c
#include <cppIni/cppIni_c.h>

void* ini = cppIni_open("test.ini");
const int intValue = cppIni_geti(ini, "section", "key");
const int newValue = 42;
cppIni_set(ini, "section", "key", newValue);
cppIni_close(&ini);
```

## License

cppIni is licensed under the GPLv3. See [COPYING](https://github.com/Master92/cppIni/blob/master/COPYING) for more information.
