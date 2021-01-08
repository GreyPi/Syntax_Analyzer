#include<iostream>
#include<fstream>
#include"lexer.h"
#include"Analyzer.h"

using namespace std;

int main(int argc, char **argv)
{
	bool next_lexeme;
	ofstream outputFile;
	ifstream inputFile;
	Analyzer syntactic;
	Lexer lexer;
	char inChar;
	
	inputFile.open("input.txt");
	outputFile.open("output.txt");

	do{
		inputFile.get(inChar);
		lexer.char_input(inChar);
		if (lexer.checkLexeme()) {
		  syntactic.addTokenLex_toPrint("\nToken:\t" + lexer.get_token() + "\t\t" + "Lexeme:\t  " + lexer.get_lexeme() + "\n  ");
			next_lexeme = false;
			while(!next_lexeme) {
				next_lexeme = syntactic.start(lexer.get_token(), lexer.get_lexeme());
			}
		}
	} while(!inputFile.eof());

	cout << syntactic.printout();
	outputFile << syntactic.printout();
	inputFile.close();
	outputFile.close();
	cout << endl;
	return 0;
}
