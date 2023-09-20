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

#include <cppIni/Section.h>

#include <algorithm>

Section::Section(std::string_view title, Section* parent)
    : m_title(title)
    , m_parent(parent)
{

}

/// \note The entry is moved into the vector
/// \arg entry The Entry to add
auto Section::addEntry(Entry entry) -> void
{
    m_entries.insert(std::make_pair(entry.key(), std::move(entry)));
}

/// If the Section is a top-level Section, the title is returned.
/// Otherwise, the title is prefixed with the parent's fully qualified title and a dot.
/// \returns The fully qualified title of the Section.
auto Section::fqTitle() const -> std::string
{
    if (m_parent == nullptr) {
        return m_title;
    }

    return m_parent->fqTitle() + "." + m_title;
}

/// \param name The name of the Entry to find.
/// \returns A pointer to the Entry if found, nullptr otherwise.
auto Section::findEntry(std::string_view name) const -> const Entry*
{
    const auto entry = m_entries.find(std::string(name));

    if (entry != m_entries.cend()) {
        return &entry->second;
    }

    return nullptr;
}

/// \details Two Sections are equal if they have the same title and the same entries. Their parents are not compared.
/// \param other The Section to compare to.
/// \returns true if the Sections are equal, false otherwise.
auto Section::operator==(const Section& other) const -> bool
{
    if (m_title != other.m_title) {
        return false;
    }

    return std::ranges::all_of(m_entries, [&other](const auto& entry) {
        const auto otherEntry = other.findEntry(entry.first);
        return otherEntry and entry.second.data() == otherEntry->data();
    });
}
