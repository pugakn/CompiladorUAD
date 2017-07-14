#include "stdafx.h"
#include "SyntacticAnalizer.h"
#include <iostream>
#include <fstream>
#include "ErrorModule.h"
using namespace std;

void SyntacticAnalizer::ReadVarDeclRecur()
{
	while (true)
	{
		actualTok = m_lexicAnalizer->GetToken();
		if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size() - 1)
			break;
		if (actualTok.first == "var")
		{
			ReadParamSet();
		}
		else
		{
			m_lexicAnalizer->TokenIndex--;
			break;
		}
		
	}
}

bool SyntacticAnalizer::ReadFunctionPropCall()
{
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
					return true;
				}
				else
				{
					PanicMode();
					ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ;", actualString);
				}
			}
			else
			{
				PanicMode();
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba )", actualString);
			}
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un (", actualString);
		}
		return false;
	//}
}

void SyntacticAnalizer::ReadParamList()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID)
	{
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ":")
		{
			actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
			{
				actualTok = m_lexicAnalizer->GetToken();
				if (actualTok.first == ",")
				{
					ReadParamList();
				}
			}
			else
			{
				PanicMode();
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo", actualString);
			}
		}
		else if (actualTok.first == ",")
		{
			ReadParamList();
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba :", actualString);
		}
	}
}
void SyntacticAnalizer::PanicMode()
{
	while (true)
	{
		if (actualTok.first == ";"  || actualTok.first == "}" )
		{
			break;
		}
		actualTok = m_lexicAnalizer->GetToken();
		if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size() - 1)
			break;
	}

}

void SyntacticAnalizer::ReadParamSet()
{
	//auto resp = m_lexicAnalizer->TokenIndex-1;

	actualTok= m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID)
	{
		std::string id = actualTok.first;
		actualString += actualTok.first;
		actualTok= m_lexicAnalizer->GetToken();
		if (actualTok.first == ":")
		{
			actualString += actualTok.first;
			actualTok= m_lexicAnalizer->GetToken();
			if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
			{
				std::string type = actualTok.first;
				actualString += actualTok.first;
				actualTok= m_lexicAnalizer->GetToken();
				if (actualTok.first == ";")
				{
					actualString += actualTok.first;
					if (m_context == "gvar")
					{
						SymbolTable::GlobalNode node;
						node.dimension = 0;
						node.type = "gvar";
						node.ptrVal = 0;
						node.name = id;
						node.type = type;

						symTable.AddGlobalNode(node);
					}
				}
				else
				{
					ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ;", actualString);
					//m_lexicAnalizer->TokenIndex = resp;
					PanicMode();
				}
			}
			else
			{
				//m_lexicAnalizer->TokenIndex = resp;
				PanicMode();
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo de variable", actualString);
			}
		}
		else if (actualTok.first == ",")
		{
			actualString += actualTok.first;
			ReadParamSet();
		}
		else
		{
			//m_lexicAnalizer->TokenIndex = resp;
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba :", actualString);
		}
	}
	else
	{
		//m_lexicAnalizer->TokenIndex = resp;
		PanicMode();
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ID", actualString);
	}
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
	//ReadDimension();
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "=")
	{
		actualString += actualTok.first;
		ReadExpr();
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ";")
		{
			actualString += actualTok.first;
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ;", actualString);
		}
	}
}

void SyntacticAnalizer::ReadDimension()
{
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
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un indice", actualString);
		}
	}
}

void SyntacticAnalizer::ReadExpr()
{
	do {
		actualTok = m_lexicAnalizer->GetToken();
		if (m_lexicAnalizer->TokenIndex < m_lexicAnalizer->m_tokens.size() - 1)
		{
			if (m_lexicAnalizer->m_tokens[m_lexicAnalizer->TokenIndex].first == "=")
			{
				ReadAsignation();
			}
			else if (m_lexicAnalizer->m_tokens[m_lexicAnalizer->TokenIndex].first == "(")
			{
				ReadFunctionPropCall();
			}
			else if (actualTok.first == "if" || actualTok.first == "while")
			{
				ReadStatement();
			}
			else
			{
				ReadTerm();
				ReadOper();
			}
		}
		else
		{
			ReadTerm();
			ReadOper();
		}
		
	} while (actualTok.second == LexicAnalizer::ETokenType::ID || actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.first == "(");

}

void SyntacticAnalizer::ReadStatement()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "(")
	{
		ReadExpr();
		if (actualTok.first == ")")
		{
			ReadBlock();
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un )", actualString);
		}
	}
	else
	{
		PanicMode();
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un (", actualString);
	}
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

void SyntacticAnalizer::ReadFunction()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID) {
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == "(")
		{
			ReadParamList();
			//actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == ")")
			{
				actualTok = m_lexicAnalizer->GetToken();
				if (actualTok.first == ":")
				{
					actualTok = m_lexicAnalizer->GetToken();
					if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
					{
						actualTok = m_lexicAnalizer->GetToken();
						ReadFuncBlock();
					}
					else
					{
						PanicMode();
						ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo", actualString);
					}
				}
				else
				{
					PanicMode();
					ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba :", actualString);
				}
			}
			else
			{
				PanicMode();
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba )", actualString);
			}
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba (", actualString);
		}
	}
	else
	{
		PanicMode();
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ID", actualString);
	}
}

void SyntacticAnalizer::ReadProcedure()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID) {
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == "(")
		{
			ReadParamList();
			//actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == ")")
			{
				ReadBlock();
			}
			else
			{
				PanicMode();
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba )", actualString);
			}
		}
		else
		{
			PanicMode();
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba (", actualString);
		}
	}
	else
	{
		PanicMode();
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ID", actualString);
	}
}

void SyntacticAnalizer::ReadBlock()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "{")
	{
		ReadVarDeclRecur();
		ReadExprBlock();
		if (actualTok.first == "}")
		{

		}
		//else
		//	ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba }", actualString);
	}
	else
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba {", actualString);
}
void SyntacticAnalizer::ReadFuncBlock()
{
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.first == "{")
	{
		ReadVarDeclRecur();
		ReadExprBlock();
		if (actualTok.first == "return")
		{
			while (true)
			{
				if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size() - 1)
				{
					ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba }", actualString);
					break;
				}
				actualTok = m_lexicAnalizer->GetToken();
				if (actualTok.first == "}")
				{
					break;
				}
			}

		}
	}
	else
		ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba {", actualString);
}


void SyntacticAnalizer::ReadExprBlock()
{
	while (true)
	{
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == "}" || actualTok.first == "return")
			break;
		//if (actualTok.second == )
		//{
			ReadExpr();
		//}
		//else
		//{
			//m_lexicAnalizer->TokenIndex--;
			//break;
		//}
		if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size() - 1)
			break;
	}
}

void SyntacticAnalizer::SaveFile()
{
	ofstream file;
	file.open("tokens.syn");
	for (auto &it : symTable.m_hashTable)
	{
		file << "\t" << it.second.name << "\t" << it.second.type << "\t" << it.second.varType << "\t" << it.second.dimension << "\t" << "context" << "\t" << it.second.ptrVal;
		for (auto &it2 : it.second.localNode)
		{
			file
				<< "{" << "\t"
				<< it2.type << "\t" << it2.varType << "\t" << it2.dimension << "\t" << "context" << "\t" << it2.ptrVal << "\t"
				<< "}" << "\t";
		}
		file << endl;
	}
	file.close();
}

void SyntacticAnalizer::Analize(LexicAnalizer & lexicAnalizer)
{
	m_lexicAnalizer = &lexicAnalizer;
	m_context = "gvar";
	if (m_lexicAnalizer->m_tokens.size() != 0)
	{
		//Leer Vars
		ReadVarDeclRecur();
		//Leer FuncProcs

		while (true)
		{
			actualString.clear();
			actualTok = m_lexicAnalizer->GetToken();
			if (m_lexicAnalizer->TokenIndex != m_lexicAnalizer->m_tokens.size() - 1 && actualTok.first != "main" && m_lexicAnalizer->m_tokens.size())
			{
				if (actualTok.first == "function")
				{
					actualString += actualTok.first;
					ReadFunction();
					m_set.push_back(actualString);
				}
				else
					if (actualTok.first == "procedure")
					{
						actualString += actualTok.first;
						ReadProcedure();
						m_set.push_back(actualString);
					}
			}else
			if (actualTok.first == "main")
			{
				if (actualTok.first == "(")
				{
					if (actualTok.first == ")")
					{
						ReadBlock();
					}
				}
				break;
			}
			else
			{
				ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba main", actualString);
				break;
			}
		}
	}
	SaveFile();
}

SyntacticAnalizer::SyntacticAnalizer()
{
}


SyntacticAnalizer::~SyntacticAnalizer()
{
}
