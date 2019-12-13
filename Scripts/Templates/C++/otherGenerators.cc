
#include "../codeGenerator.h"
#include <cassert>

using namespace std;

{generate_function_definitions}

map<string, GenerateFunction> generate_{root}_map = {
    {generate_functions}
};

GenerateFunction get_{root}_code_generation_function(ParseTree* tree){
    NonTerminal* nt = tree->getNonTerminal();
    return generate_{root}_map[nt->getRule()];
}
        