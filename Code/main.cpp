#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstring>
#include <locale>
#include <map>
#include <sstream>
#include <stack>


#define tokenSP tokenString[Index]
#define SIZE 9999999
#define NUM 0
using namespace std;


const int num = 0;
string currentLexeme;
string token_buffer;
string lexeme_buffer="";
string token;
string lexeme="";
vector<string> vec;
bool hasLexeme = false;
bool next_lexeme;

string tokenString;
int State = 1;
int Index = 0;

int address = 1;

int start = 0;
int count = 1;
int _address= 2000;
int jumpAddress;
stack<int> jumpStack;

vector <string> tokenVec;
vector <string> lexemeVec;
string save;
bool valid = true;
int vecIndex = 0;
string instructionTable[300][3];
string symbolTable[10][3];

//function PROTOTYPES
int isIdentifier(string *token);
void LexemeVector();
int lexer(char input, int index);
bool isKeyword(string token);
bool isOperator(char to );
void removeComments(string *comment);
bool isSeparator(char to);
void readFile(string inputFile);
string getML(string id);
void parser();
bool S();
bool C();
bool A();
bool E();
bool T();
bool EPrime();
bool TPrime();
bool F();
bool If();
bool Else();
bool While();
void epsilon();
void insertTable(int & address, string op, string oprnd);
void insertSymbol(string &id, int & address,string &keyword,string(&symbolTable)[10][3]);
void print();
void printSymbol();
string toString(int num);
void printInstruction();

void backPatch(int jumpAddress,stack<int> & jumpStack);
int table[10][6] = {
    2,5,9,4,10,9,
    2,2,2,3,3,9,
    1,1,1,1,1,1,
    1,1,1,1,1,1,
    9,5,9,6,6,7,
    1,1,1,1,1,1,
    9,7,9,8,8,9,
    1,1,1,1,1,1,
    1,1,1,1,1,1,
    1,1,1,1,1,1
};




int main(){

  readFile("input.txt");
  removeComments(&tokenString);
  LexemeVector();
  parser();
  printSymbol();
  printInstruction();
}

int isIdentifier(string *token){

string a = *token;

char *buffer = new char[a.size()];
  for(int i =0; i < a.size(); i++){
    buffer[i] = a.at(i);
  }

  if(isalpha(buffer[0]))
  {
    return 1;
  }
  else{
      return 0;
    }
}
bool isOperator(char to ){
  if(to =='*'||to =='+'||to == '-' ||to == '=' ||to == '/'||to == '>'||to == '<' ||to == '%'){
    return true;
  }
  else{
    return false;
  }
}
bool isKeyword(string token)
{

  if (token == "int" || token == "float" || token == "boolean" || token == "if"
  || token == "else" || token == "then" || token == "do" || token == "while"
  || token == "whileend" || token == "do" || token == "doend" || token == "for"
   || token == "and" || token == "or" || token == "function" || token == "true" || token == "false")
   {
      return true;
  }
  else
  {
    return false;
  }


}

bool isSeparator(char to){
  if(to =='(' || to == ')' || to == '{' || to == '}' || to == '[' || to == ']'|| to == ','|| to == ':'|| to == ';'|| to == '!' || to=='\''
  || int(to) == 10 || int(to) == 13 ||int(to) == 9 || int(to) == 32){
    return true;

  }
  else{
    return false;
  }
}
void removeComments(string *comment){
  int len = comment->length();
  for (int i=0; i < len; i++){
    if(comment->at(i) == '!'){
      int StartComment= i;
      int j = i;
      j++;
      while(comment->at(j) != '!'){
        j++;
      }
      int EndComment = j;
      comment->erase(StartComment,EndComment-StartComment+1);
      len = comment->length();
    }
  }
}
int lexer(char input, int index){
  int i = -1;
  if(isSeparator(input)){
    return table[State+i][i+4];
  }
  if(isOperator(input)){
    return table[State+i][i+5];
  }
  if(isalpha(input)){
    return table[State+i][i+1];
  }
  if(isdigit(input)){
    return table[State+i][i+2];
  }
  if (input == '$') {
     return table[State+i][i+3];
  }
  if (input == '.') {

      return table[State+i][i+6];
  }
  else return i;
}
void LexemeVector () {

    for (int i = 0; i < SIZE ; i++) {

        switch (State) {
            case 1:
                State = lexer(tokenSP, Index);
                if (int(tokenSP) != 13 && int(tokenSP) != 10 && int(tokenSP) != 32 && int(tokenSP) != 9 && int(tokenSP) != 0) {
                    currentLexeme.push_back(tokenString[Index]);

                }
                Index++;
                break;

            case 2:
                State = lexer(tokenSP, Index);
                if (State!=3 && State != 9) {
                    currentLexeme.push_back(tokenString[Index]);
                    Index++;
                }
                break;

            case 3:
                if (isKeyword(currentLexeme)) {
                    tokenVec.push_back("KEYWORD");


                }
                else {
                    tokenVec.push_back("IDENTIFIER");
                }
                lexemeVec.push_back(currentLexeme);
                currentLexeme = "";
                State = lexer(tokenSP, Index);
                break;

            case 4:

                if (currentLexeme.size() > 0) {
                    lexemeVec.push_back(currentLexeme);
                    tokenVec.push_back("SEPARATOR");
                    currentLexeme = "";
                    //E();
                }
                State = lexer(tokenSP, Index);
                break;

            case 5:
                State = lexer(tokenSP, Index);
                if (State!=6 && State != 9) {
                    currentLexeme.push_back(tokenString[Index]);
                    Index++;
                }
                break;

            case 6:
                lexemeVec.push_back(currentLexeme);
                tokenVec.push_back("INTEGER");
                currentLexeme = "";
                State = lexer(tokenSP, Index);
                break;

            case 7:
                State = lexer(tokenSP, Index);
                if (State!=8 && State != 9) {
                    currentLexeme.push_back(tokenString[Index]);
                    Index++;
                }
                break;

            case 8:
                tokenVec.push_back("REAL NUMBER");
                lexemeVec.push_back(currentLexeme);
                currentLexeme = "";
                State = lexer(tokenSP, Index);
                break;

            case 9:
                State = lexer(tokenSP, Index);
                Index++;
                break;

            case 10:
                lexemeVec.push_back(currentLexeme);
                tokenVec.push_back("OPERATOR");
                currentLexeme = "";
                State = lexer(tokenSP, Index);
                break;

            default:
                break;
        }
    }
}


void readFile(string filename){
  ifstream inputFile(filename);
  if(inputFile.is_open()){


    cout << "File open" << endl;

    string contents((istreambuf_iterator<char>(inputFile)),istreambuf_iterator<char>());

    for(int i = 0; i < contents.length(); i++){
      tokenString.push_back(contents[i]);


    }
}
  else{
    cout << "File Error" << endl;
    exit(1);
  }


}

void parser() {

    //loop through vector

    for(vecIndex = 0; vecIndex < lexemeVec.size()-1; vecIndex++) {
      cout << vecIndex <<" "<< tokenVec.at(vecIndex) <<"  " << lexemeVec.at(vecIndex) <<endl;

        print();

        if( !S() ) {

            cout << "CODE INVALID\n";

            valid = false;
            break;
        }
    }

    if (valid) cout << "\nCODE VALID\n";
};

bool S() { //   S  -> If | Else | While | A | KEYWORD
  string dataType;
    print();

    cout << "<Statement> -> <If> | <Else> | <While> | <A> | KEYWORD" << endl;

    if (tokenVec[vecIndex] == "IDENTIFIER"){
        A();
        return true;
    }
    else if (lexemeVec[vecIndex] == "if") {
        If();
        return true;
    }
    else if (tokenVec[vecIndex] == "KEYWORD"){
      cout << "KEYWORD"<< lexemeVec[vecIndex]  << _address<< endl;
        dataType = lexemeVec.at(vecIndex);
      string _id = lexemeVec.at(vecIndex+1);
      vecIndex++;
      while(lexemeVec[vecIndex] != ";"){

        if(lexemeVec[vecIndex] != ","){

        symbolTable[start][0] = lexemeVec[vecIndex];
        symbolTable[start][1] = toString(_address);
        symbolTable[start][2] = dataType;
        _address++;

        }

    vecIndex++;
    start++;
      _id= lexemeVec.at(vecIndex);
  }
  return true;
}
    else if (lexemeVec[vecIndex] == "while") {
        While();
        return true;
    } else {
        return false;
    }

};

bool C() {

    cout << "<Condition> -> <Expression> <Relationship Operator> <Expression>" << endl;

    E();

    if (lexemeVec[vecIndex] == "=="    || lexemeVec[vecIndex] == "!="
        || lexemeVec[vecIndex] == "<"  || lexemeVec[vecIndex] == ">"
        || lexemeVec[vecIndex] == "<=" || lexemeVec[vecIndex] == ">=") {
          if(lexemeVec[vecIndex] == "<"){

              insertTable(address,"LES","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }
          if(lexemeVec[vecIndex] == ">"){
              insertTable(address,"GRT","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }
          if(lexemeVec[vecIndex] == "!="){
              insertTable(address,"NEQ","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }if(lexemeVec[vecIndex] == "=="){
              insertTable(address,"EQU","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }if(lexemeVec[vecIndex] == "<="){
              insertTable(address,"LEQ","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }
          if(lexemeVec[vecIndex] == ">="){
              insertTable(address,"GEQ","");
              jumpStack.push(address);
              insertTable(address,"JUMPZ","");


          }
        vecIndex++;
    } else return false;

    if(!E()) return false;

    return true;
};

bool A() { //  A  -> i = E


    cout << "<Assign> -> <IDENTIFIER> = <Expression>" << endl;
    if (tokenVec.at(vecIndex) == "IDENTIFIER") {
      save = lexemeVec.at(vecIndex);
        vecIndex++;
        print();

        if (lexemeVec.at(vecIndex) == "=") {
            vecIndex++;
            //insertTable(int & address, string op, int oprnd, string(&instructionTable)[300][3])
              E();
              insertTable(address,"POPM",getML(save));
            if (!E())
             return false;
        } else {
            std::cerr << "Expected =\n";
            return false;
        }
    } else {
        std::cerr << "Expected identifier\n";
        return false;
    }
    return true;
};

bool E() { //  E  -> TE'

    print();

    cout << "<Expression> -> <Term> <Expression Prime>" << endl;
    if (tokenVec.at(vecIndex) != "IDENTIFIER") {

      cout << tokenVec.at(vecIndex) << endl;
      if(tokenVec.at(vecIndex) == "INTEGER" || lexemeVec.at(vecIndex)=="true"||lexemeVec.at(vecIndex)=="false"){
        //insertTable(int & address, string op, string oprnd)
        if(tokenVec.at(vecIndex) == "INTEGER" ){
        insertTable(address,"PUSHI",lexemeVec.at(vecIndex));
        }
        if(lexemeVec.at(vecIndex)=="true"){
          insertTable(address,"PUSHI",toString(1));
        }
        if(lexemeVec.at(vecIndex)=="false"){
          insertTable(address,"PUSHI",toString(0));
        }
          epsilon();
          vecIndex++;
      }
        return false;
    }

    T();
    EPrime();
    return true;
};

//  E' -> +TE' | -TE' | ε
bool EPrime() {


    cout << "<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε" << endl;

    if (lexemeVec.at(vecIndex) == "+" || lexemeVec.at(vecIndex) == "-") {
      if(lexemeVec.at(vecIndex) == "+"){

      insertTable(address,"ADD","");
      }
      if(lexemeVec.at(vecIndex) == "-"){
      insertTable(address,"SUB","");
      }
      if(lexemeVec.at(vecIndex) == "*"){
      insertTable(address,"MUL","");
      }
        vecIndex++;
        print();

        if(!T())




        return false;
        EPrime();
        return true;
    } else {
        print();

        epsilon();
    }
    return false;
};

bool T() { //  T  -> FT'


    cout << "<Term> -> <Factor> <Term Prime>" << endl;
    if(tokenVec.at(vecIndex) != "IDENTIFIER") {
      cout << tokenVec.at(vecIndex);
        std::cerr << "Expected identifier\n";
        return false;
    }
    F();
    TPrime();

    return true;
};

bool TPrime() { //  T' -> *FT' | /FT' | ε


    cout << "<Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | ε" << endl;
    if (lexemeVec.at(vecIndex) == "*" || lexemeVec.at(vecIndex) == "/") {

        F();
        if(lexemeVec.at(vecIndex) == "*"){
        insertTable(address,"MUL","");
        }
        if(lexemeVec.at(vecIndex) == "/"){
        insertTable(address,"DIV","");
        }
        TPrime();
        return true;
    } else {
        epsilon();
    }
    return false;
};

bool F() { //  F -> i | (E)


    cout << "<Factor> -> " << lexemeVec.at(vecIndex) << endl;

    insertTable(address,"PUSHM",getML(lexemeVec.at(vecIndex)));

    vecIndex++;
    print();



    return false;
};


bool If() {


    cout << "<If> -> If ( <Condition> ) { <Statement> }" << endl;
    if (lexemeVec.at(vecIndex) == "if") {
        vecIndex++;
        print();

        if (lexemeVec.at(vecIndex) == "(") {
            vecIndex++;

            if (!C()) return false;


            if (lexemeVec.at(vecIndex) == ")") {
                vecIndex++;
                print();
                S();

                if (lexemeVec.at(vecIndex) == "{") {
                    vecIndex++;

                    print();
                    S();
                    vecIndex++;
                    backPatch(address,jumpStack);
                    if (lexemeVec.at(vecIndex) == "}") {
                        vecIndex++;

                        Else();
                        if(lexemeVec.at(vecIndex)=="endif")
                        return true;
                    }

                }

            }
        }
    }
    return false;
};


bool Else() {
    print();

    cout << "<While> -> While ( <Condition> ) { <Statement> }" << endl;
    if (lexemeVec.at(vecIndex) == "else") {
        vecIndex++;
        print();

        if (lexemeVec.at(vecIndex) == "{") {
            vecIndex++;
            if(!S()) return false;


            if (lexemeVec.at(vecIndex) == "}") {
                vecIndex++;
                return true;
            }
        }
    } else {
        vecIndex--;
        epsilon();
    }
    return false;
};

bool While() {
    print();

    cout << "<While> -> While ( <Condition> ) { <Statement> }" << endl;
    if (lexemeVec.at(vecIndex) == "while") {
        vecIndex++;
        print();

        if (lexemeVec.at(vecIndex) == "(") {
            vecIndex++;
            print();

            if (!C()) return false;

            if (lexemeVec.at(vecIndex) == ")"){
                vecIndex++;

                if (lexemeVec.at(vecIndex) == "{") {
                    print();
                    vecIndex++;
                    if (!S()) return false;

                    vecIndex++;
                    if (lexemeVec.at(vecIndex) == "}") return true;
                }
            }
        }
    }
    return false;
};


void epsilon () {

    cout << "<Empty> -> ε case" << endl;

};
string toString(int num){
  stringstream ss;
  ss << num;
  return ss.str();
}
void insertTable(int & address, string op, string oprnd)
{
  instructionTable[address][0] = toString(address);
  cout << instructionTable[address][0] << endl;
  instructionTable[address][1] = op;
  cout << instructionTable[address][1] << endl;
  instructionTable[address][2] = oprnd;
  cout << instructionTable[address][2] << endl;
  address++;
}


void print(){

        cout << "\nToken: " << tokenVec.at(vecIndex) << "\tLexeme: " << lexemeVec.at(vecIndex) << endl;



}
string getML(string id){
  string memoryLocation;
cout << id << endl;
  for(int i =0; i < 10; i ++){
    for(int j =0; j < 3; j++){
      if(symbolTable[i][j]==id)
      {
        memoryLocation =symbolTable[i][1];
      }

    }
  }
  return memoryLocation;
}
void printSymbol(){
  cout << "Symbol Table" << endl;
  cout << "IDENTIFIER\t\t" << "MemoryLocation\t\t" << "Type\t\t" << endl;
  for(int i =0; i < 10; i ++){
    for(int j =0; j < 3; j++){

        cout <<symbolTable[i][j] << "\t\t\t";
    }
    cout << endl;
  }
}
void printInstruction(){
    cout << "Instruction Table" << endl;
  for(int i =0; i < 20; i ++){
    for(int j =0; j < 3; j++){

        cout <<instructionTable[i][j] << "\t\t\t";
    }
    cout << endl;
  }
}

void backPatch(int jumpAddress,stack<int> & jumpStack){
  int address = jumpStack.top();

  instructionTable[address][2].clear();
  instructionTable[address][2] = toString(jumpAddress);
}
