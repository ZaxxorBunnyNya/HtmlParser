//
// Created by Zaxxor on 01.05.2026.
//

#ifndef HTMLPARSER_HTMLNODE_TYPE_HPP
#define HTMLPARSER_HTMLNODE_TYPE_HPP

namespace HtmlParser {

    enum class HtmlNodeType {
        Root,
        Comment,
        Element,
        Declaration,
        Text
    };

}

#endif //HTMLPARSER_HTMLNODE_TYPE_HPP
