
import os
import textwrap
import yaml
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

special_characters = set(("^", "$", ".", "|", "?", "*", "+", "(", ")", "[", "]", "{", "}"))

def generateScannerCPlusPlus(infile, verbose=False):
    relpath = os.path.dirname(os.path.abspath(__file__))
    relpath = os.path.join(relpath, "Templates", "C++")

    with open(infile) as file:
        specs = yaml.load(file, Loader=Loader)

    if not os.path.isdir("Scanner/"):
        os.mkdir("Scanner/")


    def wrap(text, width=80, indent="    "):
        return (os.linesep + indent).join(textwrap.fill(text, width).split(os.linesep))


    with open(os.path.join(relpath, "scanner.h")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    types = [str(key) for token in specs["tokens"] for key in token] + \
            [str(key) for keyword in specs["keywords"] for key in keyword] + \
            ["WHITESPACE", "NONE"]

    template = template.replace("{types}", wrap(", ".join(types), indent=" "*8))

    with open(os.path.join("Scanner", "scanner.h"), "w") as file:
        file.write(template)

    
    with open(os.path.join(relpath, "scanner.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    keywordType = {keyword[key]['lexeme']: key for keyword in specs["keywords"] for key in keyword}

    keywordLexeme = {v:k for k, v in keywordType.items()}
    # template = template.replace(
    #     "{keywordLexemes}",
    #     wrap(", ".join(["{{{k}, \"{v}\"}}".format(k=k, v=v.upper()) for k, v in keywordLexeme.items()]))
    # )

    tokenType = {token[key]['lexeme'].replace("\"", "\\\""):key for token in specs["tokens"] for key in token if "lexeme" in token[key]}    
    template = template.replace(
        "{tokenTypes}",
        ",\n    ".join(['{{"{k}", {v}}}'.format(k=k, v=v) for k, v in tokenType.items()])
    )

    typeLexeme = {v:k for k, v in tokenType.items()}
    template = template.replace(
        "{typeLexemes}",
        ",\n    ".join(
            ['{{{k}, "{k}"}}'.format(k=k, v=v) for k, v in typeLexeme.items()] + \
            ["{{{k}, \"{v}\"}}".format(k=k, v=v.upper()) for k, v in keywordLexeme.items()]
        )
    )

    template = template.replace("{type_cases}", "\n        ".join(
        'case {t}: return "{t}";'.format(t=t)
        for t in types
    ))

    num_regex = specs.get("num_regex", "\\d*\\.?\\d+").replace("\\", "\\\\")
    if not num_regex.startswith("^"):
        num_regex = "^(" + num_regex + ")"

    template = template.replace("{num_regex}", num_regex)

    id_regex = specs.get("id_regex", "[a-zA-Z]+[0-9a-zA-Z]*")
    if isinstance(id_regex, list):
        id_regex.sort(key=len, reverse=True)
        id_regex = "|".join(map(str, id_regex))

    if not id_regex.startswith("^"):
        id_regex = "^(" + id_regex + ")"

    template = template.replace("{id_regex}", id_regex)

    token_regex = "^" + "|".join(
        "".join("\\%s" % c if c in special_characters else c for c in list(token))
        for token in sorted(tokenType.keys(), key=len, reverse=True)
    ).replace("\\", "\\\\").replace("\"", "\\\"")
    template = template.replace("{token_regex}", token_regex)

    


    with open(os.path.join("Scanner", "scanner.cc"), "w") as file:
        file.write(template)



if __name__ == "__main__":
    generateScannerCPlusPlus("./LanguageSpecification.yml", verbose=True)