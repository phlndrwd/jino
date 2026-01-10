#ifndef INCLUDE_TYPEMAP_H_
#define INCLUDE_TYPEMAP_H_

#include <cstdint>
#include <stdfloat>
#include <variant>

#include "Constants.h"

namespace jino {

using JsonValueType = std::variant<std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                                   std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                                   float, double, std::string>;

// Mapping between eDataTypes and actual types
template <std::int8_t T> struct TypeMap;
template <> struct TypeMap<jino::consts::eInt8>   { using type = std::int8_t; };
template <> struct TypeMap<jino::consts::eInt16>  { using type = std::int16_t; };
template <> struct TypeMap<jino::consts::eInt32>  { using type = std::int32_t; };
template <> struct TypeMap<jino::consts::eInt64>  { using type = std::int64_t; };
template <> struct TypeMap<jino::consts::eUInt8>  { using type = std::uint8_t; };
template <> struct TypeMap<jino::consts::eUInt16> { using type = std::uint16_t; };
template <> struct TypeMap<jino::consts::eUInt32> { using type = std::uint32_t; };
template <> struct TypeMap<jino::consts::eUInt64> { using type = std::uint64_t; };
template <> struct TypeMap<jino::consts::eFloat32>  { using type = std::float32_t; };
template <> struct TypeMap<jino::consts::eFloat64> { using type = std::float64_t; };
template <> struct TypeMap<jino::consts::eString> { using type = std::string; };

}

#endif // TYPEMAP_H
