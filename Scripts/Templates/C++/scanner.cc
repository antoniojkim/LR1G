
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
	{keywordLexemes}
};
unordered_map<string, Type> tokenType = {
	{tokenTypes}
};
unordered_map<Type, string> typeLexeme = {
	{typeLexemes}
};
// unordered_map<char, Type> charType = {
// 	{charTypes}
// };

std::regex whitespace_regex("^\\s+");
std::regex num_regex("{num_regex}");
std::regex id_regex("{id_regex}");
std::regex token_regex("{token_regex}");

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
        for(unsigned int i = 1; i < match.size(); ++i){
            if (match[i].length() > match[longest_at].length()){
                longest_at = i;
            }
        }
        tokens.emplace_back(Token{match[longest_at], NUM});
        return scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, id_regex)){
        int longest_at = 0;
        for(unsigned int i = 1; i < match.size(); ++i){
            if (match[i].length() > match[longest_at].length()){
                longest_at = i;
            }
        }
        tokens.emplace_back(Token{match[longest_at], ID});
        return scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, token_regex)){
        int longest_at = 0;
        for(unsigned int i = 1; i < match.size(); ++i){
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
    