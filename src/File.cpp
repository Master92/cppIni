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

#include <cppIni/File.h>

#include <algorithm>
#include <fstream>
#include <stdexcept>

/// \param filename The filename of the file to open.
File::File(std::string_view filename)
: m_filename{filename}
{
    open();
}

File::~File()
{
    for (auto& section : m_sections) {
        delete section;
    }

    m_sections.clear();
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

void File::flush()
{
    std::ofstream file{m_filename};

    for (const auto& section: m_sections) {
        file << std::format("[{}]\n", section->fqTitle());

        for (const auto& [_, entry]: section->entries()) {
            file << std::format("{}={}\n", entry.key(), entry.data());
        }

        file << "\n";
    }
}

/// \details Scans the vector of Sections for the parent and creates the tree including the new Section if it does not exist.
/// \param fqTitle The fully qualified title of the Section to create.
/// \returns A pointer to the created Section.
auto File::getSection(std::string_view fqTitle) -> Section*
{
    if (const auto section = findSection(fqTitle); section) {
        return const_cast<Section*>(section);
    }

    if (fqTitle.find('.') == std::string_view::npos) {
        m_sections.emplace_back(new Section(fqTitle));
        return m_sections.back();
    } else {
        const auto parent = getSection(fqTitle.substr(0, fqTitle.find_last_of('.')));
        m_sections.emplace_back(new Section(fqTitle.substr(fqTitle.find_last_of('.') + 1), parent));
        return m_sections.back();
    }
}

/// \param title The title of the Section to find.
/// \returns A pointer to the Section if found, nullptr otherwise.
auto File::findSection(std::string_view title) const -> const Section*
{
    const auto section = std::find_if(std::cbegin(m_sections), std::cend(m_sections), [&title](const auto& section) {
        return section->fqTitle() == title;
    });

    if (section == std::cend(m_sections)) {
        return nullptr;
    }

    return *section;
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

auto File::operator==(const File& other) const -> bool
{
    return std::equal(std::cbegin(m_sections), std::cend(m_sections), std::cbegin(other.m_sections), std::cend(other.m_sections), [](const auto& lhs, const auto& rhs) {
        return *lhs == *rhs;
    });
}

/// \details This function is called by the constructor. It should not be called directly.
/// \throws std::runtime_error if the file cannot be opened.
/// \see File::open for the public function.
auto File::parse() -> void
{
    auto content = std::ifstream{m_filename};

    for (std::string line; std::getline(content, line);) {
        std::string_view lineView{line};

        if (lineView.empty()) {
            continue;
        }

        if (lineView[0] == '[') {
            lineView = lineView.substr(1);

            Section* parent = lineView.at(0) == '.' ? m_sections.back() : nullptr;

            if (lineView[0] == '.') {
                lineView = lineView.substr(1);
            } else if (const auto section = std::find_if(std::begin(m_sections), std::end(m_sections), [&lineView](const auto& section) {
                return section->fqTitle() == lineView.substr(0, lineView.find_last_of('.'));
            }); section != std::end(m_sections)) {
                parent = *section;
                lineView = lineView.substr(lineView.find_last_of('.') + 1);
            }

            m_sections.emplace_back(new Section(lineView.substr(0, lineView.find(']')), parent));
        } else {
            m_sections.back()->createEntry(lineView.substr(0, lineView.find('=')), lineView.substr(lineView.find('=') + 1));
        }
    }
}
