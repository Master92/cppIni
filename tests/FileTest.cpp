#include <doctest/doctest.h>
#include <cstring>

#include "../src/File.h"

using namespace std::literals;

static const std::string fileName = WORKING_DIR + "/res/test.ini"s;

TEST_SUITE_BEGIN("File");

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
    CHECK(f.findEntry("Section1.Entry1"));
    CHECK_EQ(f.findEntry("Section1.Entry1")->value<std::string_view>(), "Value1"sv);
    CHECK_EQ(f.findEntry("Section1.IntEntry")->value<int>(), 42);
}

TEST_CASE("Open file from static method")
{
    REQUIRE_NOTHROW(const auto f = File::open(fileName));

    const auto f = File::open(fileName);
    const auto f2 = File{fileName};
    CHECK_EQ(f, f2);
}

TEST_SUITE_END();
