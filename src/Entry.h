#pragma once

#include <string>

class Section;

/// \brief The Entry class
/// \details An entry is a key-value pair. The key is a string and the value can be any type. It has a parent Section
/// held by a (non-owning) pointer.
/// \note The value is stored as a string.
/// \note The value is copied into the Entry and not moved.
/// \note The parent Section is a pointer to the Section object that contains this Entry.
class Entry {
public:
    constexpr Entry() = default; ///< Default constructor
    virtual ~Entry() = default; ///< Destructor

    Entry(const Entry& other) = default; ///< Copy constructor

    template<class T>
    constexpr Entry(std::string_view key, T value, Section* parent = nullptr); ///< Constructor with key, value and parent Section pointer (default nullptr)

    auto key() const -> std::string_view { return m_key; } ///< Key as std::string_view

    template<class T> auto value() const -> T; ///< Value as type T
    auto data() const -> std::string_view { return m_data; } ///< Value as std::string_view
    constexpr auto parent() const -> const Section* { return m_parent; } ///< Parent Section
    auto fqKey() const -> std::string; ///< Fully qualified key (e.g. "Section1.Section2.Key")

    auto setKey(std::string_view key) -> void { m_key = key; } ///< Set the key
    template<class T> auto setData(T value) -> void; ///< Set the value

    constexpr auto operator<=>(const Entry& other) const = default; ///< Spaceship operator
    auto operator=(const Entry& other) -> Entry& = default; ///< Copy assignment operator
    auto operator=(Entry&& other) -> Entry& = default; ///< Move assignment operator

private:
    std::string m_key {};
    std::string m_data {};
    Section* m_parent {nullptr};
};

template<class T>
constexpr Entry::Entry(std::string_view key, T value, Section* parent)
    : m_key(key)
    , m_parent(parent)
{
    m_data = std::to_string(value);
}

template<>
inline Entry::Entry(std::string_view key, std::string value, Section* parent)
    : m_key(key)
    , m_data(std::move(value))
    , m_parent(parent)
{
}

template<>
inline Entry::Entry(std::string_view key, const char* value, Section* parent)
    : m_key(key)
    , m_data(std::string(value))
    , m_parent(parent)
{
}

template<class T>
auto Entry::setData(T value) -> void
{
    m_data = std::to_string(value);
}

template<>
inline auto Entry::setData(std::string value) -> void
{
    m_data = std::move(value);
}

template<> inline auto Entry::value<char>() const               -> char               { return std::stoi(m_data); }
template<> inline auto Entry::value<short>() const              -> short              { return std::stoi(m_data); }
template<> inline auto Entry::value<int>() const                -> int                { return std::stoi(m_data); }
template<> inline auto Entry::value<long>() const               -> long               { return std::stol(m_data); }
template<> inline auto Entry::value<long long>() const          -> long long          { return std::stoll(m_data); }
template<> inline auto Entry::value<unsigned char>() const      -> unsigned char      { return std::stoull(m_data); }
template<> inline auto Entry::value<unsigned short>() const     -> unsigned short     { return std::stoull(m_data); }
template<> inline auto Entry::value<unsigned int>() const       -> unsigned int       { return std::stoull(m_data); }
template<> inline auto Entry::value<unsigned long>() const      -> unsigned long      { return std::stoull(m_data); }
template<> inline auto Entry::value<unsigned long long>() const -> unsigned long long { return std::stoull(m_data); }
template<> inline auto Entry::value<float>() const              -> float              { return std::stof(m_data); }
template<> inline auto Entry::value<double>() const             -> double             { return std::stod(m_data); }
template<> inline auto Entry::value<long double>() const        -> long double        { return std::stold(m_data); }
template<> inline auto Entry::value<std::string>() const        -> std::string        { return m_data; }
template<> inline auto Entry::value<const char*>() const        -> const char*        { return data().data(); }
