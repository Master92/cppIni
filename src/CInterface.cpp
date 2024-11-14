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

#include <cstring>

#include <cppIni/cppIni_c.h>
#include <cppIni/cppIni.h>

/// Opens a file for reading and writing. Throws an exception if the file cannot
/// be opened.
///
/// \param[in] filename The name of the file to open
/// \return A pointer to a File object
void* cppIni_open(const char* filename)
{
    return new File(filename);
}

/// Closes a file that was opened with cppIni_open().
/// \param[in] file A pointer to a File object
void cppIni_close(void** const file)
{
    delete static_cast<File*>(*file);
}

/// \param[in] file A pointer to a File object
/// \param[in] section The name of the section to add
/// \param[in] key The name of the key to add
/// \param[in] value The value to add
void cppIni_set(void* const file, const char* const section, const char* const key, const char* value)
{
    static_cast<File*>(file)->set(section, key, value);
}

/// \param[in] file A pointer to a File object
/// \param[in] section The name of the section to get
/// \param[in] key The name of the key to get
/// \param[out] out A buffer to store the value in
/// \param[in] outSize The size of the buffer
/// \return A pointer to the buffer
const char* cppIni_gets(const void* const file, const char* const section, const char* const key, char* out, size_t outSize)
{
    const auto value = static_cast<const File*>(file)->get<std::string>(section, key);

    if (value.empty()) {
        return out;
    }

    std::strncpy(out, value.data(), outSize);
    return out;
}

/// \see cppIni_gets
/// \param[in] file A pointer to a File object
/// \param[in] section The name of the section to get
/// \param[in] key The name of the key to get
/// \return The value of the key
int cppIni_geti(const void* const file, const char* const section, const char* const key)
{
    return static_cast<const File*>(file)->get<int>(section, key);
}

/// \see cppIni_gets
/// \param[in] file A pointer to a File object
/// \param[in] section The name of the section to get
/// \param[in] key The name of the key to get
/// \return The value of the key
float cppIni_getf(const void* const file, const char* const section, const char* const key)
{
    return static_cast<const File*>(file)->get<float>(section, key);
}
