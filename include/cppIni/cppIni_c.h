// cppIni - A C++20 library for reading and writing INI files
// Copyright (C) 2023-2024 Nils Hofmann <nils.friedchen@googlemail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <stdlib.h>
#include <cppIni/cppini_export.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pFile;
/// \file cppIni_c.h
/// \brief The C API for cppIni
///
/// \details
/// This file contains the C API for cppIni. It is a very thin wrapper around
/// the C++ API. It is intended for use with languages that do not support
/// C++.

CPPINI_EXPORT pFile cppIni_open(const char* filename); ///< Opens a file
CPPINI_EXPORT void cppIni_close(pFile* file); ///< Closes a file
CPPINI_EXPORT void cppIni_set(pFile file, const char* section, const char* key, const char* value); ///< Sets a value
CPPINI_EXPORT const char* cppIni_gets(pFile file, const char* section, const char* key, char* out, size_t outSize); ///< Gets a string
CPPINI_EXPORT int cppIni_geti(pFile file, const char* section, const char* key); ///< Gets an integer
CPPINI_EXPORT float cppIni_getf(pFile file, const char* section, const char* key); ///< Gets a float

#ifdef __cplusplus
}
#endif
