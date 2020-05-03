#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <string>

#include "Header.h"

using namespace std;
int SymCat[128];

extern lexem lex;
extern identifier identif;
extern keywordd keyword;
extern delimiter delim;

void SymbolCategories() {
	int i = 0;
	while (i < 128) {
		if ((i < 14 && i > 7) || i == 32)
			SymCat[i] = 0;//ws
		else if (i == 40)										// (
			SymCat[i] = 5;//com
		else if (i == 41 || i == 58 || i == 59 || i == 44)		// ):;,
			SymCat[i] = 3;//dm
		else if ((i < 91 && i > 64) || (i < 123 && i > 96))		// A-Z a-z
			SymCat[i] = 2;//lt
		else if (i < 58 && i > 47)								// 0-9
			SymCat[i] = 1;//dg
		else
			SymCat[i] = 6;//er
		i++;
	}
}
void InitTables() {
	InfoT id;
	id.name = "SIGNAL";
	id.num = 1001;
	identif.push_back(id);
	id.name = "COMPLEX";
	id.num = 1002;
	identif.push_back(id);
	id.name = "INTEGER";
	id.num = 1003;
	identif.push_back(id);
	id.name = "FLOAT";
	id.num = 1004;
	identif.push_back(id);
	id.name = "BLOCKFLOAT";
	id.num = 1005;
	identif.push_back(id);
	id.name = "EXT";
	id.num = 1006;
	identif.push_back(id);
	InfoT key;
	key.name = "PROGRAM";
	key.num = 401;
	keyword.push_back(key);
	key.name = "BEGIN";
	key.num = 402;
	keyword.push_back(key);
	key.name = "END";
	key.num = 403;
	keyword.push_back(key);
	key.name = "PROCEDURE";
	key.num = 404;
	keyword.push_back(key);
	InfoT dm;
	dm.name = ";";
	dm.num = 59;
	delim.push_back(dm);
	dm.name = ")";
	dm.num = 41;
	delim.push_back(dm);
	dm.name = ":";
	dm.num = 58;
	delim.push_back(dm);
	dm.name = ",";
	dm.num = 44;
	delim.push_back(dm);
}
void LexRow(int num, int row, int col) {
	LexPos elem;
	elem.code = num;
	elem.row_num = row;
	elem.col_num = col;
	lex.push_back(elem);
}
int Compar(vector<char> &id) {
	InfoT idnew;
	int i = 0, size = id.size(), number;
	bool same = false;
	char *temp = new char[size+1];
	while (i < size) {
		temp[i] = id[i];
		i++;
	}
	temp[size] = '\0';
	i = 0;
	while (i < keyword.size()) {
		if (string(temp) == keyword[i].name) { //register???
			number = keyword[i].num;
			delete[] temp;
			return number;
		}
		i++;
	}
	i = 0;
	while (i < identif.size()) {
		if (string(temp) == identif[i].name) {
			number = identif[i].num;
			delete[] temp;
			return number;
		}
		i++;
	}
	
	idnew.name = string(temp);
	idnew.num = identif.size() + 1001;
	number = idnew.num;
	identif.push_back(idnew);
	delete[] temp;
	return number;
} 
void FileReading(const char *fname) {
	int i, s, row = 1, col = 0, idn = 0, del = 0, number, first, f = 0;
	int combrack = 0, com = 0, ecom = 0; //comment
	int error = 0, eofrow = 0, eofcol = 0; //errors
	vector<char>idlex;
	ifstream file(fname);
	if (file.is_open()) {
		while ((s = file.get()) != EOF) { //inp/exit
			col++; 
			if (SymCat[s] == 5 && com == 0) { //(
				combrack = 1;
				if (f == 0) {
					number = Compar(idlex);
					LexRow(number, row, first);
					idlex.clear();
				}
				idn = 0;
				continue;
			}
			if (combrack == 1) { //*
				combrack = 0;   
				if (s == 42) {
					com = 1;
					eofrow = row; //opened comment
					eofcol = col;
					continue;
				}
				else {
					LexRow(40, row, col - 1);
				}
			}
			if (com == 1) { //*
				if (s != 42) {
					if (s == 10) {
						col = 0;
						row++;
					}
					continue;
				}
				else {
					ecom = 1;
					com = 0;
					continue;
				}
			}
			if (ecom == 1) { //)
				if (s != 41) {
					ecom = 0;
					com = 1;
					if (s == 42) {
						ecom = 1;
						com = 0;
					}
					if (s == 10) {
						col = 0;
						row++;
					}
					continue;
				}
				else {
					com = 0;
					ecom = 0;
					eofrow = 0;
					eofcol = 0;
					continue;
				}
			}
			if (SymCat[s] == 6 || (SymCat[s] == 1 && idn == 0)) { //error symb
				Error(s, row, col);
				error = 1;
			}
			if (SymCat[s] == 2 || (SymCat[s] == 1 && idn == 1)) { //idn
				if (error == 0) {
					f = 0;
					if (idn == 0)
						first = col;
					idn = 1;
					idlex.push_back(char(s));
				}
			}
			if (SymCat[s] == 3) {
				del = 1;
			}
			if (SymCat[s] == 0 || SymCat[s] == 3 || SymCat[s] == 5 ) { //out
				f = 1;
				if (idn == 1 && error == 0) {
					number = Compar(idlex);
					LexRow(number, row, first);
					idlex.clear();
					idn = 0;
				}
				if (SymCat[s] == 0)
					error = 0;
				if (del == 1) {
					LexRow(s, row, col);
					del = 0;
					error = 0;
				}
				if (s == 10) {
					col = 0;
					row++;
					error = 0;
				}
			}
		}
		if (eofrow != 0) {
			EOFerror(eofrow, eofcol - 1);
			eofrow = 0;
			eofcol = 0;
		}
		cout << fname << " is opened" << endl;
		file.close();
	}
	else
		cout << "file is not opened" << endl;
}
