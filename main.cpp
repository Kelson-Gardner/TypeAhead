//
//  main.cpp
//  TypeAhead
//
//  Created by Kelson on 10/28/24.
//
#include "rlutil.h"
#include "WordTree.hpp"

#include <fstream>
#include <iostream>

std::shared_ptr<WordTree> readDictionary(std::string filename);

int main()
{
    auto wordTree = readDictionary("dictionary.txt");
    std::string input;
    std::string currentWord;

    int cursorIndex = 0;

    while (true)
    {
        rlutil::locate(cursorIndex, 0);
        char key = rlutil::getkey();
        rlutil::cls();

        if (key == rlutil::KEY_BACKSPACE && !input.empty())
        {
            cursorIndex -= 1;
            if (!currentWord.empty())
            {
                currentWord.pop_back();
            }
            input.pop_back();
        }
        else if (key == '\n')
        {
            input.clear();
            currentWord.clear();
        }
        else if (std::isalpha(key))
        {
            cursorIndex += 1;
            input += key;
            currentWord += key;
        }
        else if (key == ' ')
        {
            cursorIndex += 1;
            input += key;
            currentWord.clear();
        }

        if (input == "exit")
        {
            break;
        }

        std::cout << input << std::endl;

        std::cout << "\n--- Prediction for current word ---\n";

        auto predictions = wordTree->predict(currentWord, rlutil::trows() - 3);
        int predictionIndex = 0;
        for (const auto& word : predictions)
        {
            if (predictionIndex == rlutil::trows() - 5)
            {
                break;
            }
            std::cout << word << '\n';
            predictionIndex++;
        }
    }

    return 0;
}

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it
        // exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send
        // isalpha an unsigned char or it will throw exception on negative
        // values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(),
                        [](unsigned char c) { return std::isalpha(c); }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c)
                           { return static_cast<char>(std::tolower(c)); });
            wordTree->add(word);
        }
    }

    return wordTree;
}
