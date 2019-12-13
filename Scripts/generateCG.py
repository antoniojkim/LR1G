
import os
import yaml
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

def generateCG_CPlusPlus(infile, verbose=False):
    relpath = os.path.dirname(os.path.abspath(__file__))
    relpath = os.path.join(relpath, "Templates", "C++")

    with open(infile) as file:
        specs = yaml.load(file, Loader=Loader)

    if not os.path.isdir("CodeGenerator"):
        os.mkdir("CodeGenerator")
    
    if not os.path.isdir(os.path.join("CodeGenerator", "Generators")):
        os.mkdir(os.path.join("CodeGenerator", "Generators"))

    productions = {}
    for rule in specs["productionRules"]:
        rule = rule.split(" ")
        if len(rule) > 1:
            if rule[0] not in productions:
                productions[rule[0]] = []
            
            productions[rule[0]].append(rule[1:])

    with open(os.path.join(relpath, "codeGenerator.h")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    template = template.replace(
        "{functions}",
        os.linesep.join("GenerateFunction get_{root}_code_generation_function(ParseTree* tree);".format(root=root)
                        for root in productions)
    )

    with open(os.path.join("CodeGenerator", "codeGenerator.h"), "w") as file:
        file.write(template)


    with open(os.path.join(relpath, "codeGenerator.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)
    
    template = template.replace(
        "{generateFunctions}",
        ",\n    ".join(
            "{{\"{root}\", get_{root}_code_generation_function}}".format(root=root)
            for root in productions
        )
    )

    with open(os.path.join("CodeGenerator", "codeGenerator.cc"), "w") as file:
        file.write(template)

    
    with open(os.path.join(relpath, "otherGenerators.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)
    
    for root in productions:
        filepath = os.path.join("CodeGenerator", "Generators", "{root}Generator.cc".format(root=root))

        existingImplementations = {}
        if os.path.isfile(filepath):
            with open(filepath) as file:
                for row in file:
                    if row.startswith("ostream& generate_"):
                        fname = row.strip().split(" ")[1].split("(")[0]
                        paren_count = 0
                        brace_count = 0
                        implementation = ""
                        while True:
                            for c in list(row):
                                if (brace_count > 0):
                                    implementation += c
                                if c == "{":
                                    brace_count += 1
                                elif c == "}":
                                    brace_count -= 1
                                elif c == "(":
                                    paren_count += 1
                                elif c == ")":
                                    paren_count -= 1
                            if (paren_count > 0 or brace_count > 0):
                                row = next(file)
                            else:
                                break

                        existingImplementations[fname] = implementation.strip()

        functions = []
        for rules in productions[root]:
            fname = "generate_{root}_{rules}".format(root=root, rules='_'.join(rules))
            if fname in existingImplementations:
                impl = existingImplementations[fname]
            else:
                impl = os.linesep.join((
                    'assert(tree->getParams()->rule == "{root} {rules}");'.format(
                        root=root,
                        rules=" ".join(rules)
                    ),
                    '    throw "{fname} not implemented";\n}}'.format(fname=fname)
                ))

            functions.append(os.linesep.join((
                "ostream& {fname}(ContextSensitiveTree* tree, ostream& out, const string& indent){{".format(fname=fname),
                "    " + impl
            )))

        otherGenerator = template.replace(
            "{generate_function_definitions}",
            os.linesep.join(functions)
        )

        otherGenerator = otherGenerator.replace(
            "{generate_functions}",
            ",\n    ".join(
                "{{\"{root} {rules1}\", generate_{root}_{rules2}}}".format(
                    root=root,
                    rules1=' '.join(rules),
                    rules2='_'.join(rules)
                )
                for rules in productions[root]
            )
        )

        otherGenerator = otherGenerator.replace("{root}", root)

        with open(filepath, "w") as file:
            file.write(otherGenerator)



if __name__ == "__main__":
    generateCG_CPlusPlus("LanguageSpecification.yml", verbose=True)
