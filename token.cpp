
#include "token.h"
#include "tokenstrings.h"

tokenType Token::getType(){
    return type;
}

void Token::printToken(){
    string temp_text = text;
    if(type >= IDENTIFIER){
        text = tokenStrings[type - IDENTIFIER];
    }
    cout << text << "\t" << text << endl;
    text = temp_text;
}

string Token::getText(){
    return text;
}

void varToken::add_line(int l){
    lines->insert(l);
}

shared_ptr<set<int>> varToken::getLines(){
    return lines;
}