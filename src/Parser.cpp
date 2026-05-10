//
// Created by Zaxxor on 01.05.2026.
//

#include "Parser.h"

#include <regex>
#include <unordered_set>

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

                    if (node->getType() != HtmlNodeType::Text && node->getType() != HtmlNodeType::Declaration) {
                        parent = node;
                    }

                    break;
                case STATE::READING_TAG:
                    if (tokens[i].type == TokenType::TagEnd) {
                        parent = node->getParent();

                        continue;
                    }

                    node->setTagName(tokens[i].val);

                    if (requiresClosingTag(tokens[i].val) == true) {
                        this->m_state = STATE::READING_ATTRIBUTES;
                    } else {
                        this->m_state = STATE::READING_SELF_CLOSING_ELEMENT;
                    }

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
                    i = this->workOnReadingAttributes(i, tokens, node, parent);
                    break;
                case STATE::READING_TAG_TERMINATOR:
                    if (tokens[i].type == TokenType::TagEnd) {
                        this->m_state = STATE::INITIAL;

                        if (parent != nullptr) {
                            parent = parent->getParent();
                        }
                    }
                    break;
                case STATE::READING_SELF_CLOSING_ELEMENT:
                     i = this->workOnReadingSelfClosingElement(i, tokens, node, parent);
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
        } else if (token.type == TokenType::DeclaratorStart) {
            auto node = std::make_shared<HtmlNode>("", HtmlNodeType::Declaration);
            node->setParent(_parent);
            this->m_state = STATE::READING_SELF_CLOSING_ELEMENT;

            return node;
        }

        return nullptr;
    }

    size_t  Parser::workOnReadingAttributes(const size_t &_counter,const std::vector<Token> &_tokens,const std::shared_ptr<HtmlNode> &_node,
                                     const std::shared_ptr<HtmlNode> &_parent) {
        auto curToken = _tokens[_counter];
        std::string attributeName;
        bool isHasAssign = false;

        size_t counter = _counter;

        while (true) {
            if (curToken.type == TokenType::TagEnd) {
                this->m_state = STATE::INITIAL;

                break;
            }

            if (curToken.type == TokenType::AssignSymbol) {
                isHasAssign = true;
            }

            if (isHasAssign == true) {
                if (curToken.type == TokenType::Word || curToken.type == TokenType::QuotedText) {
                    _node->addAttribute(attributeName, curToken.val);
                    attributeName.clear();
                }
            } else {
                if (attributeName.empty()) {
                    attributeName = curToken.val;
                } else {
                    _node->addAttribute(attributeName, "");
                    attributeName.clear();
                }
            }

            counter+= 1;
            curToken = _tokens[counter];
        }

        this->m_state = STATE::INITIAL;

        return counter;
    }

    size_t Parser::workOnReadingSelfClosingElement(const size_t &_counter, const std::vector<Token> &_tokens,
                                                  const std::shared_ptr<HtmlNode> &_node,
                                                  const std::shared_ptr<HtmlNode> &_parent) {
        auto curToken = _tokens[_counter];
        std::string attributeName;
        bool isHasAssign = false;
        size_t counter = _counter;

        while (true) {
            if (curToken.type == TokenType::TagEnd) {
                this->m_state = STATE::INITIAL;

                if (attributeName.empty() == false) {
                    _node->addAttribute(attributeName, "");
                }

                break;
            }

            if (curToken.type == TokenType::AssignSymbol) {
                isHasAssign = true;
            }

            if (isHasAssign == true) {
                if (curToken.type == TokenType::Word || curToken.type == TokenType::QuotedText) {
                    _node->addAttribute(attributeName, curToken.val);
                    attributeName.clear();
                }
            } else {
                if (curToken.type == TokenType::Word || curToken.type == TokenType::QuotedText) {
                    if (attributeName.empty()) {
                        attributeName = curToken.val;
                    } else {
                        _node->addAttribute(attributeName, "");
                        attributeName.clear();
                        attributeName = curToken.val;
                    }
                }
            }

            counter+= 1;
            curToken = _tokens[counter];
        }

        return counter;
    }

    /**
     * @brief Проверяет, требуется ли закрывающий тег для указанного HTML-элемента.
     * @param tagName Имя тега (например, "div", "BR", "img").
     * @return true  - если тег требует закрытия (например, <p>, <div>)
     *         false - если тег является void-элементом и закрытие не требуется (например, <br>, <img>)
     */
    bool Parser::requiresClosingTag(const std::string& tagName) {
        if (tagName.empty()) return true;

        // Приводим имя к нижнему регистру для регистронезависимого сравнения
        std::string lowerName = tagName;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        // Список void-элементов согласно спецификации HTML5
        static const std::unordered_set<std::string> voidElements = {
            "area", "base", "br", "col", "embed", "hr", "img",
            "input", "link", "meta", "param", "source", "track", "wbr"
        };

        // Если тег НЕ найден в списке void-элементов, он требует закрытия
        return !voidElements.contains(lowerName);
    }
} // HtmlParser
