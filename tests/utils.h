/*
 * cppIni - A C++20 library for reading and writing INI files
 * Copyright (C) 2024 Nils Hofmann <nils.friedchen@googlemail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <filesystem>

namespace utils
{

/// \brief Class for managing lifetime of a pointer
///
/// \details This class is a RAII wrapper for raw pointers. It is intended to be used as automatic variable in a function
///          or as non-static data member in a class. It takes ownership of the pointer and deletes the managed object
///          when it goes out of scope.
///
/// \tparam T The type of the object managed by the pointer
template<class T, void(* Deleter)(T*)>
class ScopeGuard
{
public:
    explicit ScopeGuard(T obj) : m_obj(obj) {}
    ~ScopeGuard() { Deleter(&m_obj); }

    T& operator*() { return m_obj; }

private:
    T m_obj;
};

/// \brief Class for managing the lifetime of a temporary copy of a file
///
/// \details This class is a RAII wrapper for a temporary file. It is intended to be used as automatic variable in a
///          function or as non-static data member in a class. It takes ownership of the file and deletes the file
///          when it goes out of scope. The file is copied during construction.
class TempFile
{
public:
    explicit TempFile(const std::string& fileName)
    {
        std::filesystem::copy(fileName, filename());
    }
    ~TempFile()
    {
        std::filesystem::remove(filename());
    }

    constexpr static std::string_view filename() { return "tmp.ini"; }
};
}
