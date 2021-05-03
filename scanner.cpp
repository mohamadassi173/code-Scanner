#include <regex>
#include "scanner.h"

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
        }
    }

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
    return nullptr;
}