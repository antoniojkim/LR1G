
#include "codeGenerator.h"
#include <map>

using namespace std;
    

map<string, GetGenerateFunction> generateFunctionsMap = {
    {generateFunctions}
};

GenerateFunction getCodeGenerationFunction(ParseTree* tree){
    return generateFunctionsMap[tree->getRoot()](tree);
}
    