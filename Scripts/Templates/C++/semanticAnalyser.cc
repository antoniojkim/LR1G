
#include "./semanticAnalyser.h"
#include <map>

using namespace std;
    

map<string, GetAnalysisFunction> analysisFunctionsMap = {
    {analysis_functions}
};

GenerateFunction getSemanticAnalysisFunction(ParseTree* tree){
    return analysisFunctionsMap[tree->getRoot()](tree);
}
    