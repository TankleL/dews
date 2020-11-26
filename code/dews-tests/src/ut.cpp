#include <cassert>
#include "dews.hpp"

using namespace dews;

void test_int32_encode_decode();

int main(int argc, char** argv)
{
    test_int32_encode_decode();
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
    db.get_dews(ds);

    DewsBreaker dr;
    dr.setdews(std::move(ds));

    int32_t val;
    dr.unpack_int32(val);
    assert(val == 0);

    dr.unpack_int32(val);
    assert(val == 1);

    dr.unpack_int32(val);
    assert(val == -1);

    dr.unpack_int32(val);
    assert(val == 8);

    dr.unpack_int32(val);
    assert(val == -8);

    dr.unpack_int32(val);
    assert(val == 128);

    dr.unpack_int32(val);
    assert(val == 4096);

    dr.unpack_int32(val);
    assert(val == 4096);

    dr.unpack_int32(val);
    assert(val == -4096);

    dr.unpack_int32(val);
    assert(val == 65536);

    dr.unpack_int32(val);
    assert(val == -12345);

    dr.unpack_int32(val);
    assert(val == 2147483646);

    dr.unpack_int32(val);
    assert(val == -2147287038);
}
