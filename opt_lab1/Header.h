#pragma once
#include <vector>
#include <string>
struct InfoT {
	std::string name;
	int num;
};
struct LexPos {
	int code;
	int row_num;
	int col_num;
};
struct ErrStr {
	int name;
	int row;
	int col;
};

typedef std::vector<LexPos> lexem;
typedef std::vector<InfoT> identifier;
typedef std::vector<InfoT> keywordd;
typedef std::vector<InfoT> delimiter;


void EOFerror(int row, int col);
void Error(int sym, int row, int col);
void Output(const char *fname, const char *gener, std::vector<InfoT>& id, std::vector<InfoT>& key, std::vector<InfoT>& del, std::vector<LexPos>& lex);
void SymbolCategories();
void InitTables();
void FileReading(const char *fname);