/*
 * cppIni - A C++20 library for reading and writing INI files
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
#include <cppIni/Section.h>

#include <filesystem>
#include <vector>
#include <format>

/// \brief Represents a file on disk.
/// A file is a collection of Sections.
class CPPINI_EXPORT File {
public:
    explicit File(std::string_view filename); ///< Constructor.
    virtual ~File(); ///< Destructor.

    static File open(std::string_view filename); ///< Open a file. Throws if the file cannot be opened.
    void open(); ///< Open the file. Throws if the file cannot be opened.
    void flush(); ///< Write the file to disk.

    template<class T>
    auto set(std::string_view section, std::string_view key, T value) -> void; ///< Set a value in a section.

    auto getSection(std::string_view fqTitle) -> Section*; ///< Get a Section by fully qualified title (e.g. "Section1.Section2")
    auto findSection(std::string_view title) const -> const Section*; ///< Find a Section by title.
    auto findEntry(std::string_view name) const -> const Entry*; ///< Find an Entry by name.

    template<class T>
    auto get(std::string_view section, std::string_view name) const -> T; ///< Get an Entry by name and convert it to the specified type.

    constexpr auto sections() const -> const auto& { return m_sections; }

    auto operator==(const File& other) const -> bool; ///< Equality operator.
    auto operator!=(const File& other) const -> bool { return !(*this == other); }; ///< Inequality operator.

private:
    void parse(); ///< Parse the file.

private:
    std::string m_filename{};

    std::vector<Section*> m_sections{};
};

/// \details Calls findEntry() and returns the value of the Entry if it exists.
/// Otherwise, returns a default-constructed value.
/// \arg section The fully qualified title of the Section to search in.
/// \arg name The name of the Entry to search for.
/// \tparam T The type of the value to return.
/// \returns The value of the Entry if it exists, otherwise a default-constructed value.
template<class T>
auto File::get(std::string_view section, std::string_view name) const -> T
{
    if (const auto entry = findEntry(std::format("{}.{}", section, name))) {
        return entry->value<T>();
    }

    return T();
}

/// \details The parameters are forwarded to the Section::setEntry() method. The Section is created if it does not exist.
/// \arg section The title of the Section to set the value in.
/// \arg key The key of the Entry to set.
/// \arg value The value of the Entry to set.
template<class T>
auto File::set(std::string_view section, std::string_view key, T value) -> void
{
    if (auto s = std::ranges::find_if(m_sections,
                                      [section](const auto& s) { return s->fqTitle() == section; });
            s != m_sections.end()) {
        (*s)->setEntry({key, value});
    } else {
        auto targetSection = getSection(section);
        targetSection->setEntry({key, value});
    }

    flush();
}
