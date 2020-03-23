
#include "scanner.h"
#include <sstream>
#include <regex>
#include <unordered_map>

using namespace std;
using namespace Scanner;

const unordered_map<string, Type> tokenType = {
    {tokenTypes}
};

const std::regex whitespace_regex("^\\s+");
const std::regex hex_regex("^(0x[0-9a-fA-f]+)");
const std::regex bin_regex("^(0b[01]+)");
const std::regex num_regex("{num_regex}");
const std::regex id_regex("{id_regex}");
const std::regex token_regex("{token_regex}");
    
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
    if (std::regex_search(str, match, bin_regex)){
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
        tokens.emplace_back(Token{match[0], tokenType.at(match[0])});
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
            out << "  " << typeStrings[token.type];
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
            out << "  " << typeStrings[token.type];
        }
        out << token.lexeme;
    }
    return out.str();
}
    
