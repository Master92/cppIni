/*
 * cppIni - C++20/23 library for dealing with settings files
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

#include <doctest/doctest.h>

#include <cppIni/Variant.h>

class MyTestClass {
public:
    MyTestClass() : m_value{0}, m_name{""} {}
    MyTestClass(int value, std::string name) : m_value{value}, m_name{name} {}

    auto value() const -> int
    {
        return m_value;
    }

    auto name() const -> std::string
    {
        return m_name;
    }

    auto operator==(const MyTestClass& other) const -> bool
    {
        return m_value == other.m_value && m_name == other.m_name;
    }

    auto operator!=(const MyTestClass& other) const -> bool
    {
        return !(*this == other);
    }
private:
    int m_value;
    std::string m_name;
};

TEST_SUITE_BEGIN("Variant");

TEST_CASE_TEMPLATE("Construction with several types", T, bool, int, double, MyTestClass)
{
    Variant v{T{}};

    CHECK(v.is<T>());
    CHECK_EQ(v.get<T>(), T{});
}

TEST_CASE("Check for existence of value after some method calls")
{
    const auto assigner = [](const MyTestClass& v) -> Variant {
        return Variant{v};
    };

    const auto myValue = MyTestClass{42, "Test"};
    const auto variant = assigner(myValue);

    CHECK_EQ(variant.get<MyTestClass>(), myValue);

    const auto checker = [](const Variant &a, const auto &b) -> bool {
        return a.get<std::decay_t<MyTestClass>>() == b;
    };

    CHECK(checker(variant, myValue));
}

TEST_CASE("Copying a Variant")
{
    const auto myValue = MyTestClass{42, "Test"};
    const auto variant = Variant{myValue};

    CHECK_EQ(variant.get<MyTestClass>(), myValue);

    const auto copy(variant);
    CHECK_EQ(variant.get<MyTestClass>(), copy.get<MyTestClass>());

    CHECK_NE(&variant.get<MyTestClass>(), &copy.get<MyTestClass>());
}

TEST_CASE("Access by conversion operator")
{
    const auto myValue = MyTestClass{42, "Test"};
    const auto variant = Variant{myValue};

    CHECK_EQ(variant.get<MyTestClass>(), myValue);

    const auto copy = static_cast<MyTestClass>(variant);
    CHECK_EQ(variant.get<MyTestClass>(), copy);

    const auto myInt = 42;
    const auto primitiveVariant = Variant{myInt};
    CHECK_EQ(primitiveVariant.get<int>(), myInt);

    const auto primitiveCopy = int(primitiveVariant);
    CHECK_EQ(primitiveVariant.get<int>(), primitiveCopy);
}

TEST_SUITE_END();
