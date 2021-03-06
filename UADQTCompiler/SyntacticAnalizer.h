#include "LexicAnalizer.h"
#include <string>
#include "SymbolTable.h"
class SyntacticAnalizer
{

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
    void ReadReturnExpr();
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
    bool noTrow = false;
	std::pair<std::string, LexicAnalizer::ETokenType> actualTok;

    std::vector<std::string> varsV;
    int actualDim;
	void SaveFile();
public:
    SymbolTable symTable;
	std::vector<std::string> m_set;
	std::string actualString;
	void Analize(LexicAnalizer& lexicAnalizer);
	SyntacticAnalizer();
	~SyntacticAnalizer();
};

