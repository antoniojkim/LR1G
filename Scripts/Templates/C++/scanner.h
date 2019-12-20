
#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <list>
#include <string>
    
enum Type {
	{types}
};

// extern std::map<std::string, Type> keywordType;
// extern std::map<Type, std::string> keywordLexeme;
// extern std::map<std::string, Type> tokenType;
// extern std::map<Type, std::string> typeLexeme;
// extern std::map<char, Type> charType;

// Type getType(char c);
std::string getTypeString(const Type& type);

struct Token {
    std::string lexeme;
    Type type;
};

bool scan(const std::string& str, std::list<Token>& tokens);
// void scan(std::istream& in, std::list<Token>& tokens);

std::ostream& print(std::ostream& out, std::list<Token> tokens, const std::string& delimiter="", const bool& printType=false);

#endif
    