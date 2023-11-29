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

#pragma once

#include <cppIni/cppini_export.h>
#include <cppIni/Entry.h>

#include <unordered_map>

/// \brief Represents a section in a configuration file
/// \details A section is a collection of Entry objects with a title (e.g. [Section]) in a configuration file
/// \note A section has a title and a list of Entry objects
class CPPINI_EXPORT Section {
public:
    explicit Section(std::string_view title, const Section* parent = nullptr); ///< Constructor with title

    auto title() const -> std::string_view { return m_title; } ///< Title as std::string_view
    auto fqTitle() const -> std::string; ///< Fully qualified title (e.g. "Section1.Section2")

    constexpr auto parent() const -> const Section* { return m_parent; } ///< Parent Section
    auto isSubsection() const -> bool { return m_parent != nullptr; } ///< Returns true if this Section is a subsection

    auto addEntry(Entry entry) -> void; ///< Add an Entry object to the section

    template<class T>
    auto createEntry(std::string_view key, T value) -> void; ///< Create an Entry object in place and add it to the section

    constexpr auto entries() const -> const auto& { return m_entries; } ///< List of Entry objects

    auto findEntry(std::string_view name) const -> const Entry*; ///< Find an Entry object by name

    auto operator==(const Section& other) const -> bool; ///< Equality operator
    auto operator!=(const Section& other) const -> bool = default; ///< Inequality operator
private:
    std::string m_title;
    std::unordered_map<std::string, Entry> m_entries;
    const Section *m_parent {nullptr};
};

/// \details The parameters are forwarded to the Entry constructor and a pointer to this Section object is added as the parent
/// \arg key The key of the Entry
/// \arg value The value of the Entry
template<class T>
auto Section::createEntry(std::string_view key, T value) -> void
{
    m_entries.emplace(key, Entry{key, value, this});
}
