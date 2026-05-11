//
// Created by Zaxxor on 02.05.2026.
//

#ifndef HTMLPARSER_TOKENIZER_HPP
#define HTMLPARSER_TOKENIZER_HPP

#include <iostream>
#include <vector>
#include <regex>
#include "Token.hpp"

namespace HtmlParser {
    class Tokenizer {
    public:
        std::vector<Token> Tokenize(const std::string &_text);

    private:
        static std::string makeWord(const std::string &_text, size_t _offset);

        static std::string makeMultiwordText(const std::string &_text, size_t _offset);

        static size_t tokenizeComment(const std::string &_text, std::vector<Token> &_tokens, size_t _offset);
        static size_t tokenizeTag(const std::string &_text, std::vector<Token> &_tokens, size_t _offset);
        static size_t tokenizeTagCloser(const std::string &_text, std::vector<Token> &_tokens, size_t _offset);
    };
}


#endif //HTMLPARSER_TOKENIZER_HPP
