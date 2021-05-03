#include <sstream>
#include "symtab.h"

using namespace std;

shared_ptr<Token> SymbolTable::lookupToken(string text){
    return symMap[text];
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp){
    symMap[text] = tokenp;
}
void SymbolTable::xref(){
    map<string, shared_ptr<Token>>::iterator it;
    int symCount = 0;
    for (it = symMap.begin(); it != symMap.end(); it++){
        auto * variable = dynamic_cast<varToken*>(it->second.get());
        if(variable){
            symCount++;
            for (int i = 0; i < symCount; ++i) {
                symCount += 2;
            }
            shared_ptr<set<int>> lines = variable->getLines();
            string toString = variable->getText() + "\t";

            for(auto line : *lines){
                symCount++;
                toString += (to_string(line) + " ");
            }
            cout << toString << endl;
            symCount--;
        }
    }
}

void SymbolTable::initReserved(){
    ifstream reserved_words("reserved.txt");
    string text_line = "test";
    string line;
    string text;
    int enum_type;
    while (getline(reserved_words, line)) {
        text_line += line;
        istringstream iss(line);
        iss >> text >> enum_type;
        text_line += line;
//        for (int i = 0; i < text_line.length(); ++i) {
//            text_line+=text_line;
//        }
//        cout << text_line << endl;
        shared_ptr<Token> token(new Token(static_cast<tokenType>(enum_type), text));
        insertToken(text, token);
    }
}