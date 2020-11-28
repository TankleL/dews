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
        void dews_pack_uint8(DEWS_REF Dews& dews, DEWS_IN uint8_t value, DEWS_IN DewsHeaderTypes dht);
        void dews_pack_uint16(DEWS_REF Dews& dews, DEWS_IN uint16_t value, DEWS_IN DewsHeaderTypes dht);
        void dews_pack_uint32(DEWS_REF Dews& dews, DEWS_IN uint32_t value, DEWS_IN DewsHeaderTypes dht);
        void dews_pack_uint64(DEWS_REF Dews& dews, DEWS_IN uint64_t value, DEWS_IN DewsHeaderTypes dht);
    }
}

DewsBuilder::DewsBuilder()
    : _pks(PKS_Idle)
{}

DewsBuilder::~DewsBuilder()
{}

DewsBuilder& DewsBuilder::pack_int8(DEWS_IN int8_t value)
{
    _internal_impls::dews_pack_uint8(_dews, (uint8_t)value, DHT_Int8);
    return *this;
}

DewsBuilder& DewsBuilder::pack_int16(DEWS_IN int16_t value)
{
    _internal_impls::dews_pack_uint16(_dews, i16_to_zz(value), DHT_Int16);
    return *this;
}

DewsBuilder& DewsBuilder::pack_int32(DEWS_IN int32_t value)
{
    _internal_impls::dews_pack_uint32(_dews, i32_to_zz(value), DHT_Int32);
    return *this;
}

DewsBuilder& DewsBuilder::pack_int64(DEWS_IN int64_t value)
{
    _internal_impls::dews_pack_uint64(_dews, i64_to_zz(value), DHT_Int64);
    return *this;
}

DewsBuilder& DewsBuilder::pack_uint8(DEWS_IN uint8_t value)
{
    _internal_impls::dews_pack_uint8(_dews, value, DHT_UInt8);
    return *this;
}

DewsBuilder& DewsBuilder::pack_uint16(DEWS_IN uint16_t value)
{
    _internal_impls::dews_pack_uint16(_dews, value, DHT_UInt16);
    return *this;
}

DewsBuilder& DewsBuilder::pack_uint32(DEWS_IN uint32_t value)
{
    _internal_impls::dews_pack_uint32(_dews, value, DHT_UInt32);
    return *this;
}

DewsBuilder& DewsBuilder::pack_uint64(DEWS_IN uint64_t value)
{
    _internal_impls::dews_pack_uint64(_dews, value, DHT_UInt64);
    return *this;
}

/**
 put a string into buffer
 dew format:
           -----------------  ------------- -------------     -------------
           |  Pack  Header |  |   Char1   | |   Char2   |     |   CharN   |
 LEN < 15  | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | ... |   0 ~ 7   |
           |  DHT  |  LEN  |  |    VAL    | |    VAL    |     |    VAL    |
           -----------------  ------------- -------------     -------------
 or 
           ----------------- ---------------------- ------------- -------------     -------------
           |  Pack  Header | |  LEN : Dew-UInt32  | |   Char1   | |   Char2   |     |   CharN   |
 LEN >=15  | 0 ~ 3 | 4 ~ 7 | |     0 ~ Variable   | |   0 ~ 7   | |   0 ~ 7   | ... |   0 ~ 7   |
           |  DHT  |  LEN  | |        VAL         | |    VAL    | |    VAL    |     |    VAL    |
           ----------------- ---------------------- ------------- -------------     -------------
 */
DewsBuilder& DewsBuilder::pack_string(DEWS_IN const std::string& value)
{
    const size_t LEN = value.length();
    assert(LEN <= std::numeric_limits<uint32_t>::max());

    if (LEN > 14)
    { // len >= 15
        _dews.push(DHT_String | 0x0f);
        pack_uint32((uint32_t)LEN); 
        _dews.push((const uint8_t*)value.data(), (const uint8_t*)value.data() + LEN);
    }
    else if (LEN > 0)
    { // len < 15 && len > 0
        _dews.push(DHT_String | (uint8_t)LEN);
        _dews.push((const uint8_t*)value.data(), (const uint8_t*)value.data() + LEN);
    }
    else
    { // len == 0
        _dews.push(DHT_String); // DHT_String | 0x00
    }

    return *this;
}

bool DewsBuilder::getdews(DEWS_OUT Dews& dews)
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

/**
 put an 8-bit integer into buffer
 dew format:
 -----------------  -------------
 |  Pack  Header |  |   Value   |
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    |
 -----------------  -------------
 */
void dews::_internal_impls::dews_pack_uint8(DEWS_REF Dews& dews, DEWS_IN uint8_t value, DEWS_IN DewsHeaderTypes dht)
{
    if (value > 0)
    { // LEN == 1
        uint8_t header = dht | 0x01;
        dews.push(header);
        dews.push(value);
    }
    else
    { // LEN == 0
        uint8_t header = dht; // dht | 0x00
        dews.push(header);
    }
}

/**
 put a 16-bit integer into buffer
 dew format:
 -----------------  ------------- -------------
 |  Pack  Header |  |H  Value1  | |  Value2  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    |
 -----------------  ------------- -------------
 */
void dews::_internal_impls::dews_pack_uint16(DEWS_REF Dews& dews, DEWS_IN uint16_t value, DEWS_IN DewsHeaderTypes dht)
{
    if (value == 0)
    { // LEN == 0
        uint8_t header = dht; // dht | 0x00
        dews.push(header);
    }
    else if (value < 0x100)
    { // LEN == 1
    uint8_t header = dht | 0x01;
    dews.push(header);
    dews.push((uint8_t)value);
    }
    else
    { // LEN == 2
    uint8_t header = dht | 0x02;
    dews.push(header);
    dews.push((uint8_t)(value >> 8));
    dews.push((uint8_t)(value & 0xff));
    }
}


/**
 put a 32-bit integer into buffer
 dew format:
 -----------------  ------------- ------------- ------------- -------------
 |  Pack  Header |  |H  Value1  | |   Value2  | |   Value3  | |  Value4  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    | |    VAL    | |    VAL    |
 -----------------  ------------- ------------- ------------- -------------
 */
void dews::_internal_impls::dews_pack_uint32(DEWS_REF Dews& dews, DEWS_IN uint32_t value, DEWS_IN DewsHeaderTypes dht)
{
    if (value == 0)
    { // LEN == 0
        uint8_t header = dht; // dht | 0x00
        dews.push(header);
    }
    else if (value < 0x100)
    { // LEN == 1
        uint8_t header = dht | 0x01;
        dews.push(header);
        dews.push((uint8_t)value);
    }
    else if (value < 0x10000)
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
        dews.push((uint8_t)(value >> 16));
        dews.push((uint8_t)((value & 0xff00) >> 8));
        dews.push((uint8_t)(value & 0x00ff));
    }
    else
    { // LEN == 4
        uint8_t header = dht | 0x04;
        dews.push(header);
        dews.push((uint8_t)(value >> 24));
        dews.push((uint8_t)((value & 0xff0000) >> 16));
        dews.push((uint8_t)((value & 0x00ff00) >> 8));
        dews.push((uint8_t)(value & 0x0000ff));
    }
}

/**
 put a 64-bit integer into buffer
 dew format:
 -----------------  ------------- -------------     -------------
 |  Pack  Header |  |H  Value1  | |   Value2  |     |  Value8  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | ... |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    |     |    VAL    |
 -----------------  ------------- -------------     -------------
 */
void dews::_internal_impls::dews_pack_uint64(DEWS_REF Dews & dews, DEWS_IN uint64_t value, DEWS_IN DewsHeaderTypes dht)
{
    if (value == 0)
    { // LEN == 0
        uint8_t header = dht; // dht | 0x00
        dews.push(header);
    }
    else if (value < 0x100)
    { // LEN == 1
        uint8_t header = dht | 0x01;
        dews.push(header);
        dews.push((uint8_t)value);
    }
    else if (value < 0x10000)
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
        dews.push((uint8_t)(value >> 16));
        dews.push((uint8_t)((value & 0xff00) >> 8));
        dews.push((uint8_t)(value &  0x00ff));
    }
    else if (value < 0x100000000)
    { // LEN == 4
        uint8_t header = dht | 0x04;
        dews.push(header);
        dews.push((uint8_t)(value >> 24));
        dews.push((uint8_t)((value & 0xff0000) >> 16));
        dews.push((uint8_t)((value & 0x00ff00) >> 8));
        dews.push((uint8_t)(value &  0x0000ff));
    }
    else if (value < 0x10000000000)
    { // LEN == 5
        uint8_t header = dht | 0x05;
        dews.push(header);
        dews.push((uint8_t)(value >> 32));
        dews.push((uint8_t)((value & 0xff000000) >> 24));
        dews.push((uint8_t)((value & 0x00ff0000) >> 16));
        dews.push((uint8_t)((value & 0x0000ff00) >> 8));
        dews.push((uint8_t)(value &  0x000000ff));
    }
    else if (value < 0x1000000000000)
    { // LEN == 6
        uint8_t header = dht | 0x06;
        dews.push(header);
        dews.push((uint8_t)(value >> 40));
        dews.push((uint8_t)((value & 0xff00000000) >> 32));
        dews.push((uint8_t)((value & 0x00ff000000) >> 24));
        dews.push((uint8_t)((value & 0x0000ff0000) >> 16));
        dews.push((uint8_t)((value & 0x000000ff00) >> 8));
        dews.push((uint8_t)(value &  0x00000000ff));
    }
    else if (value < 0x100000000000000)
    { // LEN == 7
        uint8_t header = dht | 0x07;
        dews.push(header);
        dews.push((uint8_t)(value >> 48));
        dews.push((uint8_t)((value & 0xff0000000000) >> 40));
        dews.push((uint8_t)((value & 0x00ff00000000) >> 32));
        dews.push((uint8_t)((value & 0x0000ff000000) >> 24));
        dews.push((uint8_t)((value & 0x000000ff0000) >> 16));
        dews.push((uint8_t)((value & 0x00000000ff00) >> 8));
        dews.push((uint8_t)(value &  0x0000000000ff));
    }
    else
    { // LEN == 8
        uint8_t header = dht | 0x08;
        dews.push(header);
        dews.push((uint8_t)(value >> 56));
        dews.push((uint8_t)((value & 0xff000000000000) >> 48));
        dews.push((uint8_t)((value & 0x00ff0000000000) >> 40));
        dews.push((uint8_t)((value & 0x0000ff00000000) >> 32));
        dews.push((uint8_t)((value & 0x000000ff000000) >> 24));
        dews.push((uint8_t)((value & 0x00000000ff0000) >> 16));
        dews.push((uint8_t)((value & 0x0000000000ff00) >> 8));
        dews.push((uint8_t)(value &  0x000000000000ff));
    }
}


