#include "stdafx.h"
#include "LexicAnalizer.h"
#include "ErrorModule.h"
#include <iostream>
#include <fstream>
using namespace std;
void LexicAnalizer::StateEnter()
{
	if (m_state != LexicAnalizer::INITIAL_STATE)
	{
		tokenString.push_back(*ptr);
		if (!(*(ptr + 1) >= '0' && *(ptr + 1) <= '9') && m_state == LexicAnalizer::FLOAT_STATE)
		{
			ErrorModule::PushError(Error::LEXIC, actualLine, "Se esperaba un numero flotante después del punto" , tokenString);
			m_state = LexicAnalizer::INITIAL_STATE;
			tokenString.clear(); 
		}
	}
	else
	{
		incrementPtr = false;
		m_tokens.push_back(std::make_pair(tokenString, actualType));
		tokenString.clear();
	}
} 
void LexicAnalizer::StateExit()
{
}
void LexicAnalizer::SetState(EStates state)
{
	StateExit();
	m_state = state;
	StateEnter();
}
void LexicAnalizer::SaveFiles()
{
	ofstream file;
	file.open("tokens.lex");
	for (auto &it : m_tokens)
	{
		file << it.first << "\t" << typeString[ it.second] <<endl;
	}
	file.close();
}
void LexicAnalizer::Parse(char * code)
{
	m_tokens.clear();
	tokenString.clear();
	actualLine = 1;
	m_state = LexicAnalizer::INITIAL_STATE;
	ptr = code;
	while (true)
	{
		incrementPtr = true;
		switch (m_state)
		{
		case LexicAnalizer::EStates::INITIAL_STATE:
			if (*ptr >= '0' && *ptr <= '9') {
				actualType = ETokenType::INT;
				SetState(LexicAnalizer::EStates::INT_STATE);
			}
			else if (*ptr >= 'a' && *ptr <= 'z'
				|| *ptr >= 'A' && *ptr <= '>'
				|| *ptr == '_' || *ptr == 'ñ' || *ptr == 'Ñ') {
				actualType = ETokenType::ID;
				SetState(LexicAnalizer::EStates::ID_STATE);
			}
			else if (*ptr == '"')
			{
				actualType = ETokenType::STRING;
				SetState(LexicAnalizer::EStates::STRING_STATE);

			}
			else if (*ptr == '+' || *ptr == '-' ||  *ptr == '*' || *ptr == '%' || *ptr == '^') {
				actualType = ETokenType::ARITHMETIC_OP;
				SetState(LexicAnalizer::EStates::OP_ARIT_STATE);
			}
			else if ( *ptr == '/' ) {
				actualType = ETokenType::ARITHMETIC_OP;
				SetState(LexicAnalizer::EStates::OP_ARIT_DIVISION_STATE);
			}
			else if (*ptr == '&') {
				actualType = ETokenType::LOGIC_OP;
				SetState(m_state = LexicAnalizer::EStates::OP_LOG_AND_STATE);
			}
			else if (*ptr == '|') {
				actualType = ETokenType::LOGIC_OP;
				SetState(LexicAnalizer::EStates::OP_LOG_OR_STATE);
			}
			else if (*ptr == '!') {
				actualType = ETokenType::LOGIC_OP;
				SetState(LexicAnalizer::EStates::OP_LOG_NOT_STATE);
			}
			else if (*ptr == '<' || *ptr == '>') {
				actualType = ETokenType::RELATIONAL_OP;
				SetState(LexicAnalizer::EStates::OP_REL_STATE); 
			}
			else if (*ptr == '=')
			{
				actualType = ETokenType::ASIGN_OP;
				SetState(LexicAnalizer::EStates::ASIGN_STATE);
			}
			else if (*ptr == '(' || *ptr == '{' || *ptr == ')' || *ptr == '}') {
				actualType = ETokenType::GROUP_OP;
				SetState(LexicAnalizer::EStates::OP_GROUP_STATE);
			}
			else if (*ptr == ';' || *ptr == ':') {
				actualType = ETokenType::DELIMITER_OP;
				SetState(LexicAnalizer::EStates::DELIMITER_STATE);
			}
			else if (*ptr == ',') {
				actualType = ETokenType::SEPARATOR_OP;
				SetState(LexicAnalizer::EStates::SEPARATOR_STATE);
			}
			else if (*ptr == '[' || *ptr == ']') {
				actualType = ETokenType::INDEX_OP;
				SetState(LexicAnalizer::EStates::OP_INDEX_STATE);
			}
			else {
				if (*ptr == '\n' )
				{
					actualLine++;
				}
				else if (*ptr == '\r')
				{
					
				}
				else if (*ptr == '.')
				{
					string temp;
					temp.push_back(*ptr);
					ErrorModule::PushError(Error::LEXIC, actualLine, "Se esperaba un numero flotante antes del punto", temp);
				}
				else if(*ptr != ' ' && *ptr != '\t')
				{
					string temp;
					temp.push_back(*ptr);
					ErrorModule::PushError(Error::LEXIC, actualLine, "Caracter invalido", temp);
				}
			}
			break;
		case LexicAnalizer::EStates::SEPARATOR_STATE:
			SetState( LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::EStates::OP_INDEX_STATE:
			SetState( LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::DELIMITER_STATE:
			SetState ( LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::OP_ARIT_STATE:
			SetState ( LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::OP_GROUP_STATE:
			SetState ( LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::STRING_STATE:
			tokenString.push_back(*ptr);
			if (*ptr == '"') {
				SetState(LexicAnalizer::EStates::INITIAL_STATE);
				ptr++;
			}
			break;
		case LexicAnalizer::OP_ARIT_DIVISION_STATE:
			if (*ptr == '*' )
			{
				m_state = LexicAnalizer::COMENT_STATE;
				tokenString.clear();
			}
			else {
				SetState(LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::COMENT_STATE:
			if (*ptr == '*')
			{
				if (*(ptr + 1) == '/') {
					m_state = LexicAnalizer::INITIAL_STATE;
					ptr++;
				}
			}
			break;
		case LexicAnalizer::EStates::ID_STATE:
			if (*ptr >= 'a' && *ptr <= 'z'
				|| *ptr >= 'A' && *ptr <= 'Z'
				|| *ptr == '_'
				|| (*ptr >= '0' && *ptr <= '9') || *ptr =='ñ' || *ptr =='Ñ')
			{
				tokenString.push_back(*ptr);
			}
			else
			{
				incrementPtr = false;
				for (size_t i = 0; i < 18; ++i)
				{
					if (tokenString == keywords[i])
					{
						actualType = ETokenType::KEYWORD;
						break;
					}
				}
				SetState( LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::EStates::KEYWORD_STATE:
			break;
		case LexicAnalizer::EStates::INT_STATE:
			if (*ptr >= '0' && *ptr <= '9')
			{
				tokenString.push_back(*ptr);
			}else
			if (*ptr == '.')
			{
				actualType = ETokenType::FLOAT;
				SetState ( LexicAnalizer::EStates::FLOAT_STATE);
			}
			else
			{
				SetState (LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::EStates::FLOAT_STATE:
			if (*ptr >= '0' && *ptr <= '9')
			{
				tokenString.push_back(*ptr);
			}
			else
			{
				SetState(LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::EStates::OP_LOG_AND_STATE:
			if (*ptr == '&') {
				tokenString.push_back(*ptr);
				SetState(LexicAnalizer::EStates::INITIAL_STATE);
				incrementPtr = true;
			}
			else
			{
				ErrorModule::PushError(Error::LEXIC,actualLine,"Se esperaba &", tokenString);
				tokenString.clear();
				m_state = LexicAnalizer::INITIAL_STATE;
				incrementPtr = false;
			}
			break;
		case LexicAnalizer::EStates::OP_LOG_OR_STATE:
			if (*ptr == '|') {
				tokenString.push_back(*ptr);
				SetState(LexicAnalizer::EStates::INITIAL_STATE);
				incrementPtr = true;
			}
			else
			{
				ErrorModule::PushError(Error::LEXIC, actualLine, "Se esperaba |", tokenString);
				tokenString.clear(); 
				m_state = LexicAnalizer::INITIAL_STATE;
				incrementPtr = false;
			}
			break;
		case LexicAnalizer::EStates::OP_LOG_NOT_STATE:
			if (*ptr == '=') {
				actualType = ETokenType::RELATIONAL_OP;
				SetState (LexicAnalizer::EStates::OP_REL_STATE);
			}
			else
			{
				SetState ( LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::EStates::OP_REL_STATE:
			if (*ptr == '=')
			{
				tokenString.push_back(*ptr);
				SetState (LexicAnalizer::EStates::INITIAL_STATE);
				incrementPtr = true;
			}
			else
			{
				SetState (LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		case LexicAnalizer::OP_REL_EQUAL_STATE:
			SetState(LexicAnalizer::EStates::INITIAL_STATE);
			break;
		case LexicAnalizer::ASIGN_STATE:
			if (*ptr == '=')
			{
				actualType = ETokenType::RELATIONAL_OP;
				SetState (LexicAnalizer::EStates::OP_REL_EQUAL_STATE);
			}
			else
			{
				SetState ( LexicAnalizer::EStates::INITIAL_STATE);
			}
			break;
		default:
			break;
		}
		if (*ptr == 0)
		{
			if (m_state == LexicAnalizer::STRING_STATE)
			{
				//Error no se cerro string
				ErrorModule::PushError(Error::LEXIC, actualLine, "Cadena sin cerrar", tokenString);
			}
			else if (m_state == LexicAnalizer::COMENT_STATE)
			{
				//Error no se cerro comentario
				ErrorModule::PushError(Error::LEXIC, actualLine, "Comentario sin cerrar", tokenString);
			}
			SaveFiles();
			return;
		}
		if (incrementPtr)
			ptr++;
	}
}

void LexicAnalizer::Print()
{
	for (auto &it : m_tokens)
	{
		cout << it.first <<"     " << typeString[it.second]  << endl;
	}
}

LexicAnalizer::LexicAnalizer()
{
}


LexicAnalizer::~LexicAnalizer()
{
}
