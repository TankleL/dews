#pragma once
#include "dews-prereq.hpp"

namespace dews
{
    class DEWS_API Dews
    {
    public:
        Dews();
        Dews(const Dews& rhs);
        Dews(Dews&& rhs) noexcept;
        Dews(std::vector<uint8_t>&& rhs) noexcept;
        Dews(size_t length);
        ~Dews();

    public:
        Dews& operator=(const Dews& rhs);
        Dews& operator=(Dews&& rhs) noexcept;

    public:
        void push(uint8_t value);
        void push(const uint8_t* begin, const uint8_t* end);
        const uint8_t* data() const;
        const uint8_t* data(size_t index) const;
        uint8_t* data();
        uint8_t* data(size_t index);
        const size_t length() const;

    public:
        void reset();
        void resize(size_t length);
        void acquire(Dews& source);
        void flushto(Dews& dest);
        void flushto(std::vector<uint8_t>& dest);

    private:
        DEWS_HIDDEN_TYPE _buffer;
    };
}


