
#include "IpPrefixBitmap.h"
#include "Ip.h"
#include "estd.h"

#include <vector>
#include <algorithm>

IpPrefixBitmap::IpPrefixBitmap()
    : root(std::make_shared<TreeNode>())
{

}
char IpPrefixBitmap::check(unsigned ip)
{
    auto subTreeSplitSize = root->h + 1u;
    auto node = root;
    TreeNodePtr resultNode = nullptr;
    do
    {
        auto nodeIdx = estd::bits_value(ip,  subTreeSplitSize);
        unsigned lmp = llmp(node, ip);
        if(lmp > 0 || node->ibm[0])
        {
            resultNode = node;
        }
        node = node->childs[nodeIdx];
        if(node)
        {
            ip <<= subTreeSplitSize;
        }
    } while(node);
    if(resultNode)
    {
        auto lmp = llmp(resultNode, ip);
        auto maskIdx = lmp > 0 ? (1u << lmp) + estd::bits_value(ip,  lmp) : 0;
        auto mask = resultNode->masks[maskIdx];
        return mask;
    }
    return 0;
}

void IpPrefixBitmap::del(unsigned prefix, char mask)
{
    using Stack = std::vector<std::pair<TreeNodePtr, unsigned>>;

    Ip ip(prefix, mask);
    TreeNodePtr node = root, lastNodeWithMultipleChilds = nullptr;
    auto subTreeSplitSize = node->h + 1u, childToRemove = 0u;
    Stack stack(32u/subTreeSplitSize + 1u, {nullptr, 0u});
    int stackIdx = -1;

    while(ip.bit_len() >= subTreeSplitSize && node)
    {
        if(node->childsNum == 1 && node->ibm_empty())
        {
            stack[++stackIdx] = {node, estd::bits_value(ip.prefix, subTreeSplitSize)};
        }
        else
        {
            stackIdx = -1;
            lastNodeWithMultipleChilds = node;
            childToRemove = estd::bits_value(ip.prefix,  subTreeSplitSize);
        }
        node = node->childs[estd::bits_value(ip.prefix,  subTreeSplitSize)];
        ip <<= subTreeSplitSize;
    }
    if(node && node->ibm[ip.ibm_idx()])
    {
        node->ibm[ip.ibm_idx()] = false;
        if(node->childsNum == 0u && node->ibm_empty())
        {
            node.reset();
            while(stackIdx >= 0)
            {
                stack[stackIdx].first->childs[stack[stackIdx].second].reset();
                stack[stackIdx].first->childsNum = 0u;
                stack[stackIdx].first.reset();
                stackIdx--;
            }
            if(lastNodeWithMultipleChilds)
            {
                lastNodeWithMultipleChilds->childs[childToRemove].reset();
                lastNodeWithMultipleChilds->childsNum--;
            }
        }
    }
}

void IpPrefixBitmap::add(unsigned prefix, char mask)
{
    Ip ip(prefix, mask);
    auto node = root;
    auto subTreeSplitSize = root->h + 1u;
    while(ip.bit_len() >= subTreeSplitSize && node->childs[estd::bits_value(ip.prefix,  subTreeSplitSize)])
    {
        node = node->childs[estd::bits_value(ip.prefix,  subTreeSplitSize)];
        ip <<= subTreeSplitSize;
    }
    auto lastNodePrefix = ip.prefix;
    if(ip.bit_len() >= subTreeSplitSize)
    {
        do
        {
            auto nodeIdx = estd::bits_value(ip.prefix,  subTreeSplitSize);
            node->childsNum++;
            node = node->childs[nodeIdx] = std::make_shared<TreeNode>();
            lastNodePrefix = ip.prefix;
            ip <<= subTreeSplitSize;
        } while (ip.bit_len() >= subTreeSplitSize);
    }
    auto nodeIdx = ip.ibm_idx();
    node->ibm[nodeIdx] = true;
    node->masks[nodeIdx] = mask;
}

inline unsigned IpPrefixBitmap::llmp(const TreeNodePtr& node, unsigned d)
{
    unsigned i = (1u << node->h) + estd::bits_value(d, node->h);
    unsigned len = node->h;
    while(i > 0 and node->ibm[i] == false)
    {
        i >>= 1;
        if(len != 0)
            len--;
    }
    return len;
}
