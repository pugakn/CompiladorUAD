#include "stdafx.h"
#include "SyntacticAnalizer.h"
#include <iostream>
#include <fstream>
#include "ErrorModule.h"
using namespace std;

void SyntacticAnalizer::ReadVarDeclRecur()
{
	ReadParamSet();
	 
}

void SyntacticAnalizer::ReadFunctionPropCall()
{
	//auto tok = m_lexicAnalizer->GetToken();
	//if (tok.second == LexicAnalizer::ETokenType::ID)
	//{
		//actualString += tok.first;
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == "(")
		{
			actualString += actualTok.first;
			ReadPassParamList();
			//actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == ")")
			{
				actualString += actualTok.first;
				actualTok = m_lexicAnalizer->GetToken();
				if (actualTok.first == ";")
				{
					actualString += actualTok.first;
				}
			}
		}
	//}
}

void SyntacticAnalizer::ReadParamList()
{
	if (m_lexicAnalizer->m_tokens[index].second == LexicAnalizer::ETokenType::ID)
	{
		index++;
		if (m_lexicAnalizer->m_tokens[index].first == ":")
		{
			index++;
			if (m_lexicAnalizer->m_tokens[index].second == LexicAnalizer::ETokenType::KEYWORD)//Type
			{
				index++;
				if (m_lexicAnalizer->m_tokens[index].first == ",")
				{
					index++;
					ReadParamList();
				}
			}
		}
	}
	else if (m_lexicAnalizer->m_tokens[index].first == ",")
	{
		index++;
		ReadParamList();
	}
}

void SyntacticAnalizer::ReadParamSet()
{
	actualTok= m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID)
	{
		actualString += actualTok.first;
		actualTok= m_lexicAnalizer->GetToken();
		if (actualTok.first == ":")
		{
			actualString += actualTok.first;
			actualTok= m_lexicAnalizer->GetToken();
			if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
			{
				actualString += actualTok.first;
				actualTok= m_lexicAnalizer->GetToken();
				if (actualTok.first == ";")
				{
					actualString += actualTok.first;
				}
				else
					ErrorModule::PushError(Error::LEXIC, 0, "Se esperaba ;", actualString);
			}
			else
				ErrorModule::PushError(Error::LEXIC, 0, "Se esperaba un tipo de variable", actualString);
		}
		else if (actualTok.first == ",")
		{
			actualString += actualTok.first;
			ReadParamSet();
		}
		else
			ErrorModule::PushError(Error::LEXIC, 0, "Se esperaba :", actualString);
	}
	else
		ErrorModule::PushError(Error::LEXIC, 0, "Se esperaba ID", actualString);
}

void SyntacticAnalizer::ReadPassParamList()
{
	actualTok= m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID)
	{
		actualString += actualTok.first;
		actualTok= m_lexicAnalizer->GetToken();
		if (actualTok.first == ",")
		{
			actualString += actualTok.first;
			ReadPassParamList();
		}
	}
}

void SyntacticAnalizer::ReadAsignation()
{
	ReadDimension();
	if (actualTok.first == "=")
	{
		actualString += actualTok.first;
		ReadExpr();
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ";")
		{
			actualString += actualTok.first;
		}
	}
}

void SyntacticAnalizer::ReadDimension()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "[")
	{
		actualString += actualTok.first;
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.second == LexicAnalizer::ETokenType::INT)
		{
			actualString += actualTok.first;
			actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == "]")
			{
				actualString += actualTok.first;
				ReadDimension();
			}
		}
	}
}

void SyntacticAnalizer::ReadExpr()
{
	do {
		ReadTerm();
		ReadOper();
	} while (actualTok.second == LexicAnalizer::ETokenType::ID || actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.first == "(");

}

void SyntacticAnalizer::ReadTerm()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "(")
	{
		actualString += actualTok.first;
		actualTok = m_lexicAnalizer->GetToken();
	}
	if (actualTok.second == LexicAnalizer::ETokenType::INT)
	{
		actualString += actualTok.first;
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ")")
		{
			actualString += actualTok.first;
		}
	}
	else if (actualTok.second == LexicAnalizer::ETokenType::ID)//Dimen && func
	{
		actualString += actualTok.first;
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ")")
		{
			actualString += actualTok.first;
		}
		else if (actualTok.first == "(")
		{
			actualString += actualTok.first;
			actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == ")")
			{
				actualString += actualTok.first;
				actualTok = m_lexicAnalizer->GetToken();
				if (actualTok.first == ")")
				{
					actualString += actualTok.first;
				}
			}
		}
	}
}

void SyntacticAnalizer::ReadOper()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
		|| actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
		|| actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
	{
		actualString += actualTok.first;
	}
}

void SyntacticAnalizer::SaveFile()
{
	ofstream file;
	file.open("tokens.syn");
	for (auto &it : m_set)
	{
		file  << "\t" << it << endl;
	}
	file.close();
}

void SyntacticAnalizer::Analize(LexicAnalizer & lexicAnalizer)
{
	index = 0;
	m_lexicAnalizer = &lexicAnalizer;
	//Leer Vars
	while (true)
	{
		actualString.clear();
		actualTok= m_lexicAnalizer->GetToken();
		if (actualTok.first == "var")
		{
			actualString += actualTok.first;
			index++;
			ReadVarDeclRecur();
			m_set.push_back(actualString);
		}
		else
			break;
		if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size()-1)
			break;
	}
	//Leer FuncProcs
	while (true)
	{
		actualString.clear();
		if (m_lexicAnalizer->TokenIndex != m_lexicAnalizer->m_tokens.size() - 1 && actualTok.first != "main")
		{
			if (actualTok.second == LexicAnalizer::ETokenType::ID)
			{
				actualString += actualTok.first;
				ReadFunctionPropCall();
				m_set.push_back(actualString);
			}
		}
		else break;
		actualTok = m_lexicAnalizer->GetToken();
	}


	SaveFile();
}

SyntacticAnalizer::SyntacticAnalizer()
{
}


SyntacticAnalizer::~SyntacticAnalizer()
{
}
