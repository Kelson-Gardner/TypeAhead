//
//  WordTree.cpp
//  TypeAhead
//
//  Created by Kelson on 10/28/24.
//

#include "WordTree.hpp"

bool isInvalidWord(std::string word);
std::size_t countWords(const std::shared_ptr<TreeNode>& node);

TreeNode::TreeNode()
{
    children.fill(nullptr);
    endOfWord = false;
}

WordTree::WordTree() { root = std::make_shared<TreeNode>(); }

void WordTree::add(std::string word)
{
    if (isInvalidWord(word))
    {
        return;
    }

    std::shared_ptr<TreeNode> tempRoot = root;
    for (char letter : word)
    {
        int index = std::tolower(letter) - 'a';
        if (!tempRoot->children[index])
        {
            tempRoot->children[index] = std::make_shared<TreeNode>();
        }
        tempRoot = tempRoot->children[index];
    }
    tempRoot->endOfWord = true;
}
bool WordTree::find(std::string word)
{
    if (isInvalidWord(word))
    {
        return false;
    }
    std::shared_ptr<TreeNode> tempRoot = root;
    for (char letter : word)
    {
        int index = std::tolower(letter) - 'a';
        if (!tempRoot->children[index])
        {
            return false;
        }
        tempRoot = tempRoot->children[index];
    }
    return tempRoot->endOfWord;
}

std::vector<std::string> WordTree::predict(std::string partial,
                                           std::uint8_t howMany)
{
    std::vector<std::string> predictions;
    if (isInvalidWord(partial))
    {
        return predictions;
    }
    std::shared_ptr<TreeNode> currentNode = root;
    for (char letter : partial)
    {
        int index = std::tolower(letter) - 'a';
        if (!currentNode->children[index])
        {
            return predictions;
        }
        currentNode = currentNode->children[index];
    }

    std::queue<std::pair<std::shared_ptr<TreeNode>, std::string>> q;
    q.push({currentNode, partial});

    while (!q.empty() && predictions.size() < howMany)
    {
        auto [node, prefix] = q.front();
        q.pop();

        if (node->endOfWord)
        {
            predictions.push_back(prefix);
        }
        for (int i = 0; i < 26; ++i)
        {
            if (node->children[i])
            {
                char nextChar = 'a' + i;
                q.push({node->children[i], prefix + nextChar});
            }
        }
    }
    return predictions;
}

std::size_t WordTree::size()
{
    auto node = root;
    return countWords(node);
}

std::size_t countWords(const std::shared_ptr<TreeNode>& node)
{
    if (!node)
    {
        return 0;
    }

    std::size_t count = node->endOfWord ? 1 : 0;

    for (auto child : node->children)
    {
        if (child)
        {
            count += countWords(child);
        }
    }
    return count;
}

bool isInvalidWord(std::string word)
{
    if (word.empty())
    {
        return true;
    }
    for (char letter : word)
    {
        if (!std::isalpha(letter))
        {
            return true;
        }
    }

    return false;
}
