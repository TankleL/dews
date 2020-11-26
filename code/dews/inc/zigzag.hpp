#include "dews-prereq.hpp"

namespace dews
{

    inline uint8_t i8_to_zz(int8_t value)
    {
        return (value << 1) ^ (value >> 7);
    }

    inline uint16_t i16_to_zz(int16_t value)
    {
        return (value << 1) ^ (value >> 15);
    }

    inline uint32_t i32_to_zz(int32_t value)
    {
        return (value << 1) ^ (value >> 31);
    }

    inline uint64_t i64_to_zz(int64_t value)
    {
        return (value << 1) ^ (value >> 63);
    }

    inline int32_t zz_to_i32(uint32_t value)
    {
        return (int32_t)((value >> 1) ^ -(int32_t)(value & 1));
    }

}


