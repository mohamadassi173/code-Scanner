#include "scanner.h"
#include <cctype>
#include <memory>

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
        }

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
        }

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

                return token;
            }
        }
    }

    return nullptr;


}