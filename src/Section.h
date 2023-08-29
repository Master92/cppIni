#pragma once

#include "Entry.h"

#include <vector>

/// \brief Represents a section in a configuration file
/// \details A section is a collection of Entry objects with a title (e.g. [Section]) in a configuration file
/// \note A section has a title and a list of Entry objects
class Section {
public:
    explicit Section(std::string_view title, Section* parent = nullptr); ///< Constructor with title

    auto title() const -> std::string_view { return m_title; } ///< Title as std::string_view
    auto fqTitle() const -> std::string; ///< Fully qualified title (e.g. "Section1.Section2")

    auto parent() const -> const Section* { return m_parent; } ///< Parent Section
    auto isSubsection() const -> bool { return m_parent != nullptr; } ///< Returns true if this Section is a subsection

    auto addEntry(Entry entry) -> void; ///< Add an Entry object to the section

    template<class T>
    auto createEntry(std::string_view key, T value) -> void; ///< Create an Entry object in place and add it to the section

    constexpr auto entries() const -> const std::vector<Entry>& { return m_entries; } ///< List of Entry objects

private:
    std::string m_title;
    std::vector<Entry> m_entries;
    Section *m_parent {nullptr};
};

/// \details The parameters are forwarded to the Entry constructor and a pointer to this Section object is added as the parent
/// \arg key The key of the Entry
/// \arg value The value of the Entry
template<class T>
auto Section::createEntry(std::string_view key, T value) -> void
{
    m_entries.emplace_back(key, std::move(value), this);
}