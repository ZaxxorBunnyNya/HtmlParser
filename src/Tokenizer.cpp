//
// Created by Zaxxor on 02.05.2026.
//

#include "../include/Tokenizer.hpp"

#include <regex>

namespace HtmlParser {
    std::vector<Token> Tokenizer::Tokenize(const std::string &_text) {
        std::vector<Token> tokens;

        for (size_t i = 0; i < _text.length();) {
            auto curSymbol = _text[i];

            if (curSymbol == '<') {
                if (_text.size() > i + 3) {
                    if (_text[i + 1] == '!' && _text[i + 2] == '-' && _text[i + 3] == '-') {
                        i += tokenizeComment(_text, tokens, i) - 1;

                        continue;
                    }
                }

                if (_text.size() > i + 1) {
                    if (_text[i + 1] == '/') {
                        i += tokenizeTagCloser(_text, tokens, i);

                        continue;
                    }
                }

                i += tokenizeTag(_text, tokens, i);
            } else {
                auto tokenVal = makeMultiwordText(_text, i);

                tokens.push_back({
                    tokenVal, TokenType::MultiwordText
                });

                i += tokenVal.length();
            }
        }

        return tokens;
    }

    std::string Tokenizer::makeWord(const std::string &_text, const size_t _offset) {
        std::regex pattern("[^a-zA-Z0-9]");

        auto searchOn = _text.substr(_offset, _text.size() - _offset);

        auto reBegin = std::sregex_iterator(searchOn.begin(), searchOn.end(), pattern);
        auto reEnd = std::sregex_iterator();

        for (std::sregex_iterator i = reBegin; i != reEnd; ++i) {
            const std::smatch &match = *i;

            return searchOn.substr(0, match.position());
        }

        return "";
    }

    std::string Tokenizer::makeMultiwordText(const std::string &_text, const size_t _offset) {
        if (_offset > _text.size()) {
            return {};
        }

        const auto endPos = _text.find('<', _offset);
        const size_t length = (endPos == std::string::npos) ? std::string::npos : (endPos - _offset);

        return _text.substr(_offset, length);
    }

    size_t Tokenizer::tokenizeComment(const std::string &_text, std::vector<Token> &_tokens, size_t _offset) {
        auto commentsEnd = _text.find("-->", _offset);

        _tokens.push_back({
            "<!--", TokenType::CommentStart
        });

        _tokens.push_back({
            _text.substr(_offset + 4, commentsEnd - _offset - 4),
            TokenType::MultiwordText
        });

        _tokens.push_back({
            "-->", TokenType::CommentEnd
        });

        return commentsEnd - _offset + 4;
    }

    size_t Tokenizer::tokenizeTag(const std::string &_text, std::vector<Token> &_tokens, size_t _offset) {
        for (size_t i = _offset; i < _text.size(); i++) {
            if (_text[i] == '<') {
                _tokens.push_back({
                    "<",
                    TokenType::TagStart
                });
            } else if (_text[i] == '>') {
                _tokens.push_back({
                    ">",
                    TokenType::TagEnd
                });

                return i + 1 - _offset;
            } else if (_text[i] == '=') {
                _tokens.push_back({
                    "=",
                    TokenType::AssignSymbol
                });
            } else if (_text[i] == '\'') {
                auto quotedEnd = _text.find('\'', i + 1);

                auto text = _text.substr(i + 1, quotedEnd - i - 1);
                _tokens.push_back({
                    text, TokenType::QuotedText
                });

                i = quotedEnd;
            } else if (_text[i] == '"') {
                auto quotedEnd = _text.find('"', i + 1);

                auto text = _text.substr(i + 1, quotedEnd - i - 1);
                _tokens.push_back({
                    text, TokenType::QuotedText
                });

                i = quotedEnd ;
            } else {
                auto tokenVal = makeWord(_text, i);

                if (tokenVal.empty()) {
                    continue;
                }

                _tokens.push_back({
                    tokenVal, TokenType::Word
                });

                i += tokenVal.length() - 1;
            }
        }

        return _text.size();
    }

    size_t Tokenizer::tokenizeTagCloser(const std::string &_text, std::vector<Token> &_tokens, size_t _offset) {
        _tokens.push_back({
            "</", TokenType::TagCloserStart
        });

        for (size_t i = _offset + 2; i < _text.size();) {
            if (_text[i] == '>') {
                _tokens.push_back({
                    ">",
                    TokenType::TagEnd
                });

                return i - _offset + 1;
            }

            auto tokenVal = makeWord(_text, i);

            if (tokenVal.empty()) {
                continue;
            }

            _tokens.push_back({
                tokenVal, TokenType::Word
            });

            i += tokenVal.length();
        }

        return _text.size() - _offset;
    }
}
