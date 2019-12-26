
#include "parser.h"
#include "../Scanner/scanner.h"
#include "StringUtils.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using namespace Scanner;


unordered_set<string> terminals({
	{terminals}
});

unordered_set<string> nonterminals({
	{nonterminals}
});

vector<list<string>> rules ({
	{rules}
});

unordered_map<int, unordered_map<string, pair<bool, int>>> transitions({
	{transitions}
});


template <class T>
T getNext(const string& str) {
    istringstream iss{str};
    T i;
    if (!(iss >> i)) throw "Failed to get next from string";
    return i;
}

template <class T>
T getNext(istringstream& iss) {
    T i;
    if (!(iss >> i)) throw "Failed to get next from istringstream";
    return i;
}

Parser::Parser() {}

unique_ptr<ParseTree> Parser::parse(const string& input,
                                         const bool& showTokens) {
    list<Token> tokens;
    tokens.emplace_back(Token{"BOF_", Type::BOF_});
    scan(input, tokens);
    tokens.emplace_back(Token{"EOF_", Type::EOF_});
    if (showTokens) {
        print(cout, tokens, "\n", true) << endl << endl;
    }
    return parse(tokens);
}

unique_ptr<ParseTree> Parser::parse(list<Token>& tokens) {
    // int currentState = 0;
    list<unique_ptr<ParseTree>> symbolStack;
    list<int> stateStack;
    stateStack.emplace_back(0);

    for (auto& token : tokens) {
        string typeString = getTypeString(token.type);
        while (transitions[stateStack.back()].count(typeString) > 0 &&
               transitions[stateStack.back()][typeString].first == true) {
            int rule = transitions[stateStack.back()][typeString].second;

            NonTerminal* nt = new NonTerminal(rules[rule]);
            nt->reserve(rules[rule].size() - 1);

            for (size_t j = 1; j < rules[rule].size(); ++j) {
                nt->addChild(symbolStack.back());
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            reverse(nt->getChildren().begin(), nt->getChildren().end());
            unique_ptr<ParseTree> tree{nt};
            symbolStack.emplace_back(move(tree));
            stateStack.emplace_back(
                transitions[stateStack.back()][rules[rule].front()].second);
        }

        symbolStack.emplace_back(make_unique<Terminal>(token));

        if (transitions[stateStack.back()].count(
                symbolStack.back()->getRoot()) == 0) {
            ostringstream oss;
            oss << "ERROR at \"" << token.lexeme << "\"" << endl
                << "State Stack: " << endl;
            for (auto& state : stateStack) {
                oss << "    " << state << endl;
            }
            throw oss.str();
        }

        stateStack.emplace_back(
            transitions[stateStack.back()][symbolStack.back()->getRoot()]
                .second);
    }

    // cout << symbolStack.size() << endl;
    // for (auto& s : symbolStack){
    //     cout << s->getRoot() << endl;
    // }
    symbolStack.pop_back();

    unique_ptr<ParseTree> tree = move(symbolStack.back());

    return tree;
}

string actions[2] = {"SHIFT", "REDUCE"};
ostream& operator<<(ostream& out, Parser& parser) {
    out << "Terminals:" << endl;
    for (auto& terminal : terminals) {
        out << "    " << terminal << endl;
    }
    out << "Non Terminals:" << endl;
    for (auto& nonterminal : nonterminals) {
        out << "    " << nonterminal << endl;
    }
    out << "Rules:" << endl;
    for (auto& rule : rules) {
        out << "    " << join(rule, ' ') << endl;
    }
    out << "Transitions:" << endl;
    for (auto& state : transitions) {
        for (auto& symbol : state.second) {
            out << "    " << state.first << " " << symbol.first << " "
                << actions[symbol.second.first] << " " << symbol.second.second
                << endl;
        }
    }
    return out;
}
