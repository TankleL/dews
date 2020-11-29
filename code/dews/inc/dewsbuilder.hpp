#pragma once

#include "dews-prereq.hpp"
#include "dewsclass.hpp"

namespace dews
{
    class DEWS_API DewsBuilder
    {
    public:
        enum PackingStatus : int
        {
            PKS_Idle = 0,
            PKS_Array
        };

    public:
        DewsBuilder();
        DewsBuilder(Dews&& dews);
        ~DewsBuilder();

    public:
        DewsBuilder& pack_int8(DEWS_IN int8_t value);
        DewsBuilder& pack_int16(DEWS_IN int16_t value);
        DewsBuilder& pack_int32(DEWS_IN int32_t value);
        DewsBuilder& pack_int64(DEWS_IN int64_t value);

        DewsBuilder& pack_uint8(DEWS_IN uint8_t value);
        DewsBuilder& pack_uint16(DEWS_IN uint16_t value);
        DewsBuilder& pack_uint32(DEWS_IN uint32_t value);
        DewsBuilder& pack_uint64(DEWS_IN uint64_t value);

        DewsBuilder& pack_string(DEWS_IN const std::string& value);

        void setdews(DEWS_REF Dews&& dews);
        bool getdews(DEWS_OUT Dews& dews);

    private:
        Dews            _dews;
        PackingStatus   _pks;
    };
}


