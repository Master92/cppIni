#include <doctest/doctest.h>

#include "../src/Section.h"

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

TEST_CASE_FIXTURE(SectionFixture, "Add existing entries")
{
    Entry e{"key", 42};
    s.addEntry(e);

    CHECK_EQ(s.entries().size(), 1);
    CHECK_EQ(s.entries().at(0), e);
}

TEST_CASE_FIXTURE(SectionFixture, "Create entry during insertion")
{
    constexpr auto key = "Test Key";
    constexpr auto value = 42;

    s.createEntry(key, value);

    const Entry checkEntry{key, value};

    CHECK_EQ(s.entries().size(), 1);
    CHECK_EQ(s.entries().at(0), checkEntry);
    CHECK_NE(s.entries().at(0).data(), checkEntry.data());
}
