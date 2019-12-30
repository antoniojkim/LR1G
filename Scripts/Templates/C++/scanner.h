
#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <list>
#include <string>
    
namespace Scanner {

    enum Type {
        {types}
    };

    std::string getTypeString(const Type& type);

    struct Token {
        std::string lexeme;
        Type type;
    };

    bool scan(const std::string& str, std::list<Token>& tokens);
    // void scan(std::istream& in, std::list<Token>& tokens);

    std::ostream& print(std::ostream& out, std::list<Token> tokens, const std::string& delimiter="", const bool& printType=false);
    std::string join(std::list<Token> tokens, const std::string& delimiter="", const bool& printType=false);

}

#endif
    