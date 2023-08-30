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

#include "Section.h"

#include <vector>

/// \brief Represents a file on disk.
/// A file is a collection of Sections.
class File {
public:
    explicit File(std::string_view filename); ///< Constructor.
    virtual ~File() = default; ///< Destructor.

    static File open(std::string_view filename); ///< Open a file. Throws if the file cannot be opened.
    void open(); ///< Open the file. Throws if the file cannot be opened.

    auto findSection(std::string_view title) const -> const Section*; ///< Find a Section by title.
    auto findEntry(std::string_view name) const -> const Entry*; ///< Find an Entry by name.

    constexpr auto sections() const -> const auto& { return m_sections; }

    auto operator==(const File& other) const -> bool = default; ///< Equality operator.
    auto operator!=(const File& other) const -> bool = default; ///< Inequality operator.

private:
    void parse(); ///< Parse the file.

private:
    std::string m_filename{};

    std::vector<Section> m_sections{};
};
