
#include "transitions.h"

using namespace std;

#ifndef __TERMINALS__
#define __TERMINALS__

// enum Terminal {
//     {terminal_enums}
// };

const unordered_set<string> terminals ({
    {terminals}
});

#endif // __TERMINALS__


#ifndef __NONTERMINALS__
#define __NONTERMINALS__

// enum NonTerminal {
//     {nonterminal_enums}
// };

const unordered_set<string> nonterminals ({
    {nonterminals}
});

#endif // __NONTERMINALS__

#ifndef __RULES__
#define __RULES__

// enum Rules {
//     {rule_enums}
// };

const vector<list<string>> rules ({
	{rules}
});

#endif // __RULES__

const vector<unordered_map<string, pair<bool, int>>> transitions ({
	{transitions}
});
