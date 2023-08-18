#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cppIni/cppIni.h>
#include <doctest/doctest.h>

TEST_CASE("Construction test")
{
    CHECK(one() == 1);
}
