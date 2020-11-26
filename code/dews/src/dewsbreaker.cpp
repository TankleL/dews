#include "dews-api-dev-mode.hpp"
#include "dews-types.hpp"
#include "dewsbreaker.hpp"
#include "zigzag.hpp"

using namespace dews;

DewsBreaker::DewsBreaker()
    : _index(0)
{}

void DewsBreaker::setdews(DEWS_IN Dews&& dews)
{
    _dews = std::move(dews);
}

void DewsBreaker::getdews(DEWS_OUT Dews& dews)
{
    dews = std::move(_dews);
}

/**
 unpack an 32-bit integer from buffer
 dew format:
 -----------------  ------------- ------------- ------------- -------------
 |  Pack  Header |  |H  Value1  | |   Value2  | |   Value3  | |  Value4  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    | |    VAL    | |    VAL    |
 -----------------  ------------- ------------- ------------- -------------
 */
bool DewsBreaker::unpack_int32(DEWS_OUT int32_t& value)
{
    bool retval = true;
    const uint8_t* cur = _dews.data(_index);
    uint8_t header = *cur;
    ++cur;  ++_index;

    int dht = header & 0xf0;
    if(dht == DHT_Int32)
    {
        int len = header & 0x0f;
        
        if (len == 1)
        {
            uint32_t bval = (uint32_t)*cur;
            ++cur; ++_index;

            value = zz_to_i32(bval);
        }
        else if(len == 2)
        {
            uint32_t bval = ((uint32_t)*cur) << 8;
            ++cur; ++_index;

            bval |= (uint32_t)* cur;
            ++cur; ++_index;
            
            value = zz_to_i32(bval);
        }
        else if (len == 3)
        {
            uint32_t bval = ((uint32_t)*cur) << 16;
            ++cur; ++_index;

            bval |= ((uint32_t)*cur) << 8;
            ++cur; ++_index;

            bval |= (uint32_t)*cur;
            ++cur; ++_index;

            value = zz_to_i32(bval);
        }
        else if (len == 4)
        {
            uint32_t bval = ((uint32_t)*cur) << 24;
            ++cur; ++_index;

            bval |= ((uint32_t)*cur) << 16;
            ++cur; ++_index;

            bval |= ((uint32_t)*cur) << 8;
            ++cur; ++_index;

            bval |= (uint32_t)*cur;
            ++cur; ++_index;

            value = zz_to_i32(bval);
        }
        else
        {
            retval = false;
        }
    }
    else
    {
        retval = false;
    }

    return retval;
}


