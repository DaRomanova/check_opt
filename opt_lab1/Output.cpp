#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Header.h"
using namespace std;

vector<ErrStr>str;
int eofrow = 0, eofcol = 0;
void Output(const char *fname, const char *gener, vector<InfoT>& id, vector<InfoT>& key, vector<InfoT>& del, vector<LexPos>& lex) {
	ofstream out(gener);
	ifstream in(fname);
	char s;
	int i, j;
	if (in.is_open() && out.is_open()) {
		while ((s = in.get()) != EOF) {
			out << s;
		}
		i = 0;
		if (str.size() != 0) {
			while (i < str.size()) {
				out << endl << "Lexer: Error(line " << str[i].row << ", column " << str[i].col << "): Illegal symbol '" << char(str[i].name) << "'" << endl;
				i++;
			}
			str.clear();
		}
		if (eofrow != 0) {
			out << endl << "Lexer: Error(line " << eofrow << ", column " << eofcol << "): Opened comment "<< endl;
			eofrow = 0;
			eofcol = 0;
		}
		out << endl << "	Lexem table" << endl;
		i = 0;
		while (i < lex.size()) {
			j = 0;
			out << lex[i].row_num << "	" << lex[i].col_num << "	" << lex[i].code;
			if (lex[i].code > 1000) {
				while (id[j].num != lex[i].code) {
					j++;
				}
				out << "	" << id[j].name << endl;
			}
			else if (lex[i].code > 400) {
				j = 0;
				while (key[j].num != lex[i].code) {
					j++;
				}
				out << "	" << key[j].name << endl;
			}
			else {
				if (lex[i].code == 40) {
					out << "	(" << endl;
				}
				else {
					j = 0;
					while (del[j].num != lex[i].code) {
						j++;
					}
					out << "	" << del[j].name << endl;
				}
			}
			i++;
		}
		lex.clear();
		out << endl << "	Information tables" << endl;
		out << "	Identifier table" << endl;
		i = 0;
		while (i < id.size()) {
			out << id[i].name << "		" << id[i].num  << endl;
			i++;
		}
		id.clear();
		out << "	Keywords table" << endl;
		i = 0;
		while (i < key.size()) {
			out << key[i].name << "		" << key[i].num << endl;
			i++;
		}
		key.clear();
		out << endl << "	Delimeters table" << endl;
		i = 0;
		while (i < del.size()) {
			out << del[i].name << "	" << del[i].num << endl;
			i++;
		}
		del.clear();
		in.close();
		out.close();
	}
	else
		cout << "file is not opened" << endl;
}

void Error(int sym, int row, int col) {
	ErrStr er;
	er.name = sym;
	er.row = row;
	er.col = col;
	str.push_back(er);
}

void EOFerror(int row,  int col) {
	eofrow = row;
	eofcol = col;
}