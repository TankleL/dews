#pragma once
#include "dews-prereq.hpp"
#include "dewsclass.hpp"

namespace dews
{
    class DEWS_API DewsBreaker
    {
    public:
        DewsBreaker();

    public:
        void setdews(DEWS_IN Dews&& dews);
        void getdews(DEWS_OUT Dews& dews);
        

        bool unpack_int32(DEWS_OUT int32_t& value);

    public:
        bool is_eod() const;


    public:
        Dews   _dews;
        size_t _index;
    };
}
