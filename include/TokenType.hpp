//
// Created by Zaxxor on 05.05.2026.
//

#ifndef HTMLPARSER_TOKEN_TYPE_HPP
#define HTMLPARSER_TOKEN_TYPE_HPP

namespace HtmlParser {
    enum class TokenType {
        CommentStart,
        CommentEnd,
        Word,
        TagStart,
        DeclaratorStart,
        TagCloserStart,
        TagEnd,
        EndSelfClosingTag,
        MultiwordText,
        AssignSymbol,
        QuotedText,
    };
}
#endif //HTMLPARSER_TOKEN_TYPE_HPP
