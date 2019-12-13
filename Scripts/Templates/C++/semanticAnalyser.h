
#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <iostream>
#include <string>

#include "../ContextSensitiveTrees/ContextSensitiveTrees.h"
#include "../Parser/parsetree.h"

typedef std::ostream& (*AnalysisFunction)(ContextSensitiveTree* tree, std::ostream& out, const std::string& indent);
typedef AnalysisFunction (*GetAnalysisFunction)(ParseTree* tree);

AnalysisFunction getSemanticAnalysisFunction(ParseTree* tree);
    
{analysis_functions}


#endif // SEMANTIC_ANALYSIS_H
    
