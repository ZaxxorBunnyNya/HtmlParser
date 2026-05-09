//
// Created by Zaxxor on 02.05.2026.
//

#ifndef HTMLPARSER_TOKEN_HPP
#define HTMLPARSER_TOKEN_HPP

#include "TokenType.hpp"

#include <string>

namespace HtmlParser {
    struct Token {
        std::string val;
        TokenType type;
    };
}

#endif //HTMLPARSER_TOKEN_HPP
