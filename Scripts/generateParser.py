
import os
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


    with open(os.path.join(relpath, "fullparser.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    template = template.replace("{terminals}", ",\n\t".join(terminals))
    template = template.replace("{nonterminals}", ",\n\t".join(nonterminals))
    template = template.replace("{rules}", ",\n\t".join(
        map(lambda rule: "{%s}" % ", ".join('"%s"' % r for r in rule), rules)
    ))
    template = template.replace("{transitions}", ",\n\t".join(
        "{{{state}, {{{values}}}}}".format( 
            state=state,
            values=", ".join(
                "{{\"{symbol}\", {{{reduce}, {next_state}}}}}".format(
                    symbol=symbol,
                    reduce=str(reduce).lower(),
                    next_state=next_state
                )
                for symbol, (reduce, next_state) in values.items()
            )
        )
        for state, values in transitions.items()
    ))
    
    with open(os.path.join("Parser", "parser.cc"), "w") as file:
        file.write(template)



if __name__ == "__main__":
    generateParserFromLR1_CPlusPlus("./LanguageSpecification.lr1", verbose=True)