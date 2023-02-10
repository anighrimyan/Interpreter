#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>
//function for checking variable name is correct or not
bool is_identifier(std::string& token);
//function for choosing next token's action
void do_interpretation(std::ifstream& fin);
//function for checking is token keyword or not and returns it's index
int is_keyword(std::string first_token);
//function for ending condition/if/ body
void do_endif();
//function for cheking condition
bool check_condition(std::ifstream& fin, const int case_index);
//function for doing condition/if/
void do_condition(std::ifstream& fin, const int case_index);
//function to declere variable
void do_decleration(std::ifstream& fin, int case_index);
//function for assinging variables
void do_tanjanq(std::ifstream& fin, const int case_index);
//function for cheking +, -, *, / operators
bool is_operator(std::string& tmp);
//function for cheking ==, !=, >, < operators
bool is_operator_if(std::string& tmp);
//function for operations
std::string do_operation(std::string& expression);
//function for chaking token is number or not
bool is_number(std::string& operand);
//function for checking token float type number or not
bool is_double(std::string& operand);
//function for calculate integer type numbers
std::string calculator (int num1, int num2, char operation);
//function for calculate double type numbers
std::string calculator (double num1, double num2, char operation);
//function for print something
void do_print(std::ifstream& fin, const int case_index);
//function to separate a string to left and right parts
bool left_center_right (std::string& str_new);
//function for cheking big or small operators 
bool operator_big_small(std::string& center_operand, double left_num, double right_num);
//function for cheking equal operator
bool operator_equal(double left_operand, double right_operand);
//function for cheking non-equal operator
bool operator_noequal(double left_operand, double right_operand);
//function for ending line
void do_newline();

struct variable {
    std::string type;
    std::string value;
};
std::vector<std::string> keywords { "num", "str", "logic", "newline", "if", "endif", "print" };
std::map<std::string, variable> var_info;
int main()
{
    std::ifstream file;
    std::string path;
    std::cout << "Enter the name of the file:";
    std::cin >> path;
    file.open (path + ".txt") ;
    if (file.is_open())
    {   
      do_interpretation(file);
    }
    else
    {
        std::cout << "Something was gone wrong!!!" << std::endl;
    }
    return 0;
}

void do_interpretation(std::ifstream& fin)
{
    std::string token;
     enum MyKeywords {Num, Str, Logic, Newline, If, Endif, Print };
    while (!fin.eof())
    {
        fin >> token;
        int case_index = is_keyword(token);
        switch (case_index) 
        {
          case Num:
          case Str:
          case Logic:
            do_decleration(fin, case_index);
            break;
          case Newline:
            do_newline ();
            break;
          case If:
          do_condition(fin, case_index);
             break;
         case Endif:
            do_endif();
            break;
          case Print:
              do_print(fin, case_index);
            break; 
          case -1:
            throw std::invalid_argument("ERROR: You have used an invalid keyword!!!");   
         default:
            do_tanjanq(fin, case_index);
            break;
        }
    }
}

int is_keyword(std::string first_token)
{
  for (int i = 0; i < keywords.size(); i++)
     {
       if (keywords[i] == first_token) 
         { 
           return i; 
         }
      }
  return -1;
}

void do_decleration(std::ifstream& fin, const int type_index) {
    std::string token;
    do {
        std::string name;
        fin >> token;
        if (!(is_identifier(token)))
        {
          throw std::invalid_argument("ERROR: You have used an invalid keyword!");  
         }
        var_info[token].type = keywords[type_index];
        keywords.push_back(token);
        name = token;
        fin >> token;
        if (token == "=") 
        {
            fin >> token;
            std::string tmp;
            fin >> tmp;
            if (tmp == "," || tmp == ";")
            {
                int index = is_keyword(token);
                if (index > 6)
                {
                    var_info[name].value = var_info[keywords[index]].value;
                }
                else if (index == -1)
                {
                    var_info[name].value = token;
                }
                else
                {
                   throw std::invalid_argument("ERROR: You have used a keyword!");     
                }
                token = tmp;
            }
            else if (is_operator (tmp))
            {
                std::string expression;
                expression += token + tmp;
                while (token != "," && token != ";")
                {   
                    fin >> token;
                    if (token != "," && token != ";"){
                        expression += token;
                    }
                }
            var_info[name].value = do_operation(expression);
            }
        }
    } while (token != ";" && !(is_keyword(token) + 1));
}

bool is_operator(std::string& tmp)
{
    if (tmp == "+" || tmp == "-" || tmp == "*" || tmp == "/")
    {
        return true;
    }
    return false;
}

bool is_operator_if(std::string& tmp)
{
    if (tmp == "==" || tmp == "!=" || tmp == ">" || tmp == "<")
    {
        return true;
    }
    return false;
}

std::string do_operation(std::string& expression){
    std::string left_operand;
    char center_operand;
    std::string right_operand;
    int i = 0;
    while (expression[i] != '+' && expression[i] != '-' && expression[i] != '*' && expression[i] != '/' && i < expression.length())
    {
        left_operand.push_back(expression[i]);
        ++i;
    }
    if ((expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') && (i < expression.length()))
    {
        center_operand = expression[i];
    }
    ++i;
    while (i < expression.length())
    {
        right_operand.push_back(expression[i]);
        ++i;
    }
    for (int i = 7; i < keywords.size(); i++){
        if (left_operand == keywords[i]){
            left_operand = var_info[left_operand].value;
        }
        if (right_operand == keywords[i]){
            right_operand = var_info[right_operand].value;
        }
    }
    if ( is_number (left_operand) && is_number (right_operand) ){
        int left_num = std::stoi(left_operand);
        int right_num = std::stoi(right_operand);
        return calculator (left_num, right_num, center_operand);
    }
    if ( (is_double (left_operand) && is_number (right_operand)) || (is_double (right_operand) && is_number (left_operand))  || (is_double (right_operand) && is_double (left_operand)) ){
        double left_num = std::stod(left_operand);
        double right_num = std::stod(right_operand);
        return calculator (left_num, right_num, center_operand);
    }
  throw std::invalid_argument("ERROR: Wrong operation!");  
}
bool is_number(std::string& operand)
{
    for (int i = 0; i < operand.size(); ++i)
    {
        if (operand[i] < '0' || operand[i] > '9')
        {
            return false;
        }
    }
    return true;
}

bool is_double (std::string& operand) {
    int counter = 0;
    int index = 0;
    for ( int i = 1; i < operand.size()-1; i++){
        if (operand[i] == '.'){
            counter ++;
            index = i;
        }
    }
        if (counter != 1){
            return false;
        }
    
  for (int i = 0; i < operand.size(); ++i)
    {
        if ( (operand[i] < '0' && i != index ) || (operand[i] > '9' && i != index) )
        {
            return false;
        }
    }
    return true;
}

bool is_identifier(std::string& token) {
    if (is_keyword (token) != -1){
        return false;
    }
    for (int i = 0; i < token.length(); i++){
        if (token[i] < 'a' || token[i] > 'z'){
            return false;
        }
    }
    return true;
}

std::string calculator (int num1, int num2, char operation) {
    enum MyOperators {Addition = '+', Subtraction = '-', Multiplication = '*', Division = '/'};
    switch (operation){
           case Addition:
                return (std::to_string (num1 + num2));
           case Subtraction:
                 return (std::to_string(num1 - num2));
           case Multiplication:
                 return  (std::to_string(num1 * num2));
           case Division:
            if (num2!=0) {
                return (std::to_string(num1 / num2));
            }else return "div/0 !!!"; 
           default: std::cout << "Error This operation is not defined!" << std::endl;
        }
 return 0;   
}

std::string calculator (double num1, double num2, char operation) {
    enum MyOperators {Addition = '+', Subtraction = '-', Multiplication = '*', Division = '/'};
    switch (operation){
           case Addition:
                return (std::to_string (num1 + num2));
           case Subtraction:
                 return (std::to_string(num1 - num2));
           case Multiplication:
                 return  (std::to_string(num1 * num2));
           case Division:
            if (num2!=0) {
                return (std::to_string(num1 / num2));
            } else return "div/0 !!!"; 
           default: std::cout << "Error This operation is not defined!" << std::endl;
        }
 return 0;   
}

void do_tanjanq(std::ifstream& fin, const int type_index) {
         std::string name = keywords[type_index];
        std::string token;
        fin >> token;      
     if (token == "=") 
        {
            fin >> token;
            std::string tmp;
            fin >> tmp;
            if (tmp == ";")                     
            {
                int index = is_keyword(token);
                if (index > 6 )
                {
                    var_info[name].value = var_info[keywords[index]].value;
                }
                else if (index == -1)
                {
                    var_info[name].value = token;
                }
                else
                {
                    throw std::invalid_argument("ERROR: You have used a keyword!");   
                }
                token = tmp;
            }
            else if (is_operator (tmp))
            {
                std::string expression;
                expression += token + tmp;
                while (token != ";")
                {   
                    fin >> token;
                    if (token != ";"){
                        expression += token;
                    }
                }
            var_info[name].value = do_operation(expression);
            }
        }
}

void do_print(std::ifstream& fin, const int type_index) {
       std::string name = keywords[type_index];
        std::string token;
        fin >> token;
     if (token == "->") 
        {
            fin >> token;
            std::string tmp;
            fin >> tmp;
            if (tmp == ";")                     
            {
                int index = is_keyword(token);
                if (index > 6 )
                {
                    std::cout << var_info[keywords[index]].value << "  ";
                }
                else if (index == -1)
                {
                    std::cout << token << "  ";
                }
                else
                {
                    throw std::invalid_argument("ERROR: You have used a keyword!");  
                }
                token = tmp;
            }
            else if (is_operator (tmp))
            {
                std::string expression;
                expression += token + tmp;
                while (token != ";")
                {   
                    fin >> token;
                    if (token != ";"){
                        expression += token;
                    }
                }
             std::cout << do_operation(expression) << "  ";
            }
        }  
}

 bool check_condition(std::ifstream& fin, const int case_index){
        bool result_1;
        std::string token;
        fin >> token;
         if (token == "->") 
        {
            fin >> token;     
            std::string tmp;
            fin >> tmp;
            if (tmp == ";")                     
            {
                if (token == "0"){
                 return false;
                }
     int index = is_keyword(token);
            if (index > 6 )
                {
                    if (var_info[keywords[index]].value != "0"){
                    return true;
                    }
                    else {
                        return false;
                   }
                }
                else if (index == -1)
                {
                      return true;
                }
                else
                {
                    throw std::invalid_argument("ERROR: You have used a keyword!"); 
                }
                token = tmp;
            }
            else if (is_operator_if (tmp))
            {
                std::string expression;
                expression += token + tmp;
                while (token != ";")
                {   
                    fin >> token;
                    if (token != ";"){
                        expression += token;
                    }
                }
                 bool result = left_center_right (expression);
          return result;
            }
        }  
 }

bool left_center_right (std::string& str_new)
{ 
    std::string left_operand;
    std::string center_operand;
    std::string right_operand;    
    int i = 0;
    while (str_new[i] != '!' &&str_new[i] != '=' && str_new[i] != '>' && str_new[i] != '<' && i < str_new.length())
    {
       left_operand.push_back(str_new[i]);
       ++i;
    }
    while ((str_new[i] == '!' || str_new[i] == '=' || str_new[i] == '>' || str_new[i] == '<' ) && (i < str_new.length()))
    {
        center_operand.push_back(str_new[i]);
      ++i;
    }
    while (i < str_new.length())
    {
        right_operand.push_back(str_new[i]);
        ++i;
    }
  for (int i = 7; i < keywords.size(); i++){
        if (left_operand == keywords[i]){
            left_operand = var_info[left_operand].value;
        }
        if (right_operand == keywords[i]){
            right_operand = var_info[right_operand].value;
        }
    }
     bool result;
     double left_num;
     double right_num;
    if((is_number(left_operand) || is_double(left_operand) ) && (is_number(right_operand) || is_double(right_operand))){
        left_num = std::stod(left_operand);
        right_num = std::stod(right_operand);
    }
    if (center_operand == "==")
    {
        result = operator_equal(left_num, right_num);
        return result;
    }
     else  if (center_operand == "!=")
    {
    result = operator_noequal(left_num, right_num);
      return result;
    } 
    else  if (center_operand == ">" || center_operand == "<")
    {
    result = operator_big_small(center_operand, left_num, right_num);
    return result;
    } 
}
  
bool operator_big_small(std::string& center_operand, double left_num, double right_num)
{      
    if (center_operand[0] == '>')
    {
          if (left_num > right_num)
          {
              return true;
          }
          else
          {
              return false;
          }
       }
     else if (center_operand[0] == '<')
     {
          if (left_num < right_num)
          {
              return true;
          }
          else
          {
              return false;
          }
     }
     else
     {
         std::cout << "error: this condition does not define." << std::endl;
         return false;
     }
}

bool operator_equal(double left_operand, double right_operand)
{
    if (left_operand == right_operand)
    {
        return true;
    }
    return false;
}

bool operator_noequal(double left_operand, double right_operand)
{
    if (left_operand != right_operand)
    {
        return true;
    }
    return false;
}
void do_endif() {
    return;
}

void do_condition(std::ifstream& fin, const int case_index){
    std::string  token;    
    if (!(check_condition (fin, case_index))){
        fin >> token;
        while (!fin.eof() ){
            fin >> token;
            if (token == "endif")
            return;
        }
    }
}

void do_newline(){
    std::cout << std::endl;
}
