//
//  WordTree.hpp
//  TypeAhead
//
//  Created by Kelson on 10/28/24.
//

#include <algorithm>
#include <array>
#include <cctype>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class TreeNode
{
  public:
    TreeNode();
    bool endOfWord;
    std::array<std::shared_ptr<TreeNode>, 26> children;
};

class WordTree
{
  public:
    WordTree();
    std::shared_ptr<TreeNode> root;
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size();
};
