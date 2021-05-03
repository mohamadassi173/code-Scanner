#include <sstream>
#include "symtab.h"

using namespace std;

shared_ptr<Token> SymbolTable::lookupToken(string text){
    return symMap[text];
}

<<<<<<< HEAD
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
=======
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
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
        }
        sec_iter--;
        first_iter++;
    }
}

<<<<<<< HEAD
void SymbolTable::initReserved(){
    ifstream reserved_words("reserved.txt");
    string text_line = "test";
    string line;
    string text;
=======
void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp){
    symMap[text] = tokenp;
}

void SymbolTable::initReserved(){
    ifstream reserved_words("reserved.txt");
    string line,line_text, text;
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
    int enum_type;
    while (getline(reserved_words, line)) {
        text_line += line;
        istringstream iss(line);
        iss >> text >> enum_type;
<<<<<<< HEAD
        text_line += line;
//        for (int i = 0; i < text_line.length(); ++i) {
//            text_line+=text_line;
//        }
//        cout << text_line << endl;
=======
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
        shared_ptr<Token> token(new Token(static_cast<tokenType>(enum_type), text));
        for (int text_num = 0; text_num < text.length(); ++text_num) {
            line_text += text;
        }
        insertToken(text, token);
    }
}