#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <cppIni/cppIni.h>

TEST_CASE("Construction test")
{
    CHECK_EQ(one(), 1);
}
