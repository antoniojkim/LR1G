
import os
import textwrap
import yaml
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper


def generateParserFromLR1_CPlusPlus(lr1_file, verbose=False):
    relpath = os.path.dirname(os.path.abspath(__file__))
    relpath = os.path.join(relpath, "Templates", "C++")

    if not os.path.isdir("Parser/"):
        os.mkdir("Parser/")

    with open(lr1_file) as file:
        lr1 = [row.strip() for row in file]
    
    lr1 = iter(lr1)

    num_terminals = int(next(lr1))
    terminals = [next(lr1) for i in range(num_terminals)]
    terminals.sort()

    num_nonterminals = int(next(lr1))
    nonterminals = [next(lr1) for i in range(num_nonterminals)]
    nonterminals.sort()

    start = next(lr1)

    nonterminals = [start] + nonterminals

    num_rules = int(next(lr1))
    rules = [next(lr1).split() for i in range(num_rules)]

    num_states = int(next(lr1))
    num_transitions = int(next(lr1))
    transitions = {}
    for i in range(num_transitions):
        transition = next(lr1).split()
        state = int(transition[0])
        symbol = transition[1]
        action = transition[2]
        nextStateOrRule = int(transition[3])

        assert(0 <= state < num_states)
        assert symbol in terminals or symbol in nonterminals, f"Invalid Symbol {symbol}"
        assert(action in ("SHIFT", "REDUCE"))
        
        if state not in transitions:
            transitions[state] = {}

        transitions[state][symbol] = (action == "REDUCE", nextStateOrRule)


    with open(os.path.join(relpath, "transitions.h")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    def wrap(text, width=80, indent="\t", delimiter=", "):
        if not isinstance(text, str):
            text = delimiter.join(text)
        return (os.linesep + indent).join(textwrap.fill(text, width).split(os.linesep))

    wrap_quotes = lambda iterable: map('"{}"'.format, iterable)

    template = template.replace("{terminals}", wrap(terminals))
    template = template.replace("{numTerminals}", str(len(terminals)))

    template = template.replace("{nonterminals}", wrap([nonterminals[0] + " = " + str(-len(nonterminals))]+nonterminals[1:]))
    template = template.replace("{numNonTerminals}", str(len(nonterminals)))
    
    maxRuleLength = max(map(len, rules))
    template = template.replace("{numRules}", str(len(rules)))
    template = template.replace("{maxRuleLength}", str(maxRuleLength))
    template = template.replace("{numTransitions}", str(len(transitions)))
    
    with open(os.path.join("Parser", "transitions.h"), "w") as file:
        file.write(template)

    with open(os.path.join(relpath, "transitions.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    template = template.replace("{terminalStrings}", wrap(wrap_quotes(terminals)))
    template = template.replace("{nonTerminalStrings}", wrap(wrap_quotes(nonterminals)))
    template = template.replace("{rules}", ",\n\t".join(
        map(lambda rule: "{%s}" % ", ".join([str(len(rule))]+rule+["-1"]*(maxRuleLength-len(rule))), rules)
    ))

    states = set(state for state, values in transitions.items())

    template = template.replace("{transitions}", ",\n\t".join(
        "/* State {state} */ {{{{{values}}}}}".format( 
            state=state,
            values=", ".join(
                "{{{symbol}, {next_state}}}".format(
                    symbol=symbol,
                    next_state=next_state if reduce else -next_state-1
                )
                for symbol, (reduce, next_state) in transitions.get(state, {}).items()
            )
        )
        for state in range(max(states) + 1)
    ))
    
    with open(os.path.join("Parser", "transitions.cc"), "w") as file:
        file.write(template)


    with open(os.path.join(relpath, "parser.h")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    template = template.replace("// #define INCLUDE_TRANSITIONS", "#define INCLUDE_TRANSITIONS")
    
    with open(os.path.join("Parser", "parser.h"), "w") as file:
        file.write(template)



if __name__ == "__main__":
    generateParserFromLR1_CPlusPlus("./LanguageSpecification.lr1", verbose=True)