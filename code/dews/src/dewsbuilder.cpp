#include "dews-api-dev-mode.hpp"
#include "dews-prereq.hpp"
#include "dews-types.hpp"
#include "dewsbuilder.hpp"
#include "zigzag.hpp"

using namespace dews;

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
    uint32_t zzed = i32_to_zz(value);

    if (zzed < 0x100)
    { // LEN == 1
        uint8_t header = dews::DHT_Int32 | 0x01;
        _dews.push(header);
        _dews.push((uint8_t)zzed);
    }
    else if(zzed < 0x10000)
    { // LEN == 2
        uint8_t header = dews::DHT_Int32 | 0x02;
        _dews.push(header);
        _dews.push((uint8_t)(zzed >> 8));
        _dews.push((uint8_t)(zzed & 0xff));
    }
    else if (zzed < 0x1000000)
    { // LEN == 3
        uint8_t header = dews::DHT_Int32 | 0x03;
        _dews.push(header);
        _dews.push((uint8_t) (zzed >> 16));
        _dews.push((uint8_t)((zzed & 0xff00) >> 8));
        _dews.push((uint8_t) (zzed & 0x00ff));
    }
    else
    { // LEN == 4
        uint8_t header = dews::DHT_Int32 | 0x04;
        _dews.push(header);
        _dews.push((uint8_t) (zzed >> 24));
        _dews.push((uint8_t)((zzed & 0xff0000) >> 16));
        _dews.push((uint8_t)((zzed & 0x00ff00) >> 8));
        _dews.push((uint8_t) (zzed & 0x0000ff));
    }

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




