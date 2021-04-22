#include "scanner.h"
#include <cctype>
#include <memory>

shared_ptr<Token> Scanner:: nextToken(){
    string text;

    auto read_token = [this](regex pattern){
        string token = "";
        while(nextChar() && regex_match(string(1,ch), pattern)){
            token += ch;
        }
        inputFile.unget();//return last read character to the file

        return token;
    };

    //passing read_token in order to call read_token
    auto read_number = [read_token](){
        return read_token(regex(R"([0-9Ee+\-.])"));
    };

    auto read_word = [read_token](){
        return read_token(regex(R"([\w\d])"));
    };

    auto read_char = [this](){
        string ch_str = "";
        if(nextChar()){
            ch_str = ch;
        }
        if(!(nextChar() && ch == '\'')){
            ch_str = "";
        }

        return ch_str;
    };

    auto read_string = [this](){
        string str = "";
        while(nextChar() && ch != '\"'){
            str += ch;
        }

        return str;
    };

    while(nextChar()) {
        if(ch == '/'){
            nextChar();
            if(ch == '*'){
                char c, prev = 0;
                while (nextChar()){
                    c = ch;
                    if (c == '/' && prev == '*'){
                        break;
                    }
                    prev = c;
                }
            }
            if(ch == '/'){
                while (nextChar() && ch != '\n');
            }
        }

        if (ch == ' ' || ch == '\r' || ch == '\n') {
            continue;
        }

        switch (ch) {
            case '+' :
                if(inputFile.peek() == '+'){
                    nextChar();
                    return shared_ptr<Token> (new Token(INC_OP, "++"));
                } else {
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '-' :
                if(inputFile.peek() == '-'){
                    nextChar();
                    return make_shared<Token> (DEC_OP, "--");
                } else if(inputFile.peek() == '>'){
                    nextChar();
                    return make_shared<Token> (PTR_OP, "->");
                }
                return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));

            case '&' :
                if(inputFile.peek() == '&'){
                    nextChar();
                    return shared_ptr<Token> (new Token(AND_OP, "&&"));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '|' :
                if(inputFile.peek() == '|'){
                    nextChar();
                    return shared_ptr<Token> (new Token(OR_OP, "||"));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '<' :
                if(inputFile.peek() == '='){
                    nextChar();
                    return shared_ptr<Token> (new Token(LE_OP, "<="));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '>' :
                if(inputFile.peek() == '='){
                    nextChar();
                    return shared_ptr<Token> (new Token(GE_OP, ">="));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '=' :
                if(inputFile.peek() == '='){
                    nextChar();
                    return shared_ptr<Token> (new Token(EQ_OP, "=="));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '!' :
                if(inputFile.peek() == '='){
                    nextChar();
                    return shared_ptr<Token> (new Token(NE_OP, "!="));
                }else{
                    return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
                }
            case '.':
                if(isdigit(inputFile.peek())){
                    break;
                }
                return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));

            case ';' :
            case '{' :
            case '}' :
            case ',' :
            case ':' :
            case '(' :
            case ')' :
            case '[' :
            case ']' :
            case '~' :
            case '*' :
            case '/' :
            case '%' :
            case '^' :
            case '?' :
                return make_shared<Token>(static_cast<tokenType>(ch), string(1, ch));
        }

        //Scan for number
        if(ch == '.' || isdigit(ch)){
            text = ch; //since 'ch' might change due to 'read_number' function call
            text += read_number();
            vector<regex> number_patterns { regex(R"(0)"), //0
                                            regex(R"([1-9](\d)*)"), //104
                                            regex(R"((\d)+[Ee][+-]?(\d)+)"),   //123E4
                                            regex(R"((\d)*\.(\d)+([Ee][+-]?(\d)+)?)"), //x.123
                                            regex(R"((\d)+\.(\d)*([Ee][+-]?(\d)+)?)")}; //123.x
            for(const auto & pattern: number_patterns){
                if(regex_match(text, pattern)){
                    return make_shared<Token>(CONSTANT, text);
                }
            }

            goto error;
        }

        //Scan for Char
        if(ch == '\''){
            text = read_char();
            string literal_char = '\'' + text + '\'';
            regex char_pattern(R"(\'[^\']\')");

            if(regex_match(literal_char, char_pattern)){
                return make_shared<Token>(CONSTANT, text);
            }

            goto error;
        }

        //Scan for String
        if(ch == '\"'){
            text = read_string();
            string literal_string = "\"" + text + "\"";
            regex str_pattern(R"(\"[^\"]*\")");

            if(regex_match(literal_string, str_pattern)){
                return make_shared<Token>(STRING_LITERAL, text);
            }

            goto error;
        }

        //Scan for words
        if(isalpha(ch)){
            text = ch;
            text += read_word();
            regex word_pattern(R"(\w(\w|\d)*)");
            //Check if token is valid word
            if(regex_match(text, word_pattern)){
                shared_ptr<Token> token = symTab.lookupToken(text);
                //If token not exists at symbol table
                if(token == nullptr){
                    shared_ptr<Token> var_token(new varToken(text));
                    var_token->add_line(lineno);
                    token = var_token;
                    symTab.insertToken(text, token);
                }
                else if(token->getType() == IDENTIFIER){
                    token->add_line(lineno);
                }

                return token;
            }
        }
    }

    //We reached the end of the file, no more tokens
    return nullptr;

    error:
    return make_shared<Token>(ERROR, text);
}