#pragma once

#include <vector>
#include <memory>
#include <algorithm>

struct TreeNode;
using TreeNodePtr = std::shared_ptr<TreeNode>;

static constexpr unsigned TreeSplitSize = 3;

struct TreeNode
{
    TreeNode()
            : childs(1u << TreeSplitSize, nullptr),
              ibm(1u << TreeSplitSize, false),
              masks(1u << TreeSplitSize, 0),
              h(TreeSplitSize - 1),
              childsNum(0)
    {
    }
    std::vector<TreeNodePtr> childs;
    std::vector<bool> ibm;
    std::vector<char> masks;
    unsigned h;
    unsigned childsNum;
    inline bool ibm_empty()
    {
        return std::all_of(std::begin(ibm), std::end(ibm), [](bool bit){ return !bit; });
    }
};




