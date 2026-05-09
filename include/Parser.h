#ifndef MINIBROWSER_PARSER_H
#define MINIBROWSER_PARSER_H
#include <string>

#include "HtmlNode.hpp"
#include "HtmlVersion.hpp"
#include "Tokenizer.hpp"

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

        std::shared_ptr<HtmlNode> getRoot() { return this->m_root; }
        HtmlVersion getVersion() const { return this->m_version; }

    private:
        std::shared_ptr<HtmlNode> m_root;
        HtmlVersion m_version = HtmlVersion::VERSION_3_2;
        STATE m_state = STATE::INITIAL;

        std::shared_ptr<HtmlNode> workOnInitialState(const size_t &_counter, const std::vector<Token> &_tokens,
                                                     const std::shared_ptr<HtmlNode> &_parent);

        std::shared_ptr<HtmlNode> workOnReadingAttributes(const size_t &_counter, const std::vector<Token> &_tokens,
                                                          const std::shared_ptr<HtmlNode> &_node,
                                                          const std::shared_ptr<HtmlNode> &_parent);
    };
} // HtmlParser

#endif //MINIBROWSER_PARSER_H
