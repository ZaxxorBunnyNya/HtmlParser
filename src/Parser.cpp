//
// Created by maxim on 01.05.2026.
//

#include "Parser.h"

#include <regex>

namespace HtmlParser {
    void Parser::Parse(const std::string &_html) {
        if (_html.empty()) {
            return;
        }

        this->m_root = std::make_shared<HtmlNode>("", HtmlNodeType::Root);
        auto node = std::make_shared<HtmlNode>("", HtmlNodeType::Root);
        this->m_state = STATE::INITIAL;

        auto parent = this->m_root;
        std::string attributeName;

        auto tokenizer = Tokenizer();
        const auto tokens = tokenizer.Tokenize(_html);

        for (size_t i = 0; i < tokens.size(); i++) {
            switch (this->m_state) {
                case STATE::INITIAL:
                    node = this->workOnInitialState(i, tokens, parent);

                    if (node == nullptr) {
                        continue;
                    }

                    parent->addChild(node);

                    if (node->getType() != HtmlNodeType::Text) {
                        parent = node;
                    }

                    break;
                case STATE::READING_TAG:
                    if (tokens[i].type == TokenType::TagEnd) {
                        parent = node->getParent();

                        continue;
                    }

                    node->setTagName(tokens[i].val);
                    this->m_state = STATE::READING_ATTRIBUTES;

                    break;
                case STATE::READING_COMMENT:
                    if (tokens[i].type == TokenType::CommentEnd) {
                        this->m_state = STATE::INITIAL;
                        parent = parent->getParent();
                    } else {
                        node = std::make_shared<HtmlNode>("", HtmlNodeType::Text);
                        node->setParent(parent);
                        node->setText(tokens[i].val);
                        parent->addChild(node);
                    }
                    break;
                case STATE::READING_ATTRIBUTES:
                    this->workOnReadingAttributes(i, tokens, node, parent);
                    break;
                case STATE::READING_TAG_TERMINATOR:
                    if (tokens[i].type == TokenType::TagEnd) {
                        this->m_state = STATE::INITIAL;
                        parent = parent->getParent();
                    }
                    break;

                default:
                    break;
            }
        }
    }

    std::shared_ptr<HtmlNode> Parser::workOnInitialState(const size_t &_counter, const std::vector<Token> &_tokens,
                                                         const std::shared_ptr<HtmlNode> &_parent) {
        auto token = _tokens[_counter];

        if (token.type == TokenType::CommentStart) {
            auto node = std::make_shared<HtmlNode>("", HtmlNodeType::Comment);
            node->setParent(_parent);
            this->m_state = STATE::READING_COMMENT;

            return node;
        } else if (token.type == TokenType::MultiwordText) {
            std::regex pattern("[a-zA-Z0-9]");

            auto searchOn = token.val.substr(0, token.val.size());

            auto reBegin = std::sregex_iterator(searchOn.begin(), searchOn.end(), pattern);
            auto reEnd = std::sregex_iterator();

            if (reBegin == reEnd) {
                return nullptr;
            }

            auto node = std::make_shared<HtmlNode>("", HtmlNodeType::Text);
            node->setText(token.val);
            node->setParent(_parent);

            return node;
        } else if (token.type == TokenType::TagStart) {
            auto node = std::make_shared<HtmlNode>("", HtmlNodeType::Element);
            node->setParent(_parent);
            this->m_state = STATE::READING_TAG;

            return node;
        } else if (token.type == TokenType::TagCloserStart) {
            this->m_state = STATE::READING_TAG_TERMINATOR;

            return nullptr;
        }

        return nullptr;
    }

    std::shared_ptr<HtmlNode>  Parser::workOnReadingAttributes(const size_t &_counter,const std::vector<Token> &_tokens,const std::shared_ptr<HtmlNode> &_node,
                                     const std::shared_ptr<HtmlNode> &_parent) {
        auto curToken = _tokens[_counter];
        std::string attributeName;
        bool isHasAssign = false;

        size_t counter = _counter;

        while (true) {
            if (curToken.type == TokenType::TagEnd) {
                break;
            }

            if (isHasAssign == true) {
                if (curToken.type == TokenType::Word || curToken.type == TokenType::QuotedText) {
                    _node->addAttribute(attributeName, curToken.val);
                }
            } else {
                if (curToken.type == TokenType::Word || curToken.type == TokenType::QuotedText) {
                    attributeName = curToken.val;
                }
            }

            if (curToken.type == TokenType::AssignSymbol) {
                isHasAssign = true;
            }

            counter+= 1;
            curToken = _tokens[counter];
        }

        this->m_state = STATE::INITIAL;

        return _node;
    }
} // HtmlParser
