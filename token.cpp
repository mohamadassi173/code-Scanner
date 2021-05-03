
#include "token.h"
#include "tokenstrings.h"

tokenType Token::getType(){
    return type;
}

void Token::printToken(){
<<<<<<< HEAD
    string tokenTemp = "";
    string token = text;

    if(type >= IDENTIFIER){
        tokenTemp += token;
        token = tokenStrings[type - IDENTIFIER];
    }
//    cout << tokenTemp << endl;
    cout << token << "\t" << text << endl;
=======
    string temp_text = text;
    if(type >= IDENTIFIER){
        text = tokenStrings[type - IDENTIFIER];
    }
    cout << text << "\t" << text << endl;
    text = temp_text;
}

string Token::getText(){
    return text;
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
}

void varToken::add_line(int l){
    lines->insert(l);
}

shared_ptr<set<int>> varToken::getLines(){
    return lines;
}