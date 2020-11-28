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
        ~DewsBuilder();

    public:
        DewsBuilder& pack_int32(DEWS_IN int32_t value);
        DewsBuilder& pack_uint32(DEWS_IN uint32_t value);
        DewsBuilder& pack_array_begin();
        DewsBuilder& pack_array_end();

        bool getdews(DEWS_OUT Dews& dews);

    private:
        Dews            _dews;
        PackingStatus   _pks;
    };
}


