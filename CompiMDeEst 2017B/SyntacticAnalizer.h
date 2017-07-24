#pragma once
#include "LexicAnalizer.h"
#include <string>
#include "SymbolTable.h"
class SyntacticAnalizer
{
	SymbolTable symTable;
	enum STATE
	{
		MAIN_PROG,
		VAR_DECLARTION,

	};
	std::string m_context;
	void ReadVarDeclRecur();
	bool ReadFunctionPropCall();
	void ReadParamList();
	void ReadParamSet();
	void ReadPassParamList();
	void ReadAsignation();
	void ReadDimension();
	void ReadExpr();
	void ReadTerm();
	void ReadOper();
	void ReadFunction();
	void ReadProcedure();
	void ReadBlock();
	void ReadFuncBlock();
	void ReadExprBlock();
	void ReadStatement();

	void PanicMode();
	void PushSymbolTambleErrors();
	LexicAnalizer* m_lexicAnalizer;
	int index;
	std::pair<std::string, LexicAnalizer::ETokenType> actualTok;
	void SaveFile();
public:
	std::vector<std::string> m_set;
	std::string actualString;
	void Analize(LexicAnalizer& lexicAnalizer);
	SyntacticAnalizer();
	~SyntacticAnalizer();
};

