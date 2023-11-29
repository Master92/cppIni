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

#include <any>

#include <cppIni/cppini_export.h>

/// A variant type that can hold any type
class CPPINI_EXPORT Variant {
public:
    constexpr Variant() = default; //< Default constructor
    template<class T> explicit Variant(T value) : m_value{value} {} //< Construct a variant from a value
    Variant(const Variant& other) = default; //< Copy constructor

    /// Get the value of the variant
    /// \return The value of the variant
    /// \throws std::bad_any_cast if the variant does not hold the requested type
    /// \tparam T The type of the value to get
    template<class T> auto get() const -> const T&
    {
        return std::any_cast<const T&>(m_value);
    }

    /// Check if the variant holds a value of the specified type
    /// \return true if the variant holds a value of the specified type, false otherwise
    /// \tparam T The type to check for
    /// \note If the variant is empty (i.e. holds no value), this function will return false
    template<class T> auto is() const -> bool
    {
        return m_value.has_value() and m_value.type() == typeid(T);
    }

    /// Check if the variant holds a value of the specified type
    /// \return true if the variant holds a value of the specified type, false otherwise
    /// \param type The type_info to check for
    auto is(const std::type_info& type) const -> bool
    {
        return m_value.has_value() and m_value.type() == type;
    }

    /// Convert the variant to the specified type
    /// \return The value of the variant converted to the specified type
    /// \throws std::bad_any_cast if the variant does not hold the requested type
    /// \tparam T The type to convert to
    template<class T> explicit operator T() const
    {
        return get<T>();
    }

    template<class T> auto operator==(const Variant& other) const -> bool; ///< Templated equality operator
    template<class T> auto operator!=(const Variant& other) const -> bool { return !((*this).operator==<T>(other)); } ///< Templated inequality operator

private:
    std::any m_value;
};

template<class T>
auto Variant::operator==(const Variant& other) const -> bool
{
    if (not is(other.m_value.type())) {
        return false;
    }

    return get<T>() == other.get<T>();
}
