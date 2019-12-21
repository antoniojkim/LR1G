
import os
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


    with open(os.path.join(relpath, "scanner.h")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    template = template.replace("{types}", ",\n\t".join([
            str(key) for token in specs["tokens"] for key in token
        ]+[
            str(key) for keyword in specs["keywords"] for key in keyword
        ]+[
            "WHITESPACE",
            "NONE"
        ])
    )

    with open(os.path.join("Scanner", "scanner.h"), "w") as file:
        file.write(template)

    
    with open(os.path.join(relpath, "scanner.cc")) as file:
        template = os.linesep.join(row.rstrip() for row in file)

    keywordType = {keyword[key]['lexeme']: key for keyword in specs["keywords"] for key in keyword}
    # template = template.replace(
    #     "{keywordTypes}",
    #     ",\n\t".join(["{{\"{k}\", {v}}}".format(k=k, v=v) for k, v in keywordType.items()])
    # )

    keywordLexeme = {v:k for k, v in keywordType.items()}
    template = template.replace(
        "{keywordLexemes}",
        ",\n\t".join(["{{{k}, \"{v}\"}}".format(k=k, v=v.upper()) for k, v in keywordLexeme.items()])
    )

    tokenType = {token[key]['lexeme'].replace("\"", "\\\""):key for token in specs["tokens"] for key in token if "lexeme" in token[key]}    
    template = template.replace(
        "{tokenTypes}",
        ",\n\t".join(["{{\"{k}\", {v}}}".format(k=k, v=v) for k, v in tokenType.items()])
    )

    typeLexeme = {v:k for k, v in tokenType.items()}
    template = template.replace(
        "{typeLexemes}",
        ",\n\t".join(["{{{k}, \"{k}\"}}".format(k=k, v=v) for k, v in typeLexeme.items()])
    )

    # charType = {k.replace("'", "\\'"):v for k, v in tokenType.items() if len(k) == 1 or k == "\\\""}
    # template = template.replace(
    #     "{charTypes}",
    #     ",\n\t".join(["{{'{k}', {v}}}".format(k=k, v=v) for k, v in charType.items()])
    # )

    # template = template.replace(
    #     "{current_transitions}",
    #     "".join("""
    #     }} else if (current == {current} && type == {type}) {{ /*  {k}  */
    #         current = {new_current};""".format(
    #         current=tokenType[k[:-1]],
    #         type=tokenType[k[-1:]],
    #         k=k,
    #         new_current=tokenType[k]
    #     )
    #     for k, v in tokenType.items()
    #     if len(k) in (2, 3) and k[:-1] in tokenType and k[-1:] in tokenType)
    # )

    num_regex = specs.get("num_regex", "\\d*\\.?\\d+").replace("\\", "\\\\")
    if not num_regex.startswith("^"):
        num_regex = "^(" + num_regex + ")"

    template = template.replace("{num_regex}", num_regex)

    id_regex = specs.get("id_regex", "[a-zA-Z]+[0-9a-zA-Z]*")
    if isinstance(id_regex, list):
        id_regex.sort(key=len)
        id_regex = "|".join(map(str, id_regex))

    if not id_regex.startswith("^"):
        id_regex = "^(" + id_regex + ")"

    template = template.replace("{id_regex}", id_regex)

    token_regex = "^" + "|".join(map(
        lambda token: "".join("\\%s" % c if c in special_characters else c for c in list(token)),
        sorted(tokenType.keys(), key=len)
    )).replace("\\", "\\\\").replace("\"", "\\\"")
    template = template.replace("{token_regex}", token_regex)

    


    with open(os.path.join("Scanner", "scanner.cc"), "w") as file:
        file.write(template)



if __name__ == "__main__":
    generateScannerCPlusPlus("./LanguageSpecification.yml", verbose=True)