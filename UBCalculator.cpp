/*
        Author : Kangmin Kim
*/

#include "UBCalculator.h"
#include "error_handling.h"
#include "Lexer.h"
#include <sstream>
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;


const map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
const map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
void clearAll();
void clear_by_command(string);

const string UBCalculator::openDelims = "([{";
const string UBCalculator::closeDelims = ")]}";

vector<Token> new_vec;


map<string, float> IDENT_map;
vector<Token> vec;


void UBCalculator::setLine(string str)
{
        clearAll();
        cout << "setLine \n";

        Lexer lex(str);
        form_checker(lex, openDelims, closeDelims);


        vec = lex.tokenize();

        lex.restart();

        if(vec[0].value == "clear"){
            clear_by_command(vec[1].value);
            return ;
        }

        if(vec[0].type == IDENT && vec.size() == 1){
            print_IDENT(vec[0]);
            return ;
        }

    error_checker();
    convert_to_prefix();


}

void UBCalculator::print_IDENT(Token token){
    if(IDENT_map.find(token.value) == IDENT_map.end()){
        throw Undefined_Variable_Exception(token.value);
    } else {
        cout << token.value<< " : " << IDENT_map[token.value] << endl;
    }

}

void UBCalculator::convert_to_prefix(){
    stack<Token> new_stk;


    //while(lex.has_more_token()){

    for(int i = 0 ; i <vec.size(); i++){
                //Token curr_tok = lex.next_token();
                //string curr_val = curr_tok.value;
                Token curr_tok = vec[i];
                string curr_val = curr_tok.value;
        cout << "curr_tok.value / curr_tok.type : "  << curr_tok.value <<  " / " << curr_tok.type<<endl;

    switch (curr_tok.type){

    case ENDTOK :
        cout << "ENDTOK" << endl;
        break;

    case NUMBER :
        new_vec.push_back(curr_tok);
        break;

    case IDENT :
        new_vec.push_back(curr_tok);

        break;

    case OPERATOR :
        if(new_stk.empty()){
            new_stk.push(curr_tok);
        }else {
             if(curr_val == "+" || curr_val == "-"){
                cout << "curr_tok.type : "  << curr_tok.type <<endl;
                 //while(new_stk.top().value!="(" || new_stk.top().value!="[" || new_stk.top().value!="{" || new_stk.top().value == "="){
                 //   Token temp = new_stk.top();
                 //   new_vec.push_back(temp);
                 //   new_stk.pop();
                 //   if(new_stk.empty()) break;
                 //   if(new_stk.top().value == "=") break;
                //}
                while(!new_stk.empty()){
                   if(new_stk.top().value == "(") break;
                   if(new_stk.top().value == "[") break;
                   if(new_stk.top().value == "{") break;

                   if(new_stk.top().value == "=") break;

                   Token temp = new_stk.top();
                   new_vec.push_back(temp);
                   new_stk.pop();

                }
                new_stk.push(curr_tok);


            } else if(curr_val == "*" || curr_val == "/"){

                if(new_stk.top().value=="*" || new_stk.top().value=="/"){
                    Token temp = new_stk.top();
                    new_vec.push_back(temp);
                    new_stk.pop();
                    new_stk.push(curr_tok);
                } else {
                    new_stk.push(curr_tok);
                }

            } else if(curr_val == "="){
                new_stk.push(curr_tok);
            }
        }
        break;

    case DELIM :
        if(curr_val == "(" || curr_val == "{" || curr_val == "["){
            new_stk.push(curr_tok);
        } else if(curr_val == ")" || curr_val == "]" || curr_val == "}") {

            while(new_stk.top().value!="(" && new_stk.top().value!="[" && new_stk.top().value!="{"){
                    Token temp = new_stk.top();
                    new_vec.push_back(temp);
                    new_stk.pop();
               }
               new_stk.pop();
           }
        break;
        }

          for(int i = 0; i < new_vec.size();i++){
        cout << new_vec.at(i).value << " ";
        }
        cout << endl;

        } // end of main while

        while(!new_stk.empty()){
                Token temp = new_stk.top();
                new_vec.push_back(temp);
                new_stk.pop();
        }

        cout << "calculated : " ;
        for(int i = 0; i < new_vec.size();i++){
        cout << new_vec.at(i).value << " ";
        }
        cout << endl;

        value_after_calc(new_vec);

}

void UBCalculator::error_checker(){

    bool size_changed = false;

    cout << "size : " << vec.size() <<endl;
    cout << "vec : ";
    for(int i = 0; i < vec.size(); i++){
    cout << vec[i].value << " ";
    }
    cout <<endl;

    if(vec[0].value == "+" ){
        cout << " replaced " <<endl;
        vec.erase(vec.begin() + 0);

        size_changed = true;
    }


    if(vec[0].value == "=") throw Syntax_Error_Exception("= OPERATOR can't be the first token");
    if(vec[vec.size()-1].type == OPERATOR) throw Syntax_Error_Exception("OPERATOR can't be the last token");

    for(int i = 0; i < vec.size() ; i ++){
            cout << i <<endl;
        if( (i+1) > vec.size()-1 ) break;
        if(vec[i].value == "*" || vec[i].value == "/"){
            if(vec[i+1].type == OPERATOR) throw Syntax_Error_Exception("OPERATOR can't be placed after * or /");
            if(vec[i-1].type == OPERATOR) throw Syntax_Error_Exception("OPERATOR can't be placed before * or /");
        }

        if(vec[i].type == NUMBER){
            if(vec[i+1].type == NUMBER) throw Syntax_Error_Exception("NUMBER can't be placed right after NUMBER");
            if(vec[i+1].type == IDENT) throw Syntax_Error_Exception("IDENT can't be placed right after NUMBER");
        }

        if(vec[i].type == IDENT){
            if(vec[i+1].type == NUMBER) throw Syntax_Error_Exception("NUMBER can't be placed right after IDENT");
            //if(vec[i+1].type == DELIM) throw Syntax_Error_Exception("DELIM can't be placed right after IDENT");
            if(vec[i+1].type == IDENT) throw Syntax_Error_Exception("IDENT can't be placed right after IDENT");
        }

        if(vec[i].value == "="){
            if(vec[i-1].type == NUMBER && vec[i+1].type == NUMBER)
                throw Syntax_Error_Exception("Wrong form as NUMBER = NUMBER");
            if(vec[i+1].value == "+"){
                vec.erase(vec.begin() + i+1);

                size_changed = true;
                break;
            }
        }
        if(vec[i].value == "-" && vec[i+1].value == "-"){
                cout << " replaced " <<endl;
            vec[i+1].value = "+";
            vec.erase(vec.begin() + i);

            size_changed = true;
            break;
        }

        if((vec[i].value == "[" || vec[i].value == "{" || vec[i].value == "(")&& vec[i+1].value == "+"){

            vec.erase(vec.begin() + (i+1));

            size_changed = true;
            break;
        }

        if((vec[i+1].value == "]" || vec[i+1].value == "}" || vec[i+1].value == ")")&& vec[i].type == OPERATOR){
            throw Syntax_Error_Exception();
        }


        if(vec[i].value == "+" && vec[i+1].value == "+"){
                cout << " replaced " <<endl;
            vec.erase(vec.begin() + i);

            size_changed = true;
            break;
        }
        if(vec[i].value == "+" && vec[i+1].value == "-"){
                cout << " replaced " <<endl;
            vec.erase(vec.begin() +i);

            size_changed = true;
            break;
        }
        if(vec[i+1].value == "+" && vec[i].value == "-"){
                cout << " replaced " <<endl;
            vec.erase(vec.begin() + i+1);

            size_changed = true;
            break;
        }
    }
    if(size_changed == true) error_checker();
}


void UBCalculator::value_after_calc(vector<Token>& tok_vec){
    stack<Token> tok_stk;
    stack<Token> oper_stk;
    //Token first_tok(NUMBER, "0"), second_tok(NUMBER, "0");
    Token first_tok, second_tok;


    for(unsigned i = 0; i < tok_vec.size(); i++){

        cout << "i : " << tok_vec.at(i).value << endl;
        if(tok_vec.at(i).type == NUMBER){
            tok_stk.push(tok_vec.at(i));

        }else if (tok_vec.at(i).type == IDENT){
            tok_stk.push(tok_vec.at(i));

        }else if(tok_vec.at(i).type == OPERATOR){
            if(oper_stk.empty())
            {
                oper_stk.push(tok_vec[i]);
            } else if(tok_vec[i].value == "-" && oper_stk.top().value == "-"){
                Token temp(OPERATOR, "+");
                cout << "now tok_vec[i] == "<< temp.value << endl;
                oper_stk.pop();
                oper_stk.push(temp);
            } else
            {
                oper_stk.pop();
                oper_stk.push(tok_vec[i]);
            }

            if(tok_stk.size() >= 1){
                cout << "start calculate!" << endl;
                cout << vec[i].value << endl;

                if(tok_stk.top().type == IDENT){
                    if(IDENT_map.find(tok_stk.top().value) == IDENT_map.end()){
                        throw Undefined_Variable_Exception(tok_stk.top().value);
                    } else {

                        //float ident_value = IDENT_map[tok_stk.top().value];
                        //tok_stk.top().type = IDENT;
                        //tok_stk.top().value = to_string(ident_value);
                        //cout << "value tok_stk.top() has been changed to : " << tok_stk.top().value <<endl;
                   }
                }

            second_tok = tok_stk.top();
            cout << "second tok set : " << second_tok.value << endl;
            tok_stk.pop();

            if(tok_stk.size() > 0){
                if(tok_stk.top().type == IDENT){
                    if(IDENT_map.find(tok_stk.top().value) == IDENT_map.end()){
                        if(oper_stk.top().value == "="){
                            cout << "add to map key : " << tok_stk.top().value <<endl;
                            IDENT_map[tok_stk.top().value] = 0;
                        }
                    } else {
                        //float ident_value = IDENT_map[tok_stk.top().value];
                        //tok_stk.top().type = IDENT;
                        //tok_stk.top().value = to_string(ident_value);
                        //cout << "value tok_stk.top() has been changed to : " << tok_stk.top().value <<endl;
                    }
                }
            } else if (tok_stk.size() == 0){
                cout << "tok size 0" <<endl;
                Token temp(NUMBER, "0");
                tok_stk.push(temp);
            }
            first_tok = tok_stk.top();
            cout << "first tok set : " << first_tok.value << endl;
            tok_stk.pop();

            cout << "1st, 2nd tok, OPERATOR : " << first_tok.value << " , " << second_tok.value << " , " << tok_vec[i].value << endl ;

            Token calculated_tok(NUMBER, "");
            switch(oper_stk.top().value.at(0)){

                case '+' :
                    {

                     if(first_tok.type == IDENT && second_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] + IDENT_map[second_tok.value] );
                     } else if ( first_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] + (atof(second_tok.value.c_str())) );
                     } else if ( second_tok.type == IDENT){
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) + IDENT_map[second_tok.value] );
                     } else {
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) + (atof(second_tok.value.c_str())) );
                     }
                    //cout << "pushed value :" << first_val+second_val <<endl;
                    tok_stk.push(calculated_tok);

                    }
                break;

                case '-' :
                    {
                        cout << "case : -" << endl;
                     if(first_tok.type == IDENT && second_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] - IDENT_map[second_tok.value] );
                     } else if ( first_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] - (atof(second_tok.value.c_str())) );
                     } else if ( second_tok.type == IDENT){
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) - IDENT_map[second_tok.value] );
                     } else {
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) - (atof(second_tok.value.c_str())) );
                     }
                    //cout << "pushed value :" << first_val-second_val <<endl;
                    tok_stk.push(calculated_tok);

                    }
                break;

                case '*' :
                    {

                     if(first_tok.type == IDENT && second_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] * IDENT_map[second_tok.value] );
                     } else if ( first_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] * (atof(second_tok.value.c_str())) );
                     } else if ( second_tok.type == IDENT){
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) * IDENT_map[second_tok.value] );
                     } else {
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) * (atof(second_tok.value.c_str())) );
                     }
                    //cout << "pushed value :" << first_val*second_val <<endl;
                    tok_stk.push(calculated_tok);

                    }
                break;

                case '/' :
                    {
                    if(second_tok.value == "0") throw DBZ_Exception();

                     if(first_tok.type == IDENT && second_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] / IDENT_map[second_tok.value] );
                     } else if ( first_tok.type == IDENT){
                        calculated_tok.value = to_string( IDENT_map[first_tok.value] / (atof(second_tok.value.c_str())) );
                     } else if ( second_tok.type == IDENT){
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) / IDENT_map[second_tok.value] );
                     } else {
                        calculated_tok.value = to_string( (atof(first_tok.value.c_str())) / (atof(second_tok.value.c_str())) );
                     }
                    tok_stk.push(calculated_tok);

                    }
                break;

                case '=' :
                    {
                        cout << "here is = \n";
                        cout << "first tok / second tok : " << first_tok.value << " / " << second_tok.value << endl;
                        //if(first_tok.type == NUMBER && second_tok.type == NUMBER) throw Syntax_Error_Exception("NUMBER can't be assigned to NUMBER");
                        if(first_tok.type == NUMBER) throw Syntax_Error_Exception("can't assigned to NUMBER");


                        if(first_tok.type == IDENT && second_tok.type == IDENT){

                            IDENT_map[first_tok.value] = IDENT_map[second_tok.value];
                        }else {
                            IDENT_map[first_tok.value] = atof(second_tok.value.c_str());
                        }
                        tok_stk.push(first_tok);
                        cout << "value of key(" << first_tok.value <<") : " << IDENT_map[first_tok.value] <<endl;
                    }
                }
            }
            }
    }
    if(!tok_stk.empty() && tok_stk.top().type != IDENT){
        cout << tok_stk.top().value <<" is top value" <<endl;
        stringstream ss;
        ss << atof(tok_stk.top().value.c_str());
        cout << ss.str() <<" hehe"<< endl;
    }



}


void UBCalculator::form_checker(Lexer lex, const string openDelims, const string closeDelims){
    Token tok;
    size_t p;
    stack<Token> temp_stk;


    while (lex.has_more_token()) {

            if ( (tok = lex.next_token()).type != DELIM ) continue;

            if (openDelims.find(tok.value) != string::npos) {
                temp_stk.push(tok);
                continue;
            }


            p = closeDelims.find(tok.value); // 0, 1, 2, or string::npos
            if (p != string::npos) {          // tok is a close delim
                if (temp_stk.empty() ||
                    openDelims.find(temp_stk.top().value) != p) {
                    temp_stk.push(tok); // make sure it's not empty
                    break;
                } else {
                    temp_stk.pop();
                }
            }

        }

    if (!temp_stk.empty()) {
            throw Syntax_Error_Exception("Expression is not well formed");
            while (!temp_stk.empty()) temp_stk.pop();
    }

}

void clearAll()
{
    vec.clear();
    new_vec.clear();
}

void clear_by_command(string com){
        cout << "com : " << com <<endl;
    if(com == "all"){
        IDENT_map.clear();
        cout << "ALL variables removed." <<endl;
    } else {
        if(IDENT_map.find(com) == IDENT_map.end()){
            throw Syntax_Error_Exception("No variable to clear");
        } else {
        IDENT_map.erase(com);
        cout << "variable [" << com << "] removed." << endl;
        }
    }
}


