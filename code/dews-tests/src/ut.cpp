#include <cassert>
#include "dews.hpp"

using namespace dews;

void test_int8_encode_decode();
void test_int16_encode_decode();
void test_int32_encode_decode();
void test_int64_encode_decode();

void test_uint8_encode_decode();
void test_uint16_encode_decode();
void test_uint32_encode_decode();
void test_uint64_encode_decode();

void test_string_encode_decode();

void test_dews_apis();

int main(int argc, char** argv)
{
    test_int8_encode_decode();
    test_int16_encode_decode();
    test_int32_encode_decode();
    test_int64_encode_decode();

    test_uint8_encode_decode();
    test_uint16_encode_decode();
    test_uint32_encode_decode();
    test_uint64_encode_decode();

    test_string_encode_decode();

    test_dews_apis();

    return 0;
}

void test_int8_encode_decode()
{
    DewsBuilder db;

    db.pack_int8(0);
    db.pack_int8(1);
    db.pack_int8(-1);
    db.pack_int8(8);
    db.pack_int8(-8);
    db.pack_int8(127);
    db.pack_int8(-126);

    Dews ds; db.getdews(ds);
    DewsBreaker dr; dr.setdews(std::move(ds));

    int8_t val;
    assert(dr.unpack_int8(val));
    assert(val == 0);

    assert(dr.unpack_int8(val));
    assert(val == 1);

    assert(dr.unpack_int8(val));
    assert(val == -1);

    assert(dr.unpack_int8(val));
    assert(val == 8);

    assert(dr.unpack_int8(val));
    assert(val == -8);

    assert(dr.unpack_int8(val));
    assert(val == 127);

    assert(dr.unpack_int8(val));
    assert(val == -126);
}

void test_int16_encode_decode()
{
    DewsBuilder db;

    db.pack_int16(0);
    db.pack_int16(1);
    db.pack_int16(-1);
    db.pack_int16(8);
    db.pack_int16(-8);
    db.pack_int16(128);
    db.pack_int16(-128);
    db.pack_int16(4096);
    db.pack_int16(-4096);
    db.pack_int16(32767);
    db.pack_int16(-32765);
    db.pack_int16(-12345);

    Dews ds; db.getdews(ds);

    DewsBreaker dr; dr.setdews(std::move(ds));

    int16_t val;
    assert(dr.unpack_int16(val));
    assert(val == 0);

    assert(dr.unpack_int16(val));
    assert(val == 1);

    assert(dr.unpack_int16(val));
    assert(val == -1);

    assert(dr.unpack_int16(val));
    assert(val == 8);

    assert(dr.unpack_int16(val));
    assert(val == -8);

    assert(dr.unpack_int16(val));
    assert(val == 128);

    assert(dr.unpack_int16(val));
    assert(val == -128);

    assert(dr.unpack_int16(val));
    assert(val == 4096);

    assert(dr.unpack_int16(val));
    assert(val == -4096);

    assert(dr.unpack_int16(val));
    assert(val == 32767);

    assert(dr.unpack_int16(val));
    assert(val == -32765);

    assert(dr.unpack_int16(val));
    assert(val == -12345);
}

void test_int32_encode_decode()
{
    DewsBuilder db;

    db.pack_int32(0);
    db.pack_int32(1);
    db.pack_int32(-1);
    db.pack_int32(8);
    db.pack_int32(-8);
    db.pack_int32(128);
    db.pack_int32(4096);
    db.pack_int32(-4096);
    db.pack_int32(65536);
    db.pack_int32(-12345);
    db.pack_int32(2147483646);
    db.pack_int32(-2147287038);

    Dews ds;
    db.getdews(ds);

    DewsBreaker dr;
    dr.setdews(std::move(ds));

    int32_t val;
    assert(dr.unpack_int32(val));
    assert(val == 0);

    assert(dr.unpack_int32(val));
    assert(val == 1);

    assert(dr.unpack_int32(val));
    assert(val == -1);

    assert(dr.unpack_int32(val));
    assert(val == 8);

    assert(dr.unpack_int32(val));
    assert(val == -8);

    assert(dr.unpack_int32(val));
    assert(val == 128);

    assert(dr.unpack_int32(val));
    assert(val == 4096);

    assert(dr.unpack_int32(val));
    assert(val == -4096);

    assert(dr.unpack_int32(val));
    assert(val == 65536);

    assert(dr.unpack_int32(val));
    assert(val == -12345);

    assert(dr.unpack_int32(val));
    assert(val == 2147483646);

    assert(dr.unpack_int32(val));
    assert(val == -2147287038);
}

void test_int64_encode_decode()
{
    DewsBuilder db;

    db.pack_int64(0);
    db.pack_int64(1);
    db.pack_int64(-1);
    db.pack_int64(65536);
    db.pack_int64(-65536);
    db.pack_int64(1234567);
    db.pack_int64(-1234567);
    db.pack_int64(4294967296);
    db.pack_int64(-4294967296);
    db.pack_int64(1099511627776);
    db.pack_int64(-1099511627776);
    db.pack_int64(1152921504606846976);
    db.pack_int64(-1152921504606846976);
    db.pack_int64(9223372036854775807);
    db.pack_int64(-9223372036854775806);

    Dews ds; db.getdews(ds);
    DewsBreaker dr; dr.setdews(std::move(ds));

    int64_t val;
    assert(dr.unpack_int64(val));
    assert(val == 0);

    assert(dr.unpack_int64(val));
    assert(val == 1);

    assert(dr.unpack_int64(val));
    assert(val == -1);

    assert(dr.unpack_int64(val));
    assert(val == 65536);

    assert(dr.unpack_int64(val));
    assert(val == -65536);

    assert(dr.unpack_int64(val));
    assert(val == 1234567);

    assert(dr.unpack_int64(val));
    assert(val == -1234567);

    assert(dr.unpack_int64(val));
    assert(val == 4294967296);

    assert(dr.unpack_int64(val));
    assert(val == -4294967296);

    assert(dr.unpack_int64(val));
    assert(val == 1099511627776);

    assert(dr.unpack_int64(val));
    assert(val == -1099511627776);

    assert(dr.unpack_int64(val));
    assert(val == 1152921504606846976);

    assert(dr.unpack_int64(val));
    assert(val == -1152921504606846976);

    assert(dr.unpack_int64(val));
    assert(val == 9223372036854775807);

    assert(dr.unpack_int64(val));
    assert(val == -9223372036854775806);
}

void test_uint8_encode_decode()
{
    DewsBuilder db;

    db.pack_uint8(0);
    db.pack_uint8(1);
    db.pack_uint8(8);
    db.pack_uint8(127);
    db.pack_uint8(230);
    db.pack_uint8(255);

    Dews ds; db.getdews(ds);
    DewsBreaker dr; dr.setdews(std::move(ds));

    uint8_t val;
    assert(dr.unpack_uint8(val));
    assert(val == 0);

    assert(dr.unpack_uint8(val));
    assert(val == 1);

    assert(dr.unpack_uint8(val));
    assert(val == 8);

    assert(dr.unpack_uint8(val));
    assert(val == 127);

    assert(dr.unpack_uint8(val));
    assert(val == 230);

    assert(dr.unpack_uint8(val));
    assert(val == 255);
}

void test_uint16_encode_decode()
{
    DewsBuilder db;

    db.pack_uint16(0);
    db.pack_uint16(1);
    db.pack_uint16(8);
    db.pack_uint16(128);
    db.pack_uint16(4096);
    db.pack_uint16(32767);
    db.pack_uint16(50000);
    db.pack_uint16(65535);

    Dews ds; db.getdews(ds);

    DewsBreaker dr; dr.setdews(std::move(ds));

    uint16_t val;
    assert(dr.unpack_uint16(val));
    assert(val == 0);

    assert(dr.unpack_uint16(val));
    assert(val == 1);

    assert(dr.unpack_uint16(val));
    assert(val == 8);

    assert(dr.unpack_uint16(val));
    assert(val == 128);

    assert(dr.unpack_uint16(val));
    assert(val == 4096);

    assert(dr.unpack_uint16(val));
    assert(val == 32767);

    assert(dr.unpack_uint16(val));
    assert(val == 50000);

    assert(dr.unpack_uint16(val));
    assert(val == 65535);
}

void test_uint32_encode_decode()
{
    DewsBuilder db;

    db.pack_uint32(0);
    db.pack_uint32(1);
    db.pack_uint32((uint32_t)-1);
    db.pack_uint32(8);
    db.pack_uint32(128);
    db.pack_uint32(4096);
    db.pack_uint32(65536);
    db.pack_uint32(123456);
    db.pack_uint32(35915844);

    Dews ds;
    db.getdews(ds);

    DewsBreaker dr;
    dr.setdews(std::move(ds));

    uint32_t value = 0;
    assert(dr.unpack_uint32(value));
    assert(value == 0);

    assert(dr.unpack_uint32(value));
    assert(value == 1);

    assert(dr.unpack_uint32(value));
    assert(value == (uint32_t)-1);

    assert(dr.unpack_uint32(value));
    assert(value == 8);

    assert(dr.unpack_uint32(value));
    assert(value == 128);

    assert(dr.unpack_uint32(value));
    assert(value == 4096);
    
    assert(dr.unpack_uint32(value));
    assert(value == 65536);

    assert(dr.unpack_uint32(value));
    assert(value == 123456);

    assert(dr.unpack_uint32(value));
    assert(value == 35915844);
}

void test_uint64_encode_decode()
{
    DewsBuilder db;

    db.pack_uint64(0);
    db.pack_uint64(1);
    db.pack_uint64(65536);
    db.pack_uint64(1234567);
    db.pack_uint64(4294967296);
    db.pack_uint64(1099511627776);
    db.pack_uint64(1152921504606846976);
    db.pack_uint64(9223372036854775807);
    db.pack_uint64(18446744073709551615);

    Dews ds; db.getdews(ds);
    DewsBreaker dr; dr.setdews(std::move(ds));

    uint64_t val;
    assert(dr.unpack_uint64(val));
    assert(val == 0);

    assert(dr.unpack_uint64(val));
    assert(val == 1);

    assert(dr.unpack_uint64(val));
    assert(val == 65536);

    assert(dr.unpack_uint64(val));
    assert(val == 1234567);

    assert(dr.unpack_uint64(val));
    assert(val == 4294967296);

    assert(dr.unpack_uint64(val));
    assert(val == 1099511627776);

    assert(dr.unpack_uint64(val));
    assert(val == 1152921504606846976);

    assert(dr.unpack_uint64(val));
    assert(val == 9223372036854775807);

    assert(dr.unpack_uint64(val));
    assert(val == 18446744073709551615);
}

void test_string_encode_decode()
{
    DewsBuilder db;

    db.pack_string("Hello!");
    db.pack_string("This is a dews string packing test.");
    db.pack_string("");
    db.pack_string("Before me there's just an empty string get packed.");

    Dews ds; db.getdews(ds);
    DewsBreaker dr; dr.setdews(std::move(ds));

    std::string value;
    assert(dr.unpack_string(value));
    assert(value == "Hello!");

    assert(dr.unpack_string(value));
    assert(value == "This is a dews string packing test.");

    assert(dr.unpack_string(value));
    assert(value == "");

    assert(dr.unpack_string(value));
    assert(value == "Before me there's just an empty string get packed.");
}

void test_dews_apis()
{
    Dews dews1;

    DewsBuilder db(std::move(dews1));
    db.pack_int32(-10);

    Dews dews2;
    db.getdews(dews2);

    db.setdews(std::move(dews2));
    db.pack_string("hello");

    Dews dews3;
    db.getdews(dews3);


    DewsBreaker dr(std::move(dews3));

    int32_t i32val;
    assert(dr.unpack_int32(i32val));
    assert(i32val == -10);

    std::string strval;
    assert(dr.unpack_string(strval));
    assert(strval == "hello");
}


