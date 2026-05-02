//
// Created by Zaxxor on 01.05.2026.
//

#ifndef MINIBROWSER_HTMLNODE_HPP
#define MINIBROWSER_HTMLNODE_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "HtmlNodeType.hpp"

namespace HtmlParser {
    class HtmlNode {
    public:
        explicit HtmlNode(
            const std::string &_tag,
            const HtmlNodeType _type,
            const std::unordered_map<std::string, std::string> &_attributes,
            const std::vector<std::shared_ptr<HtmlNode> > &_children) : m_tagName(_tag),
                                                                        m_children(_children),
                                                                        m_attributes(_attributes),
                                                                        m_type(_type) {
        }

        explicit HtmlNode() {}

        explicit HtmlNode(const std::string &_tag, const HtmlNodeType _type) : m_tagName(_tag), m_type(_type) {
            this->m_children.clear();
        }

        void setTagName(const std::string &_tagName) {
            this->m_tagName = _tagName;
        }

        void setType(HtmlNodeType _type) {
            this->m_type = _type;
        }

        void setParent(const std::shared_ptr<HtmlNode> &_parent) {
            this->m_parent = _parent;
        }

        void setText(const std::string &_text) {
            this->m_text = _text;
        }

        HtmlNodeType getType() {
            return m_type;
        }

        std::string getText() {return m_text;}
        std::string getTagName() { return m_tagName; }
        std::shared_ptr<HtmlNode> getParent() { return m_parent; }

        std::vector<std::shared_ptr<HtmlNode> > getChildren() { return m_children; }
        std::unordered_map<std::string, std::string> getAttributes() { return m_attributes; }

        void addChild(const std::shared_ptr<HtmlNode> &_child) { m_children.push_back(_child); }

        void addAttribute(const std::string &_attribute, const std::string &_value) {
            m_attributes.insert({_attribute, _value});
        }

    private:
        std::string m_tagName;
        std::string m_text;
        std::vector<std::shared_ptr<HtmlNode> > m_children;
        std::unordered_map<std::string, std::string> m_attributes;
        HtmlNodeType m_type = HtmlNodeType::Text;
        std::shared_ptr<HtmlNode> m_parent = nullptr;
    };
}

#endif //MINIBROWSER_HTMLNODE_HPP
