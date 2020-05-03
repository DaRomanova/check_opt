#include "Header.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <vector>

 lexem lex;
 identifier identif;
 keywordd keyword;
 delimiter delim;
  
using namespace std;
int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "the program requires arguments: " << endl << "tests\\testXX" << endl << "where XX - 01, 02, 03, etc." << endl;
	}
	else {
		SymbolCategories();
		string input, output, s1 = "\\input.sig", s2 = "\\generated.sig";
		for (int i = 1; i < argc; i++) {
			InitTables();
			input = argv[i] + s1;
			FileReading(input.c_str());
			output = argv[i] + s2;
			Output(input.c_str(), output.c_str(), identif, keyword, delim, lex);
		}
	}
	_getch();
	return 0;
} 