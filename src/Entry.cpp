#include "Entry.h"
#include "Section.h"

auto Entry::fqKey() const -> std::string
{
    if (m_parent == nullptr) {
        return m_key;
    }

    return m_parent->fqTitle() + "." + m_key;
}
