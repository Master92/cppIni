#pragma once

#include <string>

class Section;

/// \brief The Entry class
/// \details An entry is a key-value pair. The key is a string and the value can be any type. It has a parent Section
/// held by a (non-owning) pointer.
/// \note The value is stored in a void pointer and must be casted to the correct type.
/// \note The value is copied into the Entry and not moved.
/// \note The parent Section is a pointer to the Section object that contains this Entry.
class Entry {
public:
    constexpr Entry() = default; ///< Default constructor
    virtual ~Entry(); ///< Destructor

    Entry(const Entry& other); ///< Copy constructor

    template<class T>
    constexpr Entry(std::string_view key, T value, Section* parent = nullptr); ///< Constructor with key, value and parent Section pointer (default nullptr)

    auto key() const -> std::string_view { return m_key; } ///< Key as std::string_view

    template<class T>
    constexpr auto value() const -> const T& { return *reinterpret_cast<T*>(m_data); } ///< Value as const reference
    constexpr auto data() const -> const void* { return m_data; } ///< Pointer to the value
    constexpr auto parent() const -> const Section* { return m_parent; } ///< Parent Section
    constexpr auto size() const -> size_t { return m_size; } ///< Size in bytes
    auto fqKey() const -> std::string; ///< Fully qualified key (e.g. "Section1.Section2.Key")

    auto setKey(std::string_view key) -> void { m_key = key; } ///< Set the key
    template<class T> auto setData(T value) -> void; ///< Set the value

    auto operator==(const Entry& other) const -> bool; ///< Equality operator
    auto operator!=(const Entry& other) const -> bool { return not (*this == other); } ///< Inequality operator

    auto operator=(const Entry& other) -> Entry&; ///< Copy assignment operator
    auto operator=(Entry&& other) noexcept -> Entry&; ///< Move assignment operator

private:
    std::string m_key {};
    const size_t m_size {0};
    void* m_data {nullptr};
    Section* m_parent {nullptr};
};

template<class T>
constexpr Entry::Entry(std::string_view key, T value, Section* parent)
    : m_key(key)
    , m_size(sizeof(std::remove_all_extents_t<T>))
    , m_parent(parent)
{
    m_data = std::malloc(m_size);
    *reinterpret_cast<std::remove_cvref_t<T>*>(m_data) = std::move(value);
}

template<class T>
auto Entry::setData(T value) -> void
{
    if (m_size != sizeof(std::remove_all_extents_t<T>)) {
        return;
    }

    *reinterpret_cast<T*>(m_data) = std::move(value);
}
