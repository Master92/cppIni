/*
 * cppIni - A C++20 library for reading and writing INI files
 * Copyright (C) 2023-2024 Nils Hofmann <nils.friedchen@googlemail.com>
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

#include <array>
#include <filesystem>
#include <format>

#include <cppIni/cppIni_c.h>
#include <doctest/doctest.h>
#include "utils.h"

static const std::string fileName = std::format("{}{}", WORKING_DIR, "/res/test.ini");;

TEST_SUITE_BEGIN("CInterface");

TEST_CASE("Construction of File object")
{
    void* file;
    CHECK_NOTHROW(file = cppIni_open(fileName.c_str()));
    CHECK_NE(file, nullptr);
    CHECK_NOTHROW(cppIni_close(&file));
}

TEST_CASE("Read a string entry")
{
    auto file = utils::ScopeGuard<void*, cppIni_close>(cppIni_open(fileName.c_str()));

    std::array<char, 64> buffer{0};
    cppIni_gets(*file, "Section1", "Entry1", buffer.data(), buffer.size());

    CHECK_EQ(std::string_view{buffer.data()}, "Value1");
}

TEST_CASE("Try to read a non-existing entry")
{
    auto file = utils::ScopeGuard<void*, cppIni_close>(cppIni_open(fileName.c_str()));

    std::array<char, 64> buffer{0};
    CHECK_EQ(cppIni_gets(*file, "Section1", "NonExistingEntry", buffer.data(), buffer.size()), buffer.data());
    CHECK_EQ(buffer[0], '\0');
}

TEST_CASE("Change a value")
{
    constexpr auto newValue = 1337;

    utils::TempFile tmpFile(fileName);
    auto file = utils::ScopeGuard<void*, cppIni_close>(cppIni_open(tmpFile.filename().data()));

    const auto previousValue = cppIni_geti(*file, "Section1", "IntEntry");
    CHECK_NE(previousValue, newValue);
    cppIni_set(*file, "Section1", "IntEntry", std::to_string(newValue).c_str());
    CHECK_EQ(cppIni_geti(*file, "Section1", "IntEntry"), newValue);
}

TEST_CASE("Read an integer entry")
{
    auto file = utils::ScopeGuard<void*, cppIni_close>(cppIni_open(fileName.c_str()));

    CHECK_EQ(cppIni_geti(*file, "Section1", "IntEntry"), 42);
}

TEST_CASE("Read a floating point value entry")
{
    auto file = utils::ScopeGuard<void*, cppIni_close>(cppIni_open(fileName.c_str()));

    CHECK_LT(std::abs(cppIni_getf(*file, "Section1.Subsection1", "DoubleEntry") - 3.1415), 0.001);
}

TEST_SUITE_END();
