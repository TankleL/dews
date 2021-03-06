#include "dews-api-dev-mode.hpp"
#include "dewsclass.hpp"

using namespace dews;

Dews::Dews()
    : _buffer(nullptr)
{
    _buffer = new std::vector<uint8_t>;
}

Dews::Dews(const Dews& rhs)
    : _buffer(nullptr)
{
    _buffer = new std::vector<uint8_t>;

    const std::vector<uint8_t>& otherbuf = *static_cast<std::vector<uint8_t>*>(rhs._buffer);
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    
    mybuf = otherbuf;
}

Dews::Dews(Dews&& rhs) noexcept
    : _buffer(nullptr)
{
    _buffer = rhs._buffer;
    rhs._buffer = nullptr;
}

Dews::Dews(std::vector<uint8_t>&& rhs) noexcept
    : _buffer(new std::vector<uint8_t>(std::move(rhs)))
{}

Dews::Dews(size_t length)
    : _buffer(new std::vector<uint8_t>(length))
{}

Dews::~Dews()
{
    delete _buffer;
}

Dews& Dews::operator=(const Dews& rhs)
{
    const std::vector<uint8_t>& otherbuf = *static_cast<std::vector<uint8_t>*>(rhs._buffer);
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);

    mybuf = otherbuf;

    return *this;
}

Dews& Dews::operator=(Dews&& rhs) noexcept
{
    _buffer = rhs._buffer;
    rhs._buffer = nullptr;

    return *this;
}

void Dews::push(uint8_t value)
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    mybuf.push_back(value);
}

void Dews::push(const uint8_t* begin, const uint8_t* end)
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    mybuf.insert(mybuf.end(), begin, end);
}

const uint8_t* Dews::data() const
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    return mybuf.data();
}

const uint8_t* Dews::data(size_t index) const
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    return mybuf.data() + index;
}

uint8_t* Dews::data()
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    return mybuf.data();
}

uint8_t* Dews::data(size_t index)
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    return mybuf.data() + index;
}
const size_t Dews::length() const
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    return mybuf.size();
}

void Dews::reset()
{
    delete _buffer;
    _buffer = new std::vector<uint8_t>;
}

void Dews::resize(size_t length)
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    mybuf.resize(length);
}

void Dews::acquire(Dews& source)
{
    _buffer = source._buffer;
    source._buffer = nullptr;
}

void Dews::flushto(Dews& dest)
{
    dest.acquire(*this);
    reset();
}

void Dews::flushto(std::vector<uint8_t>& dest)
{
    std::vector<uint8_t>& mybuf = *static_cast<std::vector<uint8_t>*>(_buffer);
    dest = std::move(mybuf);
    reset();
}





