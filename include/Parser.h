#ifndef MINIBROWSER_PARSER_H
#define MINIBROWSER_PARSER_H
#include <string>

#include "HtmlNode.hpp"

namespace HtmlParser {
    enum STATE {
        INITIAL,
        START_TAG,
        READING_TAG,
        READING_TAG_TERMINATOR,
        READING_ATTRIBUTES,
        READING_TEXT,
        END_TAG,
        READING_COMMENT,
        END_COMMENT,
    };

class Parser {
public:
    void Parse(const std::string &_html);

    std::shared_ptr<HtmlNode> getRoot() {return this->m_root;}

private:
    std::shared_ptr<HtmlNode> m_root;
    STATE m_state = STATE::INITIAL;
};

} // HtmlParser

#endif //MINIBROWSER_PARSER_H
