#include "Entry.h"
#include "Section.h"

#include <cstring>

/// \details An Entry is a key-value pair in a configuration file
Entry::Entry(const Entry& other)
    : m_key(other.m_key)
    , m_size(other.m_size)
{
    m_data = std::malloc(m_size);
    memcpy(m_data, other.m_data, m_size);
}

Entry::~Entry()
{
    free(m_data);
}

auto Entry::operator==(const Entry& other) const -> bool
{
    if (m_key != other.m_key) {
        return false;
    }

    if (m_parent != other.m_parent) {
        return false;
    }

    if (m_data != other.m_data) {
        return std::memcmp(m_data, other.m_data, m_size) == 0;
    }

    return true;
}

auto Entry::operator=(const Entry& other) -> Entry&
{
    if (this == &other) {
        return *this;
    }

    if (this->m_size != other.m_size) {
        return *this;
    }

    m_key = other.m_key;
    memcpy(m_data, other.m_data, m_size);
    m_parent = other.m_parent;

    return *this;
}

auto Entry::operator=(Entry&& other) noexcept -> Entry&
{
    if (this == &other) {
        return *this;
    }

    if (this->m_size != other.m_size) {
        return *this;
    }

    m_key = std::move(other.m_key);
    free(m_data);
    m_data = other.m_data;
    other.m_data = nullptr;
    m_parent = other.m_parent;

    return *this;
}

auto Entry::fqKey() const -> std::string
{
    if (m_parent == nullptr) {
        return m_key;
    }

    return m_parent->fqTitle() + "." + m_key;
}
