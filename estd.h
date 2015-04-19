#pragma once

namespace estd
{
    inline unsigned bits_value(unsigned value, unsigned bits)
    {
        return value >> (32 - bits);
    }
}
