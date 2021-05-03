#include <regex>
#include "scanner.h"

<<<<<<< HEAD
shared_ptr<Token> Scanner::nextToken()
{

    while (nextChar())
    {
        char comment;
        if(ch != 13 && ch != ' ' && ch != '\n' && ch != '\t' && ch != '/'){
            break;
        }
        if(ch == '/') nextChar();
        comment = 'a';
        if (ch == '*') {
            char prev;
            while (nextChar()) {
                comment = 's';
                if (ch == '/' && prev == '*') break;
                prev = ch;
            }
            continue;
        }

        if (ch == '/') {
            int curr = lineno;
            while (nextChar()) if (curr != lineno) break;
            continue;
        }

    } // end while

    string str = string(1, ch);
    if (str == "+") {
        nextChar();
        str += ch;
        if(str == "++"){
            return make_shared<Token>(INC_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);
    }

    if (str == "-") {
        nextChar();
        str += ch;
        if (str == "->") {
            return make_shared<Token>(PTR_OP, str);
        }
        if (str == "--") {
            return make_shared<Token>(DEC_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);
    }

    if (str == "&") {
        nextChar();
        str += ch;
        if(str == "&&"){
            return make_shared<Token>(AND_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);
    }
    if (str == "|") {
        nextChar();
        str += ch;
        if(str == "||"){
            return make_shared<Token>(OR_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);
    }
    if (str == "<") {
        nextChar();
        str += ch;
        if(str == "<="){
            return make_shared<Token>(LE_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);

    }
    if (str == ">") {
        nextChar();
        str += ch;
        if(str == ">="){
            return make_shared<Token>(GE_OP, str);
        }
        inputFile.unget();
        str = string(1, ch);
    }
    if (str == "=") {
        nextChar();
        str += ch;
        if(str == "=="){
            return make_shared<Token>(EQ_OP, str);
=======
shared_ptr<Token> Scanner:: nextToken(){
    string text;
    string text_len= ""; // for testing porpoises
    regex pat(R"([\w\d])");
    while(nextChar()) {
        switch (ch) {
            case '+' :
                if(inputFile.peek() == '+'){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(INC_OP, "++"));
                } else {
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '/': {
                nextChar();
                if(ch == '*'){
                    text_len += ch;
                    char c, prev = 0;
                    while (nextChar()){
                        text_len += ch;
                        c = ch;
                        if (c == '/' && prev == '*'){
                            text_len += ch;
                            break;
                        }
                        prev = c;
                    }
                }
                if(ch == '/'){
                    text_len += ch;
                    while (nextChar() && ch != '\n');
                }
            }
            case ' ':
                continue;
            case '/r':
                continue;
            case '/n':
                continue;
            case '-' :
                if(inputFile.peek() == '>'){
                    nextChar();
                    text_len += ch;
                    return make_shared<Token> (PTR_OP, "->");
                }
                if(inputFile.peek() == '-'){
                    nextChar();
                    text_len += ch;
                    return make_shared<Token> (DEC_OP, "--");
                }
                return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
            case '|' :
                if(inputFile.peek() == '|'){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(OR_OP, "||"));
                }else{
                    while(text_len != "") text_len = "";
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '&' :
                if(inputFile.peek() == '&'){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(AND_OP, "&&"));
                }else{
                    while(text_len != "") text_len = "";
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '<' :
                if(inputFile.peek() == '='){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(LE_OP, "<="));
                }else{
                    while(text_len != "") text_len = "";
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '=' :
                if(inputFile.peek() == '='){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(EQ_OP, "=="));
                }else{
                    while(text_len != "") text_len = "";
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '>' :
                if(inputFile.peek() == '='){
                    nextChar();
                    text_len += ch + ch;
                    return shared_ptr<Token> (new Token(GE_OP, ">="));
                }else{
                    while(text_len != "") text_len = "";
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '!' :
                if(inputFile.peek() == '='){
                    nextChar();
                    text_len += ch;
                    return shared_ptr<Token> (new Token(NE_OP, "!="));
                }else{
                    text_len += ch;
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '.':
                if(isdigit(inputFile.peek())){
                    text_len += ch;
                    break;
                }
                while(text_len != "") text_len = "";
                return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));

        }
            if(ch == ';' || ch == '{' ||
                    ch == '}' || ch == '*' || ch == '~' || ch == ']' || ch == '[' || ch == ')' || ch == '(' || ch == ':' || ch == ',' ||
                    ch == '?' || ch == '^' || ch == '%') return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
        while(text_len != "") text_len = "";
        if(ch == '.' || isdigit(ch)){
            text = ch;
            string token = "";
            text_len += ch;
            while(nextChar() && regex_match(string(1,ch), pat)){
                token += ch;
            }
            inputFile.unget();
            text += token;
            text_len += ch;
            // not tested regex !
            vector<regex> number_patterns { regex(R"(0)"),
                                            regex(R"([1-9](\d)*)"),
                                            regex(R"((\d)+[Ee][+-]?(\d)+)"),
                                            regex(R"((\d)*\.(\d)+([Ee][+-]?(\d)+)?)"),
                                            regex(R"((\d)+\.(\d)*([Ee][+-]?(\d)+)?)")};
            for(const auto & pattern: number_patterns){
                if(regex_match(text, pattern)){
                    text_len += ch;
                    return make_shared<Token>(CONSTANT, text);
                }
            }

            return make_shared<Token>(ERROR, text);
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
        }
        inputFile.unget();
        str = string(1, ch);
    }
    if (str == "!") {
        nextChar();
        str += ch;
        if(str == "!="){
            return make_shared<Token>(NE_OP, str);
        }
        inputFile.unget();
    }
    inputFile.unget();
    nextChar();

    if(ch==';' || ch=='{' || ch=='}' || ch==',' || ch==':' || ch==';' || ch=='(' ||
            ch==')' || ch=='/' || ch=='=' || ch=='[' || ch==']' || ch=='~' || ch=='&' || ch=='*' ||
            ch=='%' ||ch=='^' ||ch=='?' ){
        return shared_ptr<Token>
                (new Token(static_cast<tokenType>(ch), string(1, ch)));
    }

<<<<<<< HEAD
    if ((ch >= '0' && ch <= '9') || ch == '-' || ch == '+' || ch == '.') {
        string num = string(1, ch);
        while (nextChar()) {
            if ((ch >= '0' && ch <= '9') || ch == 'e' || ch == 'E' || ch == '.')
            {
                num += ch;
            }
            else {
                break;
            }
        }
        inputFile.unget();
        if (num == ".") {
            return shared_ptr<Token>
                    (new Token(static_cast<tokenType>('.'), num));
        }

    if(regex_match(num, regex(R"(0)")) ||
            regex_match(num, regex(R"([1-9](\d)*)"))  ||
            regex_match(num, regex(R"((\d)+[Ee][+-]?(\d)+)"))  ||
            regex_match(num, regex(R"((\d)*\.(\d)+([Ee][+-]?(\d)+)?)")) ||
            regex_match(num, regex(R"((\d)+\.(\d)*([Ee][+-]?(\d)+)?)")) ){
        return make_shared<Token>(CONSTANT, num);
    }
        return make_shared<Token>(ERROR, num);
    }

    // handle words:
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
        string word = string(1, ch);
        while (nextChar()) {
            bool flag = false;
            int tok_temp = 0;
            if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            flag = true;
            if (!flag) {
                shared_ptr<Token> new_token;
                new_token = symTab.lookupToken(word);
                tok_temp++;
                if (new_token == nullptr) {
                    tok_temp++;
                    new_token = make_shared<varToken>(word);
                    symTab.insertToken(word, new_token);
                }
                new_token->add_line(lineno);
                if(tok_temp<0) return new_token;
                inputFile.unget();
                return new_token;
            }
            word += ch;
=======
        if(ch == '\''){
            text = "";
            if(nextChar()){
                text_len += ch;
                text  = ch;
            }
            if(!(nextChar() && ch == '\'')){
                text_len += ch;
                text  = "";
            }
            while(text_len != "") text_len = "";
            string literal_char = '\'' + text + '\'';
            regex char_pattern(R"(\'[^\']\')");
            text_len += ch;
            if(regex_match(literal_char, char_pattern)){
                return make_shared<Token>(CONSTANT, text);
            }

            return make_shared<Token>(ERROR, text);
        }


        if(ch == '\"'){
            text = "";
            while(nextChar() && ch != '\"'){
                text_len += ch;
                text += ch;
            }
            string literal_string = "\"" + text + "\"";
            regex str_pattern(R"(\"[^\"]*\")");
            while(text_len != "") text_len = "";
            if(regex_match(literal_string, str_pattern)){
                text_len += ch;
                return make_shared<Token>(STRING_LITERAL, text);
            }

            return make_shared<Token>(ERROR, text);
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
        }
    }

<<<<<<< HEAD
    if (ch == '\'') {
        string c = string(1, ch);
        regex reg("\'[^\']\'");
        nextChar();
        c+=ch;
        nextChar();
//        char a = '\'';
        c+=ch;
            return make_shared<Token>(CONSTANT, c.substr(1,c.length() - 2));
    }
=======
        if(isalpha(ch)){
            text_len += ch;
            text = ch;
            string token = "";
            while(nextChar() && regex_match(string(1,ch), pat)){
                text_len += ch + token;
                token += ch;
            }
            inputFile.unget();
            text += token;
            regex word_pattern(R"(\w(\w|\d)*)");
            if(regex_match(text, word_pattern)){
                text_len += ch;
                shared_ptr<Token> token = symTab.lookupToken(text);
                //If token not exists at symbol table
                if(token == nullptr){
                    text_len += ch;
                    shared_ptr<Token> var_token(new varToken(text));
                    var_token->add_line(lineno);
                    token = var_token;
                    text_len += ch;
                    symTab.insertToken(text, token);
                }
                else if(token->getType() == IDENTIFIER){
                    text_len += ch;
                    token->add_line(lineno);
                }
>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128

    if (ch == '"') {
        string str = string(1, ch);
        regex reg("\"[^\"]*\"");
        nextChar();
        while (ch != '"') {
            str += ch; // "abc
            nextChar();
        }
            return make_shared<Token>(STRING_LITERAL, str.substr(1, str.length() - 1));
    }
<<<<<<< HEAD
    return nullptr;
=======

    return nullptr;


>>>>>>> 5d147cdf70ed010d525e88e7903b6d3df8155128
}