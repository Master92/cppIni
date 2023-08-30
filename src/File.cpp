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

#include "File.h"

#include <fstream>
#include <stdexcept>

/// \param filename The filename of the file to open.
File::File(std::string_view filename)
: m_filename{filename}
{
    open();
}

/// \param filename The filename of the file to open.
File File::open(std::string_view filename)
{
    return File{filename};
}

/// \throws std::runtime_error if the file cannot be opened.
void File::open()
{
    if (m_filename.empty()) {
        throw std::runtime_error{"Filename is empty"};
    }

    parse();
}

/// \param title The title of the Section to find.
/// \returns A pointer to the Section if found, nullptr otherwise.
auto File::findSection(std::string_view title) const -> const Section*
{
    for (const auto& section : m_sections) {
        if (section.fqTitle() == title) {
            return &section;
        }
    }

    return nullptr;
}

/// \param name The name of the Entry to find.
/// \returns A pointer to the Entry if found, nullptr otherwise.
/// \see Section::findEntry
auto File::findEntry(std::string_view name) const -> const Entry*
{
    const Section* section = nullptr;

    if (name.find('.')) {
        section = findSection(name.substr(0, name.find_last_of('.')));
    }

    if (section) {
        return section->findEntry(name.substr(name.find_last_of('.') + 1));
    }

    return nullptr;
}

/// \details This function is called by the constructor. It should not be called directly.
/// \throws std::runtime_error if the file cannot be opened.
/// \see File::open for the public function.
auto File::parse() -> void
{
    auto content = std::ifstream{m_filename};

    for (std::string line; std::getline(content, line);) {
        const std::string_view lineView{line};

        if (lineView.empty()) {
            continue;
        }

        if (line[0] == '[') {
            if (line.find('.') != std::string::npos) {
                m_sections.emplace_back(lineView.substr(lineView.find_last_of('.') + 1, lineView.find_last_of(']') - 1), &m_sections.back());
            } else {
                m_sections.emplace_back(lineView.substr(1, lineView.find_last_of(']') - 1));
            }
        } else {
            m_sections.back().createEntry(lineView.substr(0, lineView.find('=')), lineView.substr(lineView.find('=') + 1));
        }
    }
}
