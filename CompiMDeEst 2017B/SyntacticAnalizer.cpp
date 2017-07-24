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
	static std::vector<std::string> id;
	std::string type;
	actualTok = m_lexicAnalizer->GetToken();
	if (actualTok.second == LexicAnalizer::ETokenType::ID)
	{
		id.push_back (actualTok.first);
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == ":")
		{
			actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
			{
				for (auto &it : id)
				{
					type = actualTok.first;
					SymbolTable::LocalNode node;
					node.dimension = 0;
					node.type = "param";
					node.ptrVal = 0;
					node.name = it;
					node.varType = type;
					node.context = m_context;

					symTable.AddLocalNode(node);
				}
				id.clear();

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

void SyntacticAnalizer::PushSymbolTambleErrors()
{
	for (auto &it : symTable.m_hashTable)
	{
		for (auto &global : it.second)
		{
			for (auto &local : global.localNode)
			{
				for (auto &it2 : symTable.m_hashTable)
				{
					for (auto &global2 : it2.second)
					{
						if (&global != &global2)
						{
							if (global.name == global2.name && global.type == global2.type && global.type != "NULL")
							{
								//Error variable global con el mismo nombre
								ErrorModule::PushError("<SNTACTIC>", 0, "Error variable global con el mismo nombre", global.name);
								//it.second.erase(1);
							}
						}
						for (auto &local2 : global2.localNode)
						{
							if (&local != &local2)
							{
								if (local.name == local2.name && local.context == local2.context)
								{
									if (local.type == local2.type)
									{
										//Error variable local con el mismo nombre dentro del mismo contexto
										ErrorModule::PushError("<SYNTACTIC>", 0, "variable local con el mismo nombre dentro del mismo contexto", local.name + " en " + local.context);
										//global.localNode.remove(local);
										//global2.localNode.remove(local2);
									}
									else
									{
										//Error variable local con el mismo nombre que un parametro
										ErrorModule::PushError("<SYNTACTIC>", 0, "variable local con el mismo nombre que un parametro", local.name + " en " + local.context);
										//global.localNode.remove(local);
										//global2.localNode.remove(local2);
									}
								}
							}
						}
					}
				}
			}
		}
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
						node.varType = type;

						symTable.AddGlobalNode(node);
					}
					else
					{
						SymbolTable::LocalNode node;
						node.dimension = 0;
						node.type = "lvar";
						node.ptrVal = 0;
						node.name = id;
						node.varType = type;
						node.context = m_context;

						symTable.AddLocalNode(node);
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
	//if (actualTok.first == "=")
	//{
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
			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ; despues de la asignacion", actualString);
		}
	//}
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
			if (actualTok.first == "=")
			{
				ReadAsignation();
			}
			else if (actualTok.first == "(")
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
		m_context = actualTok.first;
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
						SymbolTable::GlobalNode node;
						node.dimension = 0;
						node.type = "function";
						node.ptrVal = 0;
						node.name = m_context;
						node.varType = "NULL";
						symTable.AddGlobalNode(node);

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
		m_context = actualTok.first;
		actualTok = m_lexicAnalizer->GetToken();
		if (actualTok.first == "(")
		{
			ReadParamList();
			//actualTok = m_lexicAnalizer->GetToken();
			if (actualTok.first == ")")
			{
				SymbolTable::GlobalNode node;
				node.dimension = 0;
				node.type = "procedure";
				node.ptrVal = 0;
				node.name = m_context;
				node.varType = "NULL";
				symTable.AddGlobalNode(node);
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
	file.open("table.synb");
	for (auto &it : symTable.m_hashTable)
	{
		for (auto &n : it.second)
		{
			file << "{" << "\t";
			file << "\t" << n.name << "\t," << n.type << "\t," << n.varType << "\t," << n.dimension << "\t," << "GLOBAL" << "\t," << n.ptrVal <<", ";
			for (auto &it2 : n.localNode)
			{
				file
					<< "{" << "\t"
					<< it2.type << "\t," << it2.varType << "\t," << it2.dimension << "\t," << it2.context << "\t," << it2.ptrVal << "\t"
					<< "}" << "\t";
			}
			file  << "\t";
		}
		file << "}" << "\t";
		

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
	PushSymbolTambleErrors();
}

SyntacticAnalizer::SyntacticAnalizer()
{
}


SyntacticAnalizer::~SyntacticAnalizer()
{
}
