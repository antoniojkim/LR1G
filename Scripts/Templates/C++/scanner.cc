
#include "scanner.h"
#include <sstream>

using namespace std;
    
map<string, Type> keywordType = {
	{keywordTypes}
};
map<Type, string> keywordLexeme = {
	{keywordLexemes}
};
map<string, Type> tokenType = {
	{tokenTypes}
};
map<Type, string> typeLexeme = {
	{typeLexemes}
};
map<char, Type> charType = {
	{charTypes}
};

Type getType(char c) {
    if ('0' <= c && c <= '9')  return NUM;
    if ('a' <= c && c <= 'z')  return ID;
    if ('A' <= c && c <= 'Z')  return ID;
    if (charType.count(c) > 0) return charType[c];

    switch(c){
        case ' ':
            return WHITESPACE;
        case '\t':
            return WHITESPACE;
        case '\n':
            return WHITESPACE;
        case '\r':
            return WHITESPACE;
        default:
            return NONE;
    }
}

std::string getTypeString(const Type& type) {
    if (keywordLexeme.count(type) > 0) return keywordLexeme[type];
    if (typeLexeme.count(type) > 0)    return typeLexeme[type];
    if (type == ID)                    return "ID";
    if (type == STR)                   return "STR";
    if (type == NUM)                   return "NUM";
    if (type == WHITESPACE)            return "WHITESPACE";

    return "NONE";
} 

void scan(const std::string& str, std::list<Token>& tokens) {
    istringstream iss{str};
    scan(iss, tokens);
}
    
void scan(std::istream& in, std::list<Token>& tokens) {
    char c;
    ostringstream token;
    bool openQuote = false;
    Type current = NONE;
    while (in.get(c)) {
        Type type = getType(c);
        // cout << token.str() << " " << getTypeString(current) << "    " << c << " " << getTypeString(type) << endl;
        if (current == NONE || current == type) {
            current = type;
        } else if ((current == ID && (type == NUM || type == UNDERSCORE)) ||
                   (current == UNDERSCORE && (type == ID || type == NUM))) {
            current = ID;
        } else if ((current == NUM && type == DOT) || 
                   (current == NUM && type == ID && (c == 'e' || c == 'E'))) {
            current = NUM;
        } else if (openQuote && 
                    ((current == STR && type == ID) ||
                     (current == NUM && type == ID) ||
                     (current == ID && type == WHITESPACE) ||
                     (current == STR && type == WHITESPACE))) {
            current = STR;
	    
        {current_transitions}

        } else {
            if (!token.str().empty() && current != WHITESPACE) {
                auto str = token.str();
                if (current == ID && keywordType.count(str) > 0){
                    tokens.emplace_back(Token{str, keywordType[str]});
                } else {
                    tokens.emplace_back(Token{str, current});
                }
                if (current == QUOTE){
                    openQuote = !openQuote;
                }
                token = ostringstream();
            }
            current = type;
        }
        if (current != WHITESPACE){
            token << c;
        }
    }
    if (!token.str().empty()) {
        tokens.emplace_back(Token{token.str(), current});
    }
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
    