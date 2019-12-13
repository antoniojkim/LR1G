
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <iostream>
#include <string>
#include "../ContextSensitiveTrees/ContextSensitiveTrees.h"
#include "../Parser/parsetree.h"

typedef std::ostream& (*GenerateFunction)(ContextSensitiveTree* tree, std::ostream& out, const std::string& indent);
typedef GenerateFunction (*GetGenerateFunction)(ParseTree* tree);

GenerateFunction getCodeGenerationFunction(ParseTree* tree);
 
{functions}


#endif // CODE_GENERATOR_H
    
