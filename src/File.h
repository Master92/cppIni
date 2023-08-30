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
