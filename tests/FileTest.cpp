/*
 * cppIni - A C++20 library for reading and writing INI files
 * Copyright (C) 2023 Nils Hofmann <nils.friedchen@googlemail.com>
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <cppIni/File.h>

using namespace std::literals;

static const std::string fileName = std::format("{}{}", WORKING_DIR, "/res/test.ini");

TEST_SUITE_BEGIN("File");

class FileFixture
{
public:
    FileFixture() {
        std::filesystem::copy_file(::fileName, fileName);
    }
    ~FileFixture() {
        std::filesystem::remove(fileName);
    }
protected:
    const std::string fileName = std::format("{}{}", WORKING_DIR, "/res/tmp.ini");
};

TEST_CASE("Failing construction of an empty File object")
{
    CHECK_THROWS(File{""});
}

TEST_CASE("Construction of File object")
{
    CHECK_NOTHROW(File{fileName});
}

TEST_CASE("Successful parse of test.ini")
{
    const auto f = File{fileName};

    REQUIRE_GT(f.sections().size(), 0);
    CHECK(f.findSection("Section1"));
    CHECK(f.findSection("Section1.Subsection1"));
    CHECK(f.findEntry("Section1.Entry1"));
    CHECK_EQ(f.findEntry("Section1.Entry1")->value<std::string_view>(), "Value1"sv);
    CHECK_EQ(f.findEntry("Section1.IntEntry")->value<int>(), 42);
    CHECK_EQ(f.get<int>("Section1", "IntEntry"), 42);
    CHECK_EQ(f.get<double>("Section1.Subsection1", "DoubleEntry"), 3.1415);
    CHECK_EQ(f.get<std::string_view>("Section1.Subsection2", "StringEntry"), "Hello World!"sv);
    CHECK(f.get<bool>("Section1.Subsection2.Subsubsection1", "BoolEntry"));
}

TEST_CASE("Open file from static method")
{
    REQUIRE_NOTHROW(const auto f = File::open(fileName));

    const auto f = File::open(fileName);
    const auto f2 = File{fileName};
    CHECK_EQ(f, f2);
}

TEST_CASE("Get the value of an entry")
{
    const auto f = File{fileName};
    CHECK_EQ(f.get<std::string_view>("Section1", "Entry1"), "Value1"sv);
}

TEST_CASE("Get the value of an entry that doesn't exist")
{
    const auto f = File{fileName};
    CHECK_EQ(f.get<int>("Section1", "Entry2"), int());
}

TEST_CASE("Create a section")
{
    auto f = File{fileName};
    f.getSection("Section2");
    CHECK(f.findSection("Section2"));
}

TEST_CASE("Create a subsection of an existing and a non-existing Section")
{
    auto f = File{fileName};
    f.getSection("Section2.Subsection1");
    CHECK(f.findSection("Section2.Subsection1"));
    CHECK(f.findSection("Section2"));

    f.getSection("Section2.Subsection1.Subsubsection1");
    CHECK(f.findSection("Section2.Subsection1.Subsubsection1"));
}

TEST_CASE("Call findSection to get an existing Section")
{
    const auto f = File{fileName};
    const auto section = f.findSection("Section1");
    REQUIRE(section);
    CHECK_EQ(section->title(), "Section1");
    REQUIRE(section->findEntry("Entry1"));
    CHECK_EQ(section->findEntry("Entry1")->value<std::string_view>(), "Value1"sv);
}

TEST_CASE("Call findSection to get a non-existing Section")
{
    const auto f = File{fileName};
    const auto section = f.findSection("Section2");
    CHECK_EQ(section, nullptr);
}

TEST_CASE("Equality operator")
{
    const auto f = File{fileName};
    const auto f2 = File{fileName};
    CHECK_EQ(f, f2);
}

TEST_CASE_FIXTURE(FileFixture, "Change a value with set")
{
    constexpr auto newValue = "NewValue"sv;

    auto f = File{fileName};
    f.set("Section1", "Entry1", "NewValue");
    CHECK_EQ(f.get<std::string_view>("Section1", "Entry1"), newValue);

    const auto f2 = File{fileName};
    CHECK_EQ(f.get<std::string_view>("Section1", "Entry1"), newValue);
}

TEST_CASE("Write a file to disk")
{
    constexpr auto testFileName = "testWrite.ini";
    {
        auto f = File{testFileName};
        f.set("General", "IntValue", 42);
        f.set("General", "StringValue", "Hello World");
        f.set("General.Subsection", "SubsectionValue", 3.14);
        f.set("Specialized.Subsection.Subsubsection", "BoolValue", true);

        const auto f2 = File{testFileName};
        CHECK_EQ(f, f2);
    }

    // Cleanup
    std::filesystem::remove(testFileName);
}

TEST_SUITE_END();
