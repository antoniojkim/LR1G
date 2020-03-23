#ifndef __TRANSITIONS_H__
#define __TRANSITIONS_H__

#include <array>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "../Scanner/scanner.h"

enum Terminal: int {
    {terminals}
};
constexpr int numTerminals = {numTerminals};

enum NonTerminal: int {
    {nonterminals}
};
constexpr int numNonTerminals = {numNonTerminals};

std::string getTerminalString(Terminal terminal);
std::string getNonTerminalString(NonTerminal nonterminal);

int convertTokenType(Scanner::Type type);

constexpr int numRules = {numRules};
constexpr int maxRuleLength = {maxRuleLength}+1;
extern const std::array<std::array<int, maxRuleLength>, numRules> rules;

constexpr int numTransitions = {numTransitions};
extern const std::vector<std::map<int, int>> transitions;

#endif // __TRANSITIONS_H__
