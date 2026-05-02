//
// Created by maxim on 01.05.2026.
//

#include "Parser.h"

namespace HtmlParser {
    void Parser::Parse(const std::string &_html) {
        if (_html.empty()) {
            return;
        }

        this->m_root = std::make_shared<HtmlNode>("", HtmlNodeType::Root);

        auto node = std::make_shared<HtmlNode>();
        auto parent = this->m_root;
        std::string attributeName;

        std::string text;

        for (int i = 0; i < _html.length(); i++) {
            const auto c = _html[i];

            if (c == '<' && this->m_state == STATE::INITIAL) {
                if (text.empty() == false) {
                    auto textNode = std::make_shared<HtmlNode>();
                    textNode->setText(text);
                    textNode->setType(HtmlNodeType::Text);
                    textNode->setParent(parent);
                    parent->addChild(textNode);

                    text.clear();
                }

                this->m_state = STATE::START_TAG;
            } else if ((c == '\n' || c == ' ' || c == '\t') && this->m_state == STATE::INITIAL) {
            } else {
                switch (this->m_state) {
                    case STATE::START_TAG:
                        if (_html.length() > i + 2) {
                            if (_html[i] == '!' && _html[i + 1] == '-' && _html[i + 2] == '-') {
                                node = std::make_shared<HtmlNode>();

                                parent->addChild(node);
                                node->setParent(parent);

                                this->m_state = STATE::READING_COMMENT;
                                node->setType(HtmlNodeType::Comment);

                                i += 2;
                                continue;
                            }
                        }

                        if (_html[i] == '/') {
                            this->m_state = STATE::READING_TAG_TERMINATOR;
                            parent = node->getParent();

                            continue;
                        }

                        text += c;

                        node = std::make_shared<HtmlNode>();

                        node->setParent(parent);
                        node->setType(HtmlNodeType::Element);
                        parent->addChild(node);

                        this->m_state = STATE::READING_TAG;
                        break;

                    case STATE::READING_COMMENT:
                        if (c == '-') {
                            if (_html.length() > i + 2) {
                                if (_html[i] == '-' && _html[i + 1] == '-' && _html[i + 2] == '>') {
                                    this->m_state = STATE::INITIAL;

                                    parent = node;

                                    node = std::make_shared<HtmlNode>();
                                    node->setParent(parent);

                                    node->setType(HtmlNodeType::Text);
                                    node->setText(text);
                                    text.clear();

                                    parent->addChild(node);
                                    node = parent;
                                    parent = node->getParent();

                                    i += 2;

                                    continue;
                                }
                            }
                        }
                        text += c;
                        break;

                    case STATE::READING_TAG:
                        if (c == ' ') {
                            node->setTagName(text);
                            text.clear();

                            this->m_state = STATE::READING_ATTRIBUTES;
                            continue;
                        }

                        if (c == '>') {
                            node->setTagName(text);

                            text.clear();

                            parent = node;

                            this->m_state = STATE::INITIAL;
                            continue;
                        }

                        text += c;

                        break;

                    case STATE::READING_ATTRIBUTES:
                        if (c == '>') {
                            parent = node;

                            this->m_state = STATE::INITIAL;
                        } else if (c != ' ' && c != '\n' && c != '\t') {
                            text += c;

                            this->m_state = STATE::READING_ATTRIBUTE_NAME;
                        }
                        break;
                    case STATE::READING_ATTRIBUTE_NAME:
                        if (c == ' ' || c == '\t' || c == '\n') {
                            node->addAttribute(text, "");
                        } else if (c == '=') {
                            this->m_state = STATE::READING_ATTRIBUTE_VALUE;
                            attributeName = text;

                            text.clear();
                        } else if ( c == '>') {
                            node->addAttribute(text, "");
                            text.clear();

                            i--;
                            this->m_state = STATE::READING_ATTRIBUTES;
                        } else {
                            text += c;
                        }
                        break;
                    case STATE::READING_ATTRIBUTE_VALUE:
                        if (c == ' ' || c == '\t' || c == '\n') {
                            node->addAttribute(attributeName, text);

                            text.clear();
                            attributeName.clear();
                        } else if ( c == '>') {
                            node->addAttribute(attributeName, text);
                            text.clear();
                            attributeName.clear();

                            i--;
                            this->m_state = STATE::READING_ATTRIBUTES;
                        } else {
                            text += c;
                        }
                        break;
                    case STATE::INITIAL:
                        text += c;
                        break;
                    case STATE::READING_TAG_TERMINATOR:
                        if (c == '>') {
                            this->m_state = STATE::INITIAL;
                        }
                        break;
                    default: ;
                }
            }
        }
    }
} // HtmlParser
