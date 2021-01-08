#pragma once
#include<vector>
#include<string>

class Lexer
{
	private:
		int current_state, col_num;
		bool hasLexeme;
		char input;
		std::string lexeme_buffer, lexeme, token_buffer, token;
		void checkLabelToken();
		
	public:
		Lexer() {
			current_state = 0;
			col_num = 0;
			hasLexeme = false;
			input = 0;
			lexeme_buffer = "";
			lexeme = "";
			token_buffer = "";
			token = "";
		}
		void char_input(char input_buff);
		bool checkLexeme();
		std::string get_lexeme();
		std::string get_token();
};

void Lexer::char_input(char input_buff)
{
	input = input_buff;

	//letters: a-z
	if (input_buff >= 97 && input_buff <= 122)
		col_num = 0;

	//numbers: 0-9
	else if (input_buff >= 48 && input_buff <= 57)
		col_num = 1;

	//operators: % * + - / < = >
	else if (input_buff == 37 || (input_buff >= 42 && input_buff <= 43) || input_buff == 45 || input_buff == 47 ||
		(input_buff >= 60 && input_buff <= 62))
		col_num = 2;

	//separators: TAB NL SPACE ' ( ) , : ; [ ] { }
	//NOTE: added ASCII 13 (carriage return); bug fix where ASCII 13 is included token is set to UNKNOWN
	else if (input_buff == 9 || input_buff == 10 || input_buff == 13 || input_buff == 32 || (input_buff >= 39 && input_buff <= 41) || input_buff == 44 ||
		(input_buff >= 58 && input_buff <= 59) || input_buff == 91 || input_buff == 93 || input_buff == 123 || input_buff == 125)
		col_num = 3;

	//block comment: !
	else if (input_buff == 33)
		col_num = 4;

	//separator or integer-to-float: .
	else if (input == 46)
		col_num = 5;

	//identifier: $
	else if (input_buff == 36)
		col_num = 6;

	//unknown
	else
		col_num = 7;
}

bool Lexer::checkLexeme()
{
	std::vector<std::vector<int>> table
	{
		{1,3,8,9,6,9,10,10},
		{1,2,11,11,11,11,2,10},
		{2,2,11,11,11,11,2,10},
		{10,3,11,11,11,4,10,10},
		{10,5,10,10,10,10,10,10},
		{10,5,11,11,11,11,10,10},
		{6,6,6,6,7,6,6,6},
		{11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11},
		{11,11,11,11,11,11,11,11},
		{10,10,11,11,11,11,10,10},
		{0,0,0,0,0,0,0,0}
	};

	do{
		current_state = table[current_state][col_num];
		switch (current_state) {
		case 0:
			//starting state
			break;
		case 1:
			token_buffer = "LABEL";
			lexeme_buffer.push_back(input);
			break;
		case 2:
			token_buffer = "IDENTIFIER";
			lexeme_buffer.push_back(input);
			break;
		case 3:
			token_buffer = "INTEGER";
			lexeme_buffer.push_back(input);
			break;
		case 4:
			lexeme_buffer.push_back(input);
			break;
		case 5:
			token_buffer = "FLOAT";
			lexeme_buffer.push_back(input);
			break;
		case 6:
			//start block comment
			break;
		case 7:
			//end block comment
			break;
		case 8:
			token_buffer = "OPERATOR";
			lexeme_buffer.push_back(input);
			break;
		case 9:
			token_buffer = "SEPARATOR";
			lexeme_buffer.push_back(input);
			break;
		case 10:
			token_buffer = "UNKNOWN";
			lexeme_buffer.push_back(input);
			break;
		case 11:
			//ending state
			current_state = 0;
			if (!lexeme_buffer.empty()) {
				if (lexeme_buffer[0] <= 32) //ignore BLANKS
					lexeme_buffer.pop_back();
				else{
					lexeme = lexeme_buffer;
					lexeme_buffer = "";
					token = token_buffer;
					token_buffer = "";
					hasLexeme = true;
				}
			}
			break;
		default:
			break;
		}
	} while (current_state == 0);

	if (hasLexeme)
		checkLabelToken();

	return hasLexeme;
}

void Lexer::checkLabelToken()
{
	if (token == "LABEL") {
		std::vector<std::string> keywords{
			"int", "float", "bool", "if", "else", "then", "do", "while", "whileend", "doend", "for", "and", "or", "function", "return"
		};

		//token = identifier if it's not a keyword
		token = "IDENTIFIER";

		for (int x = 0; x < (int)keywords.size(); ++x)
			if (lexeme == keywords[x]) {
				token = "KEYWORD";
				break;
			}
	}
}

std::string Lexer::get_lexeme()
{
	hasLexeme = false;
	return lexeme;
}

std::string Lexer::get_token()
{
	return token;
}
