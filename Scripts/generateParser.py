
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


    with open(os.path.join(relpath, "transitions.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    def wrap(text, width=80, indent="    "):
        return (os.linesep + indent).join(textwrap.fill(text, width).split(os.linesep))

    # template = template.replace("{terminal_enums}", wrap(", ".join(terminals)))
    template = template.replace("{terminals}", wrap(", ".join('"%s"' % t for t in terminals)))

    # template = template.replace("{nonterminal_enums}", wrap(", ".join(nonterminals)))
    template = template.replace("{nonterminals}", wrap(", ".join('"%s"' % t for t in nonterminals)))
    
    # template = template.replace("{rule_enums}", wrap(", ".join(sorted("_".join(rule) for rule in rules))))
    template = template.replace("{rules}", ",\n\t".join(
        map(lambda rule: "{%s}" % ", ".join('"%s"' % r for r in rule), rules)
    ))

    states = set(state for state, values in transitions.items())

    template = template.replace("{transitions}", ",\n\t".join(
        "/* State {state} */ {{{values}}}".format( 
            state=state,
            values=", ".join(
                "{{\"{symbol}\", {{{reduce}, {next_state}}}}}".format(
                    symbol=symbol,
                    reduce=int(reduce),
                    next_state=next_state
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