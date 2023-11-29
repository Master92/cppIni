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

template<class T>
auto File::get(std::string_view section, std::string_view name) const -> T
{
    if (const auto entry = findEntry(std::format("{}.{}", section, name))) {
        return entry->value<T>();
    }

    return T();
}
