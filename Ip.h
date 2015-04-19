#pragma once

#include "estd.h"

class Ip
{
public:
    Ip(unsigned prefix, char mask) : prefix(prefix), mask(mask){}
    unsigned prefix;
    char mask;
    Ip& operator <<(unsigned i)
    {
        prefix <<= i;
        mask -= i;
        return *this;
    }
    void operator <<=(unsigned i)
    {
        *this << i;
    }

    char bit_len()
    {
        return mask;
    }

    inline unsigned ibm_idx()
    {
        return bit_len() ? (1u << bit_len()) + estd::bits_value(prefix, bit_len()) : 0;
    }

};
