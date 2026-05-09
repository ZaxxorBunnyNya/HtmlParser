//
// Created by Zaxxor on 05.05.2026.
//

#include "Tokenizer.hpp"
#include "gtest/gtest.h"

TEST(TokenizerTests, ShouldParseMultiwordText) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("Hello world!");

    ASSERT_EQ(tokens.size(), 1);
    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::MultiwordText);
    ASSERT_EQ(tokens[0].val, "Hello world!");
}

TEST(TokenizerTests, ShouldParseTag) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("<p>");

    ASSERT_EQ(tokens.size(), 3);

    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::TagStart);
    ASSERT_EQ(tokens[0].val, "<");

    ASSERT_EQ(tokens[1].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[1].val, "p");

    ASSERT_EQ(tokens[2].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[2].val, ">");
}

TEST(TokenizerTests, ShouldParseComments) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("<!--COMMENT-->");

    ASSERT_EQ(tokens.size(), 3);

    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::CommentStart);
    ASSERT_EQ(tokens[0].val, "<!--");

    ASSERT_EQ(tokens[1].type, HtmlParser::TokenType::MultiwordText);
    ASSERT_EQ(tokens[1].val, "COMMENT");

    ASSERT_EQ(tokens[2].type, HtmlParser::TokenType::CommentEnd);
    ASSERT_EQ(tokens[2].val, "-->");
}

TEST(TokenizerTests, ShouldParseCommentsAndTag) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("<!--COMMENT--><tag>");

    ASSERT_EQ(tokens.size(), 6);

    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::CommentStart);
    ASSERT_EQ(tokens[0].val, "<!--");

    ASSERT_EQ(tokens[1].type, HtmlParser::TokenType::MultiwordText);
    ASSERT_EQ(tokens[1].val, "COMMENT");

    ASSERT_EQ(tokens[2].type, HtmlParser::TokenType::CommentEnd);
    ASSERT_EQ(tokens[2].val, "-->");

    ASSERT_EQ(tokens[3].type, HtmlParser::TokenType::TagStart);
    ASSERT_EQ(tokens[3].val, "<");

    ASSERT_EQ(tokens[4].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[4].val, "tag");

    ASSERT_EQ(tokens[5].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[5].val, ">");
}

TEST(TokenizerTests, ShouldParseTagWithText) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("<p>Some text</p>");

    ASSERT_EQ(tokens.size(), 7);

    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::TagStart);
    ASSERT_EQ(tokens[0].val, "<");

    ASSERT_EQ(tokens[1].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[1].val, "p");

    ASSERT_EQ(tokens[2].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[2].val, ">");

    ASSERT_EQ(tokens[3].type, HtmlParser::TokenType::MultiwordText);
    ASSERT_EQ(tokens[3].val, "Some text");

    ASSERT_EQ(tokens[4].type, HtmlParser::TokenType::TagCloserStart);
    ASSERT_EQ(tokens[4].val, "</");

    ASSERT_EQ(tokens[5].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[5].val, "p");

    ASSERT_EQ(tokens[6].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[6].val, ">");
}

TEST(TokenizerTests, ShouldParseMultipleTagWithText) {
    auto tokenizer = HtmlParser::Tokenizer();
    const auto tokens = tokenizer.Tokenize("<p>Some text0</p><p>Some text1</p><p>Some text2</p><p>Some text3</p><p>Some text4</p>");

    ASSERT_EQ(tokens.size() % 7, 0);

    for (size_t i = 0; i < tokens.size(); i += 7) {
        ASSERT_EQ(tokens[i].type, HtmlParser::TokenType::TagStart);
        ASSERT_EQ(tokens[i].val, "<");

        ASSERT_EQ(tokens[i + 1].type, HtmlParser::TokenType::Word);
        ASSERT_EQ(tokens[i + 1].val, "p");

        ASSERT_EQ(tokens[i + 2].type, HtmlParser::TokenType::TagEnd);
        ASSERT_EQ(tokens[i + 2].val, ">");

        ASSERT_EQ(tokens[i + 3].type, HtmlParser::TokenType::MultiwordText);
        ASSERT_EQ(tokens[i + 3].val, "Some text" + std::to_string(i / 7));

        ASSERT_EQ(tokens[i + 4].type, HtmlParser::TokenType::TagCloserStart);
        ASSERT_EQ(tokens[i + 4].val, "</");

        ASSERT_EQ(tokens[i + 5].type, HtmlParser::TokenType::Word);
        ASSERT_EQ(tokens[i + 5].val, "p");

        ASSERT_EQ(tokens[i + 6].type, HtmlParser::TokenType::TagEnd);
        ASSERT_EQ(tokens[i + 6].val, ">");
    }
}

TEST(TokenizerTests, ShouldParseNestedTags) {
    auto tokenizer = HtmlParser::Tokenizer();

    const auto tokens = tokenizer.Tokenize("<div><p>Some text0</p><p>Some text1</p></div>");

    ASSERT_EQ(tokens.size(), 20);

    ASSERT_EQ(tokens[0].type, HtmlParser::TokenType::TagStart);
    ASSERT_EQ(tokens[0].val, "<");

    ASSERT_EQ(tokens[1].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[1].val, "div");

    ASSERT_EQ(tokens[2].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[2].val, ">");

    for (size_t i = 3; i < tokens.size() - 3; i += 7) {
        ASSERT_EQ(tokens[i].type, HtmlParser::TokenType::TagStart);
        ASSERT_EQ(tokens[i].val, "<");

        ASSERT_EQ(tokens[i + 1].type, HtmlParser::TokenType::Word);
        ASSERT_EQ(tokens[i + 1].val, "p");

        ASSERT_EQ(tokens[i + 2].type, HtmlParser::TokenType::TagEnd);
        ASSERT_EQ(tokens[i + 2].val, ">");

        ASSERT_EQ(tokens[i + 3].type, HtmlParser::TokenType::MultiwordText);
        ASSERT_EQ(tokens[i + 3].val, "Some text" + std::to_string(i / 7));

        ASSERT_EQ(tokens[i + 4].type, HtmlParser::TokenType::TagCloserStart);
        ASSERT_EQ(tokens[i + 4].val, "</");

        ASSERT_EQ(tokens[i + 5].type, HtmlParser::TokenType::Word);
        ASSERT_EQ(tokens[i + 5].val, "p");

        ASSERT_EQ(tokens[i + 6].type, HtmlParser::TokenType::TagEnd);
        ASSERT_EQ(tokens[i + 6].val, ">");
    }

    ASSERT_EQ(tokens[tokens.size() - 3].type, HtmlParser::TokenType::TagCloserStart);
    ASSERT_EQ(tokens[tokens.size() - 3].val, "</");

    ASSERT_EQ(tokens[tokens.size() - 2].type, HtmlParser::TokenType::Word);
    ASSERT_EQ(tokens[tokens.size() - 2].val, "div");

    ASSERT_EQ(tokens[tokens.size() - 1].type, HtmlParser::TokenType::TagEnd);
    ASSERT_EQ(tokens[tokens.size() - 1].val, ">");

}