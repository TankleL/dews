#include <cassert>
#include "dews.hpp"

using namespace dews;

void test_int32_encode_decode();
void test_uint32_encode_decode();

int main(int argc, char** argv)
{
    test_int32_encode_decode();
    test_uint32_encode_decode();
    return 0;
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




