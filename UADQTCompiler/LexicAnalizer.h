#ifndef LEXIC_H
#define LEXIC_H

#include <string>
#include <vector>
class LexicAnalizer
{
public:
	enum ETokenType
	{
		ID,
		KEYWORD,
		LOGIC_OP,
		ASIGN_OP,
		RELATIONAL_OP,
		ARITHMETIC_OP,
		INT,
		FLOAT,
		BOOL,
		GROUP_OP,
		INDEX_OP,
		DELIMITER_OP,
		SEPARATOR_OP,
		STRING,

	};
	enum EStates
	{
		INITIAL_STATE,
		END_STATE,
		ID_STATE,
		INT_STATE,
		FLOAT_STATE,
		STRING_STATE,
		OP_LOG_AND_STATE,
		OP_LOG_OR_STATE,
		OP_LOG_NOT_STATE,
		OP_ARIT_STATE,
		OP_ARIT_DIVISION_STATE,
		OP_REL_STATE,
		OP_REL_EQUAL_STATE,
		ASIGN_STATE,
		OP_GROUP_STATE,
		OP_INDEX_STATE,
		DELIMITER_STATE,
		KEYWORD_STATE,
		SEPARATOR_STATE, 
		COMENT_STATE
	}; 
private:
    std::vector<std::string> keywords;
    std::vector<std::string> typeString;
	EStates m_state;
	
	void StateEnter();
	void StateExit();
	void SetState(EStates state);
	std::string tokenString;
	char* ptr;
	bool incrementPtr;
	ETokenType actualType;
	int actualLine;
	void SaveFiles();
public:
	std::vector<std::pair<std::string, ETokenType>> m_tokens;
	std::pair<std::string, LexicAnalizer::ETokenType> GetToken();
	int TokenIndex;
	void Parse(char* code);
	void Print();
	LexicAnalizer();
	~LexicAnalizer();
};
#endif

