//
// Created by Zaxxor on 01.05.2026.
//

#include <gtest/gtest.h>

#include "Parser.h"

TEST(BasicTests, ShouldReturnNullptrOnEmptyString) {
    auto parser = HtmlParser::Parser();

    parser.Parse("");

    ASSERT_EQ(parser.getRoot(), nullptr);
}

TEST(BasicTests, ShouldParseCommentsNoSpaces) {
    auto parser = HtmlParser::Parser();

    parser.Parse("<!--COMMENT-->");

    auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);
    auto comment = root->getChildren()[0];

    ASSERT_NE(comment, nullptr);
    ASSERT_EQ(comment->getChildren().size(), 1);
    ASSERT_EQ(comment->getType(), HtmlParser::HtmlNodeType::Comment);

    auto commentText = comment->getChildren()[0];
    ASSERT_NE(commentText, nullptr);
    ASSERT_EQ(commentText->getText(), "COMMENT");
}

TEST(BasicTests, ShouldParseCommentsStartSpaces) {
    auto parser = HtmlParser::Parser();

    parser.Parse("<!-- COMMENT-->");
    auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);
    auto comment = root->getChildren()[0];

    ASSERT_NE(comment, nullptr);
    ASSERT_EQ(comment->getChildren().size(), 1);
    ASSERT_EQ(comment->getType(), HtmlParser::HtmlNodeType::Comment);

    auto commentText = comment->getChildren()[0];
    ASSERT_NE(commentText, nullptr);
    ASSERT_EQ(commentText->getText(), " COMMENT");
}

TEST(BasicTests, ShouldParseCommentsBothSpaces) {
    auto parser = HtmlParser::Parser();

    parser.Parse("<!-- COMMENT -->");

    auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);
    auto comment = root->getChildren()[0];

    ASSERT_NE(comment, nullptr);
    ASSERT_EQ(comment->getChildren().size(), 1);
    ASSERT_EQ(comment->getType(), HtmlParser::HtmlNodeType::Comment);

    auto commentText = comment->getChildren()[0];
    ASSERT_NE(commentText, nullptr);
    ASSERT_EQ(commentText->getText(), " COMMENT ");
}


TEST(BasicTests, ShouldParseNode) {
    auto parser = HtmlParser::Parser();

    parser.Parse("<p>TEST</p>");

    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);
    const auto element = root->getChildren()[0];

    ASSERT_NE(element, nullptr);
    ASSERT_EQ(element->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(element->getChildren().size(), 1);

    auto text = element->getChildren()[0];
    ASSERT_NE(text, nullptr);
    ASSERT_EQ(text->getText(), "TEST");
}

TEST(BasicTests, ShouldParseCommentAndNode) {
    auto parser = HtmlParser::Parser();

    parser.Parse("<!-- COMMENT --> \n"
        "<p>TEST</p>");

    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 2);
    const auto comment = root->getChildren()[0];
    const auto element = root->getChildren()[1];

    ASSERT_NE(comment, nullptr);
    ASSERT_EQ(comment->getType(), HtmlParser::HtmlNodeType::Comment);

    ASSERT_NE(element, nullptr);
    ASSERT_EQ(element->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(element->getChildren().size(), 1);

    auto text = element->getChildren()[0];
    ASSERT_NE(text, nullptr);
    ASSERT_EQ(text->getText(), "TEST");
}

TEST(BasicTest, ShouldParseNestedItems) {
    auto parser = HtmlParser::Parser();

    std::string html = "<div>\n"
            "<p>1</p> \n"
            "<p>2</p> \n"
            "<p>3</p> \n"
            "</div>";

    parser.Parse(html);
    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);

    const auto divElement = root->getChildren()[0];
    ASSERT_NE(divElement, nullptr);
    ASSERT_EQ(divElement->getChildren().size(), 3);
    ASSERT_EQ(divElement->getType(), HtmlParser::HtmlNodeType::Element);

    auto childs = divElement->getChildren();
    ASSERT_EQ(childs.size(), 3 );

    ASSERT_EQ(childs[0]->getChildren().size(), 1);
    ASSERT_EQ(childs[0]->getChildren()[0]->getText(), "1");

    ASSERT_EQ(childs[1]->getChildren().size(), 1);
    ASSERT_EQ(childs[1]->getChildren()[0]->getText(), "2");

    ASSERT_EQ(childs[2]->getChildren().size(), 1);
    ASSERT_EQ(childs[2]->getChildren()[0]->getText(), "3");
}

TEST(BasicTest, ShouldParseNestedItemsWithComments) {
    auto parser = HtmlParser::Parser();

    std::string html = "<div>\n"
            "<!-- COMMENT1 --> \n"
            "<p>1</p> \n"
            "<!-- COMMENT2 --> \n"
            "<p>2</p> \n"
            "<!-- COMMENT3 --> \n"
            "<p>3</p> \n"
            "</div>";

    parser.Parse(html);
    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);

    const auto divElement = root->getChildren()[0];
    ASSERT_NE(divElement, nullptr);
    ASSERT_EQ(divElement->getType(), HtmlParser::HtmlNodeType::Element);

    auto childs = divElement->getChildren();
    ASSERT_EQ(childs.size(), 6 );

    ASSERT_EQ(childs[0]->getChildren().size(), 1);
    ASSERT_EQ(childs[0]->getType(), HtmlParser::HtmlNodeType::Comment);
    ASSERT_EQ(childs[0]->getChildren()[0]->getText(), " COMMENT1 ");

    ASSERT_EQ(childs[1]->getChildren().size(), 1);
    ASSERT_EQ(childs[1]->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(childs[1]->getChildren()[0]->getText(), "1");

    ASSERT_EQ(childs[2]->getChildren().size(), 1);
    ASSERT_EQ(childs[2]->getType(), HtmlParser::HtmlNodeType::Comment);
    ASSERT_EQ(childs[2]->getChildren()[0]->getText(), " COMMENT2 ");

    ASSERT_EQ(childs[3]->getChildren().size(), 1);
    ASSERT_EQ(childs[3]->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(childs[3]->getChildren()[0]->getText(), "2");

    ASSERT_EQ(childs[4]->getChildren().size(), 1);
    ASSERT_EQ(childs[4]->getType(), HtmlParser::HtmlNodeType::Comment);
    ASSERT_EQ(childs[4]->getChildren()[0]->getText(), " COMMENT3 ");

    ASSERT_EQ(childs[5]->getChildren().size(), 1);
    ASSERT_EQ(childs[5]->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(childs[5]->getChildren()[0]->getText(), "3");
}

TEST(BasicTest, ShouldParseAttributeDoubleQuotes) {
    auto parser = HtmlParser::Parser();

    const std::string html = "<p align=\"center\">text</p>";

    parser.Parse(html);
    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);

    const auto divElement = root->getChildren()[0];
    ASSERT_NE(divElement, nullptr);
    ASSERT_EQ(divElement->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(divElement->getAttributes().size(), 1);

    auto attributes = divElement->getAttributes();

    ASSERT_NE(attributes.find("align"),  attributes.end());
    ASSERT_EQ(attributes.at("align"), "center");
}

TEST(BasicTest, ShouldParseAttributeSingleQuotes) {
    auto parser = HtmlParser::Parser();

    const std::string html = "<p align='center'>text</p>";

    parser.Parse(html);
    const auto root = parser.getRoot();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getChildren().size(), 1);

    const auto divElement = root->getChildren()[0];
    ASSERT_NE(divElement, nullptr);
    ASSERT_EQ(divElement->getType(), HtmlParser::HtmlNodeType::Element);
    ASSERT_EQ(divElement->getAttributes().size(), 1);

    auto attributes = divElement->getAttributes();

    ASSERT_NE(attributes.find("align"),  attributes.end());
    ASSERT_EQ(attributes.at("align"), "center");
}

