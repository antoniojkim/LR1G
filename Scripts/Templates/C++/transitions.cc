
#include <algorithm>
#include <vector>

#include "transitions.h"

const std::array<std::string, numTerminals> terminalStrings = {
	{terminalStrings}
};

const std::array<std::string, numNonTerminals> nonterminalStrings = {
	{nonTerminalStrings}
};

std::string getTerminalString(Terminal terminal){ return terminalStrings[terminal]; }
std::string getNonTerminalString(NonTerminal nonterminal){ return nonterminalStrings[nonterminal + numNonTerminals]; }

int convertTokenType(Scanner::Type type){
	const std::string typeString = Scanner::typeStrings[type];
    auto first = terminalStrings.data();
    auto last = first + numTerminals;
	auto it = std::lower_bound(first, last, typeString);
	if (it != last && !(typeString < *it)){
		return it - first;
	}
	throw "Cannot find " + typeString + " in Terminals";
}

const std::array<std::array<int, maxRuleLength>, numRules> rules = {{
	{rules}
}};

const std::vector<std::map<int, int>> transitions = {
	{transitions}
};
