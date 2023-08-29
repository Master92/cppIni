#include <doctest/doctest.h>

#include "../src/Entry.h"

TEST_CASE("Entry construction")
{
    Entry e;

    Entry meaningful("Blubb", 42);
    CHECK_EQ(meaningful.key(), "Blubb");
    CHECK_EQ(meaningful.value<int>(), 42);

    const int a = 69;
    Entry second("Bla", a);
}

struct DataStructure
{
    int a {42};
    float b {69.f};
    double c {1337.};

    explicit constexpr DataStructure(int val) : a(val) {}

    constexpr auto operator <=>(const DataStructure& other) const = default;
};

TEST_CASE_TEMPLATE("Entry construction with base type", T, char, int, float, DataStructure)
{
    constexpr const char* key = "Entry";
    Entry e(key, T(42));

    CHECK_EQ(e.key(), key);
    CHECK_EQ(e.value<T>(), T(42));

    const auto value = T(0);
    Entry e2(key, value);
    CHECK_EQ(e2.key(), key);
    CHECK_EQ(e2.value<T>(), value);
}

TEST_CASE("Entry equality")
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;

    Entry e1{key, value};
    Entry e2{key, value};
    Entry e3{key, 1337};
    Entry e4{"Bla", value};

    CHECK_EQ(e1, e1);
    CHECK_EQ(e1, e2);
    CHECK_NE(e1, e3);
    CHECK_NE(e1, e4);
}

TEST_CASE("Entry assignment")
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;

    Entry e1{key, value};
    const Entry e2{key, value};
    const Entry e3{key, 1337};
    const Entry e4{"Bla", value};
    const Entry e5{key, DataStructure(42)};

    e1 = e1;
    CHECK_EQ(e1, e2);

    e1 = e2;
    CHECK_EQ(e1, e2);

    e1 = e3;
    CHECK_EQ(e1, e3);

    e1 = e4;
    CHECK_EQ(e1, e4);

    e1 = e5;
    CHECK_NE(e1, e5);
    CHECK_EQ(e1, e4);
}

TEST_CASE("Entry movement")
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;

    Entry e1{key, value};
    const Entry e1Copy{e1};

    Entry e2{key, value};
    const Entry e2Copy{e2};

    Entry e3{key, 1337};
    const Entry e3Copy{e3};

    Entry e4{"Bla", value};
    const Entry e4Copy{e4};

    Entry e5{key, DataStructure(42)};
    const Entry e5Copy{e5};

    e1 = std::move(e2);
    CHECK_EQ(e1, e2Copy);

    e1 = std::move(e3);
    CHECK_EQ(e1, e3Copy);

    e1 = std::move(e4);
    CHECK_EQ(e1, e4Copy);

    e1 = std::move(e5);
    CHECK_NE(e1, e5Copy);
    CHECK_EQ(e1, e4Copy);
    CHECK_EQ(e5, e5Copy); // I know we moved from e5, but it should still be valid
}

TEST_CASE_TEMPLATE("Size of member types", T, char, int, double, DataStructure)
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;

    Entry e{key, T(value)};

    CHECK_EQ(e.size(), sizeof(T));
}

TEST_CASE("Changing the key of an Element")
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;
    constexpr auto key2 = "Test2";

    REQUIRE_NE(key, key2);

    Entry e{key, value};

    CHECK_EQ(e.key(), key);

    e.setKey(key2);

    CHECK_EQ(e.key(), key2);
}

TEST_CASE_TEMPLATE("Changing the value of an Element", T, char, int, double, DataStructure)
{
    constexpr auto key = "Test";
    constexpr auto value = 4211;
    constexpr auto valueObject = T(value);
    constexpr auto valueObject2 = T(1337);

    REQUIRE_NE(valueObject, valueObject2);

    Entry e{key, valueObject};

    CHECK_EQ(e.value<T>(), valueObject);

    e.setData(valueObject2);

    CHECK_EQ(e.value<T>(), valueObject2);
}

#if 0
#if __has_include("windows.h")

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>

static SIZE_T currentMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

TEST_CASE("Check for Entry memory leaks")
{
    auto memoryStart = currentMemoryUsage();
    SIZE_T intermediateUsage;

    std::cout << memoryStart << std::endl;
    {
        std::vector<Entry> v;

        for (size_t i = 0; i < size_t(10'000'000); i++) {
            v.emplace_back("asd", i);
        }

        intermediateUsage = currentMemoryUsage();
        std::cout << intermediateUsage << std::endl;
        CHECK_GT(intermediateUsage - memoryStart, 100'000'000);
    }

    std::cout << currentMemoryUsage() << std::endl;

    CHECK_GT(intermediateUsage - currentMemoryUsage(), 10'000'000);
}

#endif
#endif
