#include "dews-api-dev-mode.hpp"
#include "dews-types.hpp"
#include "dewsbreaker.hpp"
#include "zigzag.hpp"

using namespace dews;

namespace dews
{
    namespace _internal_impls
    {
        bool dews_unpack_uint32(
            DEWS_OUT uint32_t& value,
            DEWS_OUT size_t& read, 
            DEWS_IN const Dews& dews,
            DEWS_IN DewsHeaderTypes dht,
            DEWS_IN size_t index);
    }
}

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

bool DewsBreaker::is_eod() const
{
    return _index < _dews.length();
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
    size_t read = 0;
    uint32_t ui32value = 0;

    bool retval = _internal_impls::dews_unpack_uint32(ui32value, read, _dews, DHT_Int32, _index);
    value = zz_to_i32(ui32value);
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_uint32(DEWS_OUT uint32_t& value)
{
    size_t read = 0;

    bool retval = _internal_impls::dews_unpack_uint32(value, read, _dews, DHT_UInt32, _index);
    _index += read;

    return retval;
}


bool dews::_internal_impls::dews_unpack_uint32(
    DEWS_OUT uint32_t& value,
    DEWS_OUT size_t& read,
    DEWS_IN const Dews& dews,
    DEWS_IN DewsHeaderTypes dht,
    DEWS_IN size_t index)
{
    bool retval = true;
    const size_t origin_index = index;
    const uint8_t* cur = dews.data(index);
    uint8_t header = *cur;
    ++cur;  ++index;

    int thedht = header & 0xf0;
    if(thedht == dht)
    {
        int len = header & 0x0f;
        
        if (len == 1)
        {
            value = (uint32_t)*cur;
            ++cur; ++index;
        }
        else if(len == 2)
        {
            value = ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)* cur;
            ++cur; ++index;
        }
        else if (len == 3)
        {
            value = ((uint32_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)*cur;
            ++cur; ++index;
        }
        else if (len == 4)
        {
            value = ((uint32_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)*cur;
            ++cur; ++index;
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

    read = index - origin_index;
    return retval;
}


