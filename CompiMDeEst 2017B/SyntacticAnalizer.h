#pragma once
#include "LexicAnalizer.h"
#include <string>
class SyntacticAnalizer
{
	enum STATE
	{
		MAIN_PROG,
		VAR_DECLARTION,

	};

	void ReadVarDeclRecur();
	void ReadFunctionPropCall();
	void ReadParamList();
	void ReadParamSet();
	void ReadPassParamList();
	void ReadAsignation();
	void ReadDimension();
	void ReadExpr();
	void ReadTerm();
	void ReadOper();
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

