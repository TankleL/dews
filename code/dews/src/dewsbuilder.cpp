#include "dews-api-dev-mode.hpp"
#include "dews-prereq.hpp"
#include "dews-types.hpp"
#include "dewsbuilder.hpp"
#include "zigzag.hpp"

using namespace dews;

namespace dews
{
    namespace _internal_impls
    {
        void dews_pack_uint32(DEWS_REF Dews& dews, DEWS_IN uint32_t value, DEWS_IN DewsHeaderTypes dht);
    }
}

DewsBuilder::DewsBuilder()
    : _pks(PKS_Idle)
{}

DewsBuilder::~DewsBuilder()
{}

/**
 put an 32-bit integer into buffer
 dew format:
 -----------------  ------------- ------------- ------------- -------------
 |  Pack  Header |  |H  Value1  | |   Value2  | |   Value3  | |  Value4  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    | |    VAL    | |    VAL    |
 -----------------  ------------- ------------- ------------- -------------
 */
DewsBuilder& DewsBuilder::pack_int32(DEWS_IN int32_t value)
{
    _internal_impls::dews_pack_uint32(_dews, i32_to_zz(value), DHT_Int32);
    return *this;
}

DewsBuilder& DewsBuilder::pack_uint32(DEWS_IN uint32_t value)
{
    _internal_impls::dews_pack_uint32(_dews, value, DHT_UInt32);
    return *this;
}

bool DewsBuilder::get_dews(DEWS_OUT Dews& dews)
{
    bool retval = true;

    if (_pks == PKS_Idle)
    {
        _dews.flushto(dews);
    }
    else
    {
        retval = false;
    }

    return retval;
}

/**********************************************************
 Internal Implemetations
 *********************************************************/

void dews::_internal_impls::dews_pack_uint32(DEWS_REF Dews& dews, DEWS_IN uint32_t value, DEWS_IN DewsHeaderTypes dht)
{
    if (value < 0x100)
    { // LEN == 1
        uint8_t header = dht | 0x01;
        dews.push(header);
        dews.push((uint8_t)value);
    }
    else if(value < 0x10000)
    { // LEN == 2
        uint8_t header = dht | 0x02;
        dews.push(header);
        dews.push((uint8_t)(value >> 8));
        dews.push((uint8_t)(value & 0xff));
    }
    else if (value < 0x1000000)
    { // LEN == 3
        uint8_t header = dht | 0x03;
        dews.push(header);
        dews.push((uint8_t) (value >> 16));
        dews.push((uint8_t)((value & 0xff00) >> 8));
        dews.push((uint8_t) (value & 0x00ff));
    }
    else
    { // LEN == 4
        uint8_t header = dht | 0x04;
        dews.push(header);
        dews.push((uint8_t) (value >> 24));
        dews.push((uint8_t)((value & 0xff0000) >> 16));
        dews.push((uint8_t)((value & 0x00ff00) >> 8));
        dews.push((uint8_t) (value & 0x0000ff));
    }
}



