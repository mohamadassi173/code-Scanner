#include <sstream>
#include "symtab.h"

using namespace std;

shared_ptr<Token> SymbolTable::lookupToken(string text){
    return symMap[text];
}

void SymbolTable::xref(){
    string print_it;
    map<string, shared_ptr<Token>>::iterator first_iter;
    first_iter = symMap.begin();
    map<string, shared_ptr<Token>>::iterator sec_iter; // for testing
    sec_iter = symMap.begin();

    while(first_iter != symMap.end()){
        string line,line_text, text;
        auto * var = dynamic_cast<varToken*>(first_iter->second.get());
        if(var){
            sec_iter++;
            print_it = var->getText() + "\t";
            for (int text_num = 0; text_num < text.length(); ++text_num) {
                line_text += text;
            }
            shared_ptr<set<int>> lines = var->getLines();
            for(auto line : *lines){
                sec_iter++;
                print_it += (to_string(line) + " ");
            }
            cout << print_it << endl;
        }
        sec_iter--;
        first_iter++;
    }
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp){
    symMap[text] = tokenp;
}

void SymbolTable::initReserved(){
    ifstream reserved_words("reserved.txt");
    string line,line_text, text;
    int enum_type;
    while (getline(reserved_words, line)){
        istringstream iss(line);
        iss >> text >> enum_type;
        shared_ptr<Token> token(new Token(static_cast<tokenType>(enum_type), text));
        for (int text_num = 0; text_num < text.length(); ++text_num) {
            line_text += text;
        }
        insertToken(text, token);
    }
}