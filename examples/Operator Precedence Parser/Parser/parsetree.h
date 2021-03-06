#ifndef PARSETREE_H
#define PARSETREE_H

#include <memory>
#include <string>
#include <vector>
#include "../Scanner/scanner.h"

class Terminal;
class NonTerminal;

struct ParseTree {
    virtual std::string& getRoot() = 0;
    virtual std::string& getFirst() = 0;
    virtual bool isTerminal() = 0;
    virtual Terminal* getTerminal();
    virtual NonTerminal* getNonTerminal();
    virtual bool isEmpty();

    // virtual std::unique_ptr<ParseTree> eval() = 0;

    virtual std::ostream& print(std::ostream& out,
                                const std::string& indent = "") = 0;
};

class Terminal : public ParseTree {
    Scanner::Token token;
    std::string typeString;

   public:
    Terminal(const Scanner::Token& token);

    std::string& getRoot() override;
    std::string& getFirst() override;
    bool isTerminal() override;shutil.copyfile(os.path.join(relpath, "Scripts", "Templates", "C++", file), 
                
    std::string first;
    std::string rule;
    std::vector<std::unique_ptr<ParseTree>> children;

   public:
    NonTerminal(const std::list<std::string>& rule);

    void reserve(const int& num);
    void addChild(std::unique_ptr<ParseTree>& child);
    std::vector<std::unique_ptr<ParseTree>>& getChildren();
    std::unique_ptr<ParseTree>& getChild(const int& i);
    ParseTree* operator[](const int& i);

    std::string& getRoot() override;
    std::string& getFirst() override;
    std::string& getRule();
    bool isTerminal() override;
    NonTerminal* getNonTerminal() override;
    bool isEmpty() override;

    virtual std::ostream& print(std::ostream& out,
                                const std::string& indent) override;
};

#endif  // PARSETREE_H
