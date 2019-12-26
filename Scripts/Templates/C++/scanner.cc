
#include "scanner.h"
#include <sstream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace Scanner;

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
std::regex hex_regex("^(0x[0-9a-fA-f]+)");
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

std::string Scanner::getTypeString(const Type& type) {
    if (keywordLexeme.count(type) > 0) return keywordLexeme[type];
    if (typeLexeme.count(type) > 0)    return typeLexeme[type];
    if (type == ID)                    return "ID";
    if (type == STR)                   return "STR";
    if (type == NUM)                   return "NUM";
    if (type == WHITESPACE)            return "WHITESPACE";

    return "NONE";
} 
    
bool Scanner::scan(const std::string& str, std::list<Token>& tokens) {
    if (str.empty()) return true;

    std::smatch match;
    if (std::regex_search(str, match, whitespace_regex)){
        return Scanner::scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, hex_regex)){
        tokens.emplace_back(Token{match[0], NUM});
        return Scanner::scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, num_regex)){
        tokens.emplace_back(Token{match[0], NUM});
        return Scanner::scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, id_regex)){
        tokens.emplace_back(Token{match[0], ID});
        return Scanner::scan(match.suffix(), tokens);
    }
    if (std::regex_search(str, match, token_regex)){
        tokens.emplace_back(Token{match[0], tokenType[match[0]]});
        return Scanner::scan(match.suffix(), tokens);
    }
    return false;
}


ostream& Scanner::print(ostream& out, list<Token> tokens, const string& delimiter, const bool& printType) {
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

string Scanner::join(list<Token> tokens, const string& delimiter, const bool& printType) {
    ostringstream out;
    bool first = true;
    for (auto& token : tokens) {
        if (first) {
            first = false;
        } else {
            out << delimiter;
        }
        if (printType){
            out << "  " << getTypeString(token.type);
        }
        out << token.lexeme;
    }
    return out.str();
}
    
