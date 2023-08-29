#include "Section.h"

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

auto Section::fqTitle() const -> std::string
{
    if (m_parent == nullptr) {
        return m_title;
    }

    return m_parent->fqTitle() + "." + m_title;
}

auto Section::findEntry(std::string_view name) const -> const Entry*
{
    const auto entry = m_entries.find(std::string(name));

    if (entry != m_entries.cend()) {
        return &entry->second;
    }

    return nullptr;
}
