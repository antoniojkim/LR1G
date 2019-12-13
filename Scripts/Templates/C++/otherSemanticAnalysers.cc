
#include "../semanticAnalyser.h"
#include <cassert>

using namespace std;

{analysis_function_definitions}

map<string, AnalysisFunction> analyse_{root}_map = {
    {analysis_functions}
};

AnalysisFunction get_{root}_semantic_analysis_function(ParseTree* tree){
    NonTerminal* nt = tree->getNonTerminal();
    return analyse_{root}_map[nt->getRule()];
}
        