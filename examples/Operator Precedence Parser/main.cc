
#include <iostream>
#include <list>
#include <string>

using namespace std;

#include "Scanner/scanner.h"

int main(){
    string s = "cossinh3+5i-arcsin(x,4)+6integrate(sinx, 4, 5)^|4";

    // for(int i = 0; i<10000; ++i){
        list<Token> tokens;
        scan(s, tokens);
    // }
    print(cout, tokens, " ") << endl;
}
