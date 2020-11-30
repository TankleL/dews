#pragma once
#include "dews-prereq.hpp"
#include "dewsclass.hpp"

namespace dews
{
    class DEWS_API DewsBreaker
    {
    public:
        DewsBreaker();
        DewsBreaker(DEWS_REF Dews&& dews);

    public:
        void setdews(DEWS_IN Dews&& dews);
        void getdews(DEWS_OUT Dews& dews);

        bool unpack_int8(DEWS_OUT int8_t& value);
        bool unpack_int16(DEWS_OUT int16_t& value);
        bool unpack_int32(DEWS_OUT int32_t& value);
        bool unpack_int64(DEWS_OUT int64_t& value);

        bool unpack_uint8(DEWS_OUT uint8_t& value);
        bool unpack_uint16(DEWS_OUT uint16_t& value);
        bool unpack_uint32(DEWS_OUT uint32_t& value);
        bool unpack_uint64(DEWS_OUT uint64_t& value);

        bool unpack_string(DEWS_OUT std::string& value);

        bool unpack_uint8_array(DEWS_OUT uint8_t* dst, DEWS_IN size_t length);

        bool unpack_dews(DEWS_OUT Dews& value);

    public:
        bool uint8_array_length(DEWS_OUT size_t& length) const;

    public:
        bool is_eod() const;

    public:
        Dews   _dews;
        size_t _index;
    };
}
