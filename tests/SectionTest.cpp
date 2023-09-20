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

#include <cppIni/Section.h>

TEST_SUITE_BEGIN("Section");

class SectionFixture
{
public:
    SectionFixture() : s(title) {}

protected:
    constexpr static auto title = "Test section";
    Section s;
};

TEST_CASE("Empty section")
{
    constexpr auto title = "Test section";

    Section s{title};

    CHECK_EQ(s.title(), title);
    CHECK_EQ(s.entries().size(), 0);
}

TEST_CASE("Subsection title construction")
{
    constexpr auto title = "Test section";
    constexpr auto subSectionTitle = "Subsection";

    Section section{title};
    Section subSection{subSectionTitle, &section};

    CHECK_EQ(section.title(), title);
    CHECK_EQ(subSection.parent(), &section);
    CHECK(subSection.isSubsection());
    CHECK_EQ(subSection.title(), subSectionTitle);
    CHECK_EQ(subSection.fqTitle(), std::string(title) + "." + subSectionTitle);
    CHECK_EQ(subSection.entries().size(), 0);
}

TEST_CASE("Entry with parent")
{
    constexpr auto title = "Test section";
    constexpr auto key = "Test key";
    constexpr auto value = 42;

    Section s{title};
    Entry e{key, value, &s};
    s.createEntry(key, value);

    CHECK_EQ(e.key(), key);
    CHECK_EQ(e.value<int>(), value);
    CHECK_EQ(e.parent(), &s);

    CHECK_EQ(s.entries().size(), 1);
    CHECK_EQ(s.entries().at(key).parent(), &s);
}

TEST_CASE("Entry with subsection as parent")
{
    constexpr auto title = "Test section";
    constexpr auto subSectionTitle = "Subsection";
    constexpr auto key = "Test key";

    Section section{title};
    Section subSection{subSectionTitle, &section};

    subSection.createEntry(key, 42);
    CHECK_EQ(subSection.entries().at(key).parent(), &subSection);
    CHECK_EQ(subSection.entries().at(key).fqKey(), std::string(title) + "." + subSectionTitle + "." + key);
}

TEST_CASE("Entry assignment with different parents")
{
    constexpr auto title1 = "Test section 1";
    constexpr auto title2 = "Test section 2";
    constexpr auto key1 = "Test key";
    constexpr auto key2 = "Test key 2";
    constexpr auto value1 = 42;
    constexpr auto value2 = 1337;

    Section section1{title1};
    Section section2{title2};

    Entry e1{key1, value1, &section1};
    Entry e2{key2, value2, &section2};
    REQUIRE_EQ(e1.parent(), &section1);
    REQUIRE_EQ(e2.parent(), &section2);
    CHECK_NE(e1.parent(), e2.parent());

    e1 = e2;
    CHECK_EQ(e1.parent(), e2.parent());
}

TEST_CASE_FIXTURE(SectionFixture, "Add existing entries")
{
    constexpr auto key = "Test Key";
    constexpr auto value = 42;

    Entry e{key, value};
    s.addEntry(e);

    CHECK_EQ(s.entries().size(), 1);
    CHECK_EQ(s.entries().at(key), e);
}

TEST_CASE_FIXTURE(SectionFixture, "Create entry during insertion")
{
    constexpr auto key = "Test Key";
    constexpr auto value = 42;

    s.createEntry(key, value);

    const Entry checkEntry{key, value, &s};

    CHECK_EQ(s.entries().size(), 1);
    CHECK_EQ(s.entries().at(key), checkEntry);
}

TEST_CASE_FIXTURE(SectionFixture, "Find an Entry by name")
{
    constexpr auto key = "Test Key";
    constexpr auto value = 42;

    s.createEntry(key, value);

    const auto e = s.findEntry(key);

    REQUIRE_NE(e, nullptr);
    CHECK_EQ(e, &s.entries().at(key));
    CHECK_EQ(e->key(), key);
    CHECK_EQ(e->value<int>(), value);
}

TEST_CASE_FIXTURE(SectionFixture, "Try to find an Entry that doesn't exist")
{
    constexpr auto key = "Test Key";
    constexpr auto value = 42;

    s.createEntry(key, value);

    const auto e = s.findEntry("Nonexistent key");

    CHECK_EQ(e, nullptr);
}

TEST_CASE("Equality of two sections")
{
    constexpr auto title = "Test section";
    constexpr auto key = "Test key";
    constexpr auto value = 42;

    Section s1{title};
    Section s2{title};

    s1.createEntry(key, value);
    s2.createEntry(key, value);

    CHECK_EQ(s1, s2);
}

TEST_CASE("Inequality of two sections")
{
    constexpr auto title = "Test section";
    constexpr auto title2 = "Test section 2";
    constexpr auto key = "Test key";
    constexpr auto value = 42;

    Section s1{title};
    Section s2{title};
    Section s3{title2};

    s1.createEntry(key, value);
    s3.createEntry(key, value);

    CHECK_NE(s1, s2);
    CHECK_NE(s1, s3);
}

TEST_SUITE_END();
