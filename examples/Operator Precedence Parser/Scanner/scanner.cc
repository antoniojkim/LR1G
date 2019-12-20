
#include "scanner.h"
#include <sstream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// unordered_map<string, Type> keywordType = {
// 	{keywordTypes}
// };
unordered_map<Type, string> keywordLexeme = {
	{TRUE_, "TRUE"},
	{FALSE_, "FALSE"},
	{NONE_, "NONE"},
	{NULL_, "NULL"}
};
unordered_map<string, Type> tokenType = {
	{"(", LPAREN},
	{")", RPAREN},
	{"[", LSQUARE},
	{"]", RSQUARE},
	{"{", LBRACE},
	{"}", RBRACE},
	{"=", EQUALS},
	{"+", PLUS},
	{"-", MINUS},
	{"*", STAR},
	{"/", SLASH},
	{"%", PCT},
	{"^", CARET},
	{"&", AMP},
	{"|", PIPE},
	{"~", TILDE},
	{"!", EXCL},
	{"^|", CARET_PIPE},
	{"**", STAR_STAR},
	{"//", SLASH_SLASH},
	{"<<", LT_LT},
	{">>", GT_GT},
	{"<-", L_ARROW},
	{"->", R_ARROW},
	{":=", COLON_EQUALS},
	{".", DOT},
	{",", COMMA},
	{":", COLON},
	{";", SEMICOLON},
	{"?", QUESTION},
	{"#", POUND},
	{"$", DOLLAR},
	{"\"", QUOTE},
	{"'", APOSTROPHE},
	{"\\", BACKSLASH},
	{"`", BACKTICK},
	{"_", UNDERSCORE},
	{"BOF", BOF_},
	{"EOF", EOF_}
};
unordered_map<Type, string> typeLexeme = {
	{LPAREN, "LPAREN"},
	{RPAREN, "RPAREN"},
	{LSQUARE, "LSQUARE"},
	{RSQUARE, "RSQUARE"},
	{LBRACE, "LBRACE"},
	{RBRACE, "RBRACE"},
	{EQUALS, "EQUALS"},
	{PLUS, "PLUS"},
	{MINUS, "MINUS"},
	{STAR, "STAR"},
	{SLASH, "SLASH"},
	{PCT, "PCT"},
	{CARET, "CARET"},
	{AMP, "AMP"},
	{PIPE, "PIPE"},
	{TILDE, "TILDE"},
	{EXCL, "EXCL"},
	{CARET_PIPE, "CARET_PIPE"},
	{STAR_STAR, "STAR_STAR"},
	{SLASH_SLASH, "SLASH_SLASH"},
	{LT_LT, "LT_LT"},
	{GT_GT, "GT_GT"},
	{L_ARROW, "L_ARROW"},
	{R_ARROW, "R_ARROW"},
	{COLON_EQUALS, "COLON_EQUALS"},
	{DOT, "DOT"},
	{COMMA, "COMMA"},
	{COLON, "COLON"},
	{SEMICOLON, "SEMICOLON"},
	{QUESTION, "QUESTION"},
	{POUND, "POUND"},
	{DOLLAR, "DOLLAR"},
	{QUOTE, "QUOTE"},
	{APOSTROPHE, "APOSTROPHE"},
	{BACKSLASH, "BACKSLASH"},
	{BACKTICK, "BACKTICK"},
	{UNDERSCORE, "UNDERSCORE"},
	{BOF_, "BOF_"},
	{EOF_, "EOF_"}
};
// unordered_map<char, Type> charType = {
// 	{charTypes}
// };

std::regex whitespace_regex("^\\s+");
std::regex num_regex("^(\\d*\\.?\\d+(i(?![a-zA-Z]))?)");
std::regex id_regex("^(x|y|sin|sinh|asin|arcsin|cos|cosh|acos|arccos|integrate)");
std::regex token_regex("^\\(|\\)|\\[|\\]|\\{|\\}|=|\\+|-|\\*|/|%|\\^|&|\\||~|!|\\^\\||\\*\\*|//|<<|>>|<-|->|:=|\\.|,|:|;|\\?|#|\\$|\\\"|'|\\\\|`|_|BOF|EOF");

// Type getType(char c) {
//     if ('0' <= c && c <= '9')  return NUM;
//     if ('a' <= c && c <= 'z')  return ID;
//     if ('A' <= c && c <= 'Z')  return ID;
//     if (charType.count(c) > 0) return charType[c];

//     switch(c){
//         case ' ':
//             return WHITESPACE;
//         case '\t':
//             return WHITESPACE;
//         case '\n':
//             return WHITESPACE;
//         case '\r':
//             return WHITESPACE;
//         default:
//             return NONE;
//     }
// }

std::string getTypeString(const Type& type) {
    if (keywordLexeme.count(type) > 0) return keywordLexeme[type];
    if (typeLexeme.count(type) > 0)    return typeLexeme[type];
    if (type == ID)                    return "ID";
    if (type == STR)                   return "STR";
    if (type == NUM)                   return "NUM";
    if (type == WHITESPACE)            return "WHITESPACE";

    return "NONE";
}

bool scan(const std::string& str, std::list<Token>& tokens) {
    if (str.empty()) return true;

    std::smatch match;
    if (std::regex_search(str, match, whitespace_regex)){
        return scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, num_regex)){
        int longest_at = 0;
        for(int i = 1; i < match.size(); ++i){
            if (match[i].length() > match[longest_at].length()){
                longest_at = i;
            }
        }
        tokens.emplace_back(Token{match[longest_at], NUM});
        return scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, id_regex)){
        int longest_at = 0;
        for(int i = 1; i < match.size(); ++i){
            if (match[i].length() > match[longest_at].length()){
                longest_at = i;
            }
        }
        tokens.emplace_back(Token{match[longest_at], ID});
        return scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, token_regex)){
        int longest_at = 0;
        for(int i = 1; i < match.size(); ++i){
            if (match[i].length() > match[longest_at].length()){
                longest_at = i;
            }
        }
        tokens.emplace_back(Token{match[longest_at], tokenType[match[longest_at]]});
        return scan(match.suffix(), tokens);
    }
    return false;
}



ostream& print(ostream& out, list<Token> tokens, const string& delimiter, const bool& printType) {
    bool first = true;
    for (auto& token : tokens) {
        if (first) {
            first = false;
        } else {
            out << delimiter;
        }
        out << token.lexeme;
        if (printType){
            out << "  " << getTypeString(token.type);
        }
    }
    return out;
}
