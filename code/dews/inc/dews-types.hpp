#pragma once

#include <cstdint>

namespace dews
{
    enum DewsHeaderTypes : uint8_t
    {
        DHT_Int8 = 0x10,
        DHT_Int16 = 0x20,
        DHT_Int32 = 0x30,
        DHT_Int64 = 0x40,
        DHT_UInt8 = 0x50,
        DHT_UInt16 = 0x60,
        DHT_UInt32 = 0x70,
        DHT_UInt64 = 0x80,

        DHT_Float = 0x90,
        DHT_Double = 0xa0,

        DHT_Boolean = 0xb0,
        DHT_String = 0xc0,
        DHT_Dews = 0xd0,

        DHT_Unknown = 0xe0,
        DHT_Extended = 0xf0
    };

    enum DewsHeaderTypesExtended : uint8_t
    {
        DHTE_UInt8_Array = 0x01,

        DHTE_Extended = 0x0f
    };

    enum DewsBooleanValue : uint8_t
    {
        DBV_False = 0x00,
        DBV_True = 0x01
    };

}




