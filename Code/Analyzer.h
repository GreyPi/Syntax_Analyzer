#pragma once
#include<string>
#include<vector>
#include<stack>

class Analyzer
{
  private:
    std::string stack, output;
    std::string checkTable(std::string n_buff, std::string t_buff);
    int nonterminal(std::string inbuff);
    int terminal(std::string inbuff);
  public:
    bool start(std::string token, std::string lexeme);
    void addTokenLex_toPrint(std::string inbuff);
    std::string printout();
};

bool Analyzer::start(std::string token, std::string lexeme)
{
  std::string top_stack;

  if(stack.empty()) {
    stack.push_back('S');
    return false;
  }
  else{
    top_stack = stack.back();

    if(top_stack == " ") {
      stack.pop_back();
      return true;
    }
    else if(token == "IDENTIFIER") {
      if(top_stack != "i") {
        stack.pop_back();
        stack.append(checkTable(top_stack, "i"));
        top_stack = stack.back();
        if(top_stack == "e") {
          stack.pop_back();
        }
        return false;
      }
      else if(top_stack == "i") {
        stack.pop_back();
      }
    }
    else if(token == "OPERATOR" && terminal(lexeme) >= 0) {
      if(top_stack != lexeme) {
        stack.pop_back();
        stack.append(checkTable(top_stack, lexeme));
        top_stack = stack.back();
        if(top_stack == "e") {
          stack.pop_back();
        }
        return false;
      }
      else if(top_stack == lexeme) {
        stack.pop_back();
      }
    }
    else if(token == "SEPARATOR" && terminal(lexeme) >= 0) {
      if(top_stack != lexeme) {
        stack.pop_back();
        stack.append(checkTable(top_stack, lexeme));
        top_stack = stack.back();
        if(top_stack == "e") {
          stack.pop_back();
        }
        return false;
      }
      else if(top_stack == lexeme) {
        stack.pop_back();
      }
    }
  }
  return true;
}

int Analyzer::nonterminal(std::string inbuff)
{
  int result = -1;

  if(inbuff == "S")
    result = 0;
  else if(inbuff == "E")
    result = 1;
  else if(inbuff == "Q")
    result = 2;
  else if(inbuff == "T")
    result = 3;
  else if(inbuff == "R")
    result = 4;
  else if(inbuff == "F")
    result = 5;

  return result;
}

int Analyzer::terminal(std::string inbuff) {
  int result = -1;

  if(inbuff == "i")
    result = 0;
  else if(inbuff == "+")
    result = 1;
  else if(inbuff == "-")
    result = 2;
  else if(inbuff == "*")
    result = 3;
  else if(inbuff == "/")
    result = 4;
  else if(inbuff == "(")
    result = 5;
  else if(inbuff == ")")
    result = 6;
  else if(inbuff == "=")
    result = 7;
  else if(inbuff == ";")
    result = 8;
  else if(inbuff == "$")
    result = 9;

  return result;
}

std::string Analyzer::checkTable(std::string n_buff, std::string t_buff)
{
  std::vector<std::vector<std::string>> table {
    //            i  +   -   *   /   (   )   =   ;   $
    /* S */   {"i=E;", "", "", "", "", "", "", "", "", ""},
    /* E */   {"TQ", "", "", "", "", "TQ", "", "", "", ""},
    /* Q */  {"", "+TQ", "-TQ", "", "", "", "e", "", "e", ""},
    /* T */   {"FR", "", "", "", "", "FR", "", "", "", ""},
    /* R */  {"", "e", "e", "*FR", "/FR", "", "e", "", "e", ""},
    /* F */   {"i", "", "", "", "", "(E)", "", "", "", ""}
  };
  std::string production, reverse_production;

  if(nonterminal(n_buff) >= 0) {
    production = table[nonterminal(n_buff)][terminal(t_buff)];
    if(!production.empty()) {
      output.append("<"+ n_buff + "> --> " + production + "\n  ");
    }
    while(!production.empty()) {
        reverse_production.push_back(production.back());
        production.pop_back();
    }
  }
  else{
    return " ";
  }
  return reverse_production;
}

void Analyzer::addTokenLex_toPrint(std::string inbuff)
{
  output.append(inbuff);
}

std::string Analyzer::printout()
{
  return output;
}
