#pragma once
#include "Node.h"

class IpPrefixBitmap
{
public:
    IpPrefixBitmap();
    void add(unsigned tmpPrefix, char mask);
    void del(unsigned prefix, char mask);
    char check(unsigned ip);
private:
    unsigned llmp(const TreeNodePtr &node, unsigned d);

    TreeNodePtr root;

};



