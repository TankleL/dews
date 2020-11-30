#include "dews-api-dev-mode.hpp"
#include "dews-types.hpp"
#include "dewsbreaker.hpp"
#include "zigzag.hpp"

using namespace dews;

namespace dews
{
    namespace _internal_impls
    {
        bool dews_unpack_uint8(
            DEWS_OUT uint8_t& value,
            DEWS_OUT size_t& read, 
            DEWS_IN const Dews& dews,
            DEWS_IN DewsHeaderTypes dht,
            DEWS_IN size_t index);

        bool dews_unpack_uint16(
            DEWS_OUT uint16_t& value,
            DEWS_OUT size_t& read, 
            DEWS_IN const Dews& dews,
            DEWS_IN DewsHeaderTypes dht,
            DEWS_IN size_t index);

        bool dews_unpack_uint32(
            DEWS_OUT uint32_t& value,
            DEWS_OUT size_t& read, 
            DEWS_IN const Dews& dews,
            DEWS_IN DewsHeaderTypes dht,
            DEWS_IN size_t index);

        bool dews_unpack_uint64(
            DEWS_OUT uint64_t& value,
            DEWS_OUT size_t& read, 
            DEWS_IN const Dews& dews,
            DEWS_IN DewsHeaderTypes dht,
            DEWS_IN size_t index);
    }
}

DewsBreaker::DewsBreaker()
    : _index(0)
{}

DewsBreaker::DewsBreaker(DEWS_REF Dews&& dews)
    : _index(0)
    , _dews(std::move(dews))
{}

void DewsBreaker::setdews(DEWS_IN Dews&& dews)
{
    _index = 0;
    _dews = std::move(dews);
}

void DewsBreaker::getdews(DEWS_OUT Dews& dews)
{
    _index = 0;
    _dews.flushto(dews);
}

bool DewsBreaker::is_eod() const
{
    return _index < _dews.length();
}


bool DewsBreaker::unpack_int8(DEWS_OUT int8_t& value)
{
    size_t read = 0;
    uint8_t ui8value = 0;

    bool retval = _internal_impls::dews_unpack_uint8(ui8value, read, _dews, DHT_Int8, _index);
    value = (int8_t)ui8value;
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_int16(DEWS_OUT int16_t& value)
{
    size_t read = 0;
    uint16_t ui16value = 0;

    bool retval = _internal_impls::dews_unpack_uint16(ui16value, read, _dews, DHT_Int16, _index);
    value = zz_to_i16(ui16value);
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_int32(DEWS_OUT int32_t& value)
{
    size_t read = 0;
    uint32_t ui32value = 0;

    bool retval = _internal_impls::dews_unpack_uint32(ui32value, read, _dews, DHT_Int32, _index);
    value = zz_to_i32(ui32value);
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_int64(DEWS_OUT int64_t& value)
{
    size_t read = 0;
    uint64_t ui64value = 0;

    bool retval = _internal_impls::dews_unpack_uint64(ui64value, read, _dews, DHT_Int64, _index);
    value = zz_to_i64(ui64value);
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_uint8(DEWS_OUT uint8_t& value)
{
    size_t read = 0;

    bool retval = _internal_impls::dews_unpack_uint8(value, read, _dews, DHT_UInt8, _index);
    _index += read;

    return retval;
}

bool DewsBreaker::unpack_uint16(DEWS_OUT uint16_t& value)
{
    size_t read = 0;

    bool retval = _internal_impls::dews_unpack_uint16(value, read, _dews, DHT_UInt16, _index);
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

bool DewsBreaker::unpack_uint64(DEWS_OUT uint64_t& value)
{
    size_t read = 0;

    bool retval = _internal_impls::dews_unpack_uint64(value, read, _dews, DHT_UInt64, _index);
    _index += read;

    return retval;
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
bool DewsBreaker::unpack_string(DEWS_OUT std::string& value)
{
    bool retval = true;

    const uint8_t* cur = _dews.data(_index);
    uint8_t header = *cur;
    ++cur;  ++_index;

    int dht = header & 0xf0;
    if (dht == DHT_String)
    {
        int len = header & 0x0f;

        if (len > 14)
        {
            uint32_t real_len;
            if (unpack_uint32(real_len))
            {
                value.clear();
                value.insert(
                    value.end(),
                    (const char*)_dews.data(_index),
                    (const char*)_dews.data(_index + real_len));
                _index += real_len;
            }
            else
            {
                retval = false;
            }
        }
        else if (len > 0)
        { // len > 0 && len < 15
            value.clear();
            value.insert(value.end(), (const char*)_dews.data(_index), (const char*)_dews.data(_index + len));
            _index += len;
        }
        else
        { // len == 0
            value.clear();
        }
    }
    else
    {
        retval = false;
    }

    return retval;
}


bool DewsBreaker::unpack_uint8_array(DEWS_OUT uint8_t* dst, DEWS_IN size_t length)
{
    assert(length <= std::numeric_limits<uint32_t>::max());
    bool retval = true;

    const uint8_t* cur = _dews.data(_index);
    uint8_t header = *cur;
    ++cur;  ++_index;

    if (header == (DHT_Extended | DHTE_UInt8_Array))
    {
        uint32_t len;
        retval = unpack_uint32(len);

        if (retval && len == (uint32_t)length)
        {
            memcpy(dst, _dews.data(_index), length);
            _index += length;
        }
    }
    else
    {
        retval = false;
    }

    return retval;
}

bool DewsBreaker::uint8_array_length(DEWS_OUT size_t& length) const
{
    bool retval = true;
    uint8_t header = *_dews.data(_index);

    if (header == (DHT_Extended | DHTE_UInt8_Array))
    {
        uint32_t len;
        size_t read;
        retval = _internal_impls::dews_unpack_uint32(len, read, _dews, DHT_UInt32, _index + 1);

        if (retval)
        {
            length = len;
        }
    }
    else
    {
        retval = false;
    }
   
    return retval;
}

bool DewsBreaker::unpack_dews(DEWS_OUT Dews& value)
{
    bool retval = true;

    const uint8_t* cur = _dews.data(_index);
    uint8_t header = *cur;
    ++cur;  ++_index;

    if (header == DHT_Dews)
    {
        size_t reallen;
        retval = uint8_array_length(reallen);

        if (retval)
        {
            value.resize(reallen);
            retval = unpack_uint8_array(value.data(), reallen);
        }
    }
    else
    {
        retval = false;
    }

    return retval;
}


/**
 unpack an 8-bit integer from buffer
 dew format:
 -----------------  -------------
 |  Pack  Header |  |   Value   |
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    |
 -----------------  -------------
 */
bool dews::_internal_impls::dews_unpack_uint8(
    DEWS_OUT uint8_t& value,
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
        
        switch (len)
        {
        case 0:
            value = 0;
            break;

        case 1:
            value = *cur;
            ++cur; ++index;
            break;

        default:
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

/**
 unpack a 16-bit integer from buffer
 dew format:
 -----------------  ------------- -------------
 |  Pack  Header |  |H  Value1  | |   Value2  |
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    |
 -----------------  ------------- -------------
 */
bool dews::_internal_impls::dews_unpack_uint16(
    DEWS_OUT uint16_t& value,
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
        
        switch (len)
        {
        case 0:
            value = 0;
            break;

        case 1:
            value = (uint16_t)*cur;
            ++cur; ++index;
            break;

        case 2:
            value = ((uint16_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint16_t)* cur;
            ++cur; ++index;
            break;

        default:
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

/**
 unpack a 32-bit integer from buffer
 dew format:
 -----------------  ------------- ------------- ------------- -------------
 |  Pack  Header |  |H  Value1  | |   Value2  | |   Value3  | |  Value4  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   | |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    | |    VAL    | |    VAL    |
 -----------------  ------------- ------------- ------------- -------------
 */
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
        
        switch(len)
        {
        case 0:
            value = 0;
            break;

        case 1:
            value = (uint32_t)*cur;
            ++cur; ++index;
            break;

        case 2:
            value = ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)* cur;
            ++cur; ++index;
            break;

        case 3:
            value = ((uint32_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)*cur;
            ++cur; ++index;
            break;

        case 4:
            value = ((uint32_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint32_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint32_t)*cur;
            ++cur; ++index;
            break;

        default:
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

/**
 unpack a 32-bit integer from buffer
 dew format:
 -----------------  ------------- -------------     -------------
 |  Pack  Header |  |H  Value1  | |   Value2  |     |  Value4  L|
 | 0 ~ 3 | 4 ~ 7 |  |   0 ~ 7   | |   0 ~ 7   | ... |   0 ~ 7   |
 |  DHT  |  LEN  |  |    VAL    | |    VAL    |     |    VAL    |
 -----------------  ------------- -------------     -------------
 */
bool dews::_internal_impls::dews_unpack_uint64(
    DEWS_OUT uint64_t& value,
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
        
        switch (len)
        {
        case 0:
            value = 0;
            break;

        case 1:
            value = (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 2:
            value = ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)* cur;
            ++cur; ++index;
            break;

        case 3:
            value = ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 4:
            value = ((uint64_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 5:
            value = ((uint64_t)*cur) << 32;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 6:
            value = ((uint64_t)*cur) << 40;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 32;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 7:
            value = ((uint64_t)*cur) << 48;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 40;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 32;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        case 8:
            value = ((uint64_t)*cur) << 56;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 48;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 40;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 32;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 24;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 16;
            ++cur; ++index;

            value |= ((uint64_t)*cur) << 8;
            ++cur; ++index;

            value |= (uint64_t)*cur;
            ++cur; ++index;
            break;

        default:
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



