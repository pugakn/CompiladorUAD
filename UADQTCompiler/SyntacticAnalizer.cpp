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
                    node.dimension = actualDim;
                    node.type = "param";
                    node.ptrVal = 0;
                    node.name = it;
                    node.varType = type;
                    node.context = m_context;

                    symTable.AddLocalNode(node);
                }
                id.clear();
                actualDim = 0;

                actualTok = m_lexicAnalizer->GetToken();
                if (actualTok.first == ",")
                {

                    ReadParamList();
                }
            }
            else
            {
                id.clear();
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
            id.clear();
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
        if (m_lexicAnalizer->TokenIndex == m_lexicAnalizer->m_tokens.size() - 2)
            break;
    }

}

void SyntacticAnalizer::PushSymbolTambleErrors()
{
    for (auto &it : symTable.m_hashTable)
    {
        for (auto &global : it.second)
        {
            for (auto &global2 : it.second)
            {
                if (&global != &global2)
                {
                    if (global.name == global2.name && global.type == global2.type && global.type != "NULL")
                    {
                        if (global.type == "gvar")
                        {
                            //Error variable global con el mismo nombre
                            ErrorModule::PushError("<SNTACTIC>", 0, "Error variable global con el mismo nombre", global.name);
                            //it.second.erase(1);
                        }
                        else
                        {
                            ErrorModule::PushError("<SNTACTIC>", 0, "Error procedimiento o funcion con el mismo nombre", global.name);
                        }
                    }
                }
                for (auto &local : global.localNode)
                {
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
                    varsV.push_back(id);
                    for (auto &ID : varsV){
                        if (m_context == "gvar")
                        {
                            SymbolTable::GlobalNode node;
                            node.dimension = actualDim;
                            node.type = "gvar";
                            node.ptrVal = 0;
                            node.name = ID;
                            node.varType = type;

                            symTable.AddGlobalNode(node);
                        }
                        else
                        {
                            SymbolTable::LocalNode node;
                            node.dimension = actualDim;
                            node.type = "lvar";
                            node.ptrVal = 0;
                            node.name = ID;
                            node.varType = type;
                            node.context = m_context;

                            symTable.AddLocalNode(node);
                        }
                    }
                  varsV.clear();
                  actualDim = 0;
                }
                else
                {
                    ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ;", actualString);
                    //m_lexicAnalizer->TokenIndex = resp;
                    PanicMode();
                    varsV.clear();
                }
            }
            else
            {
                //m_lexicAnalizer->TokenIndex = resp;
                PanicMode();
                ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo de variable", actualString);
                varsV.clear();
            }
        }
        else if (actualTok.first == ",")
        {
            actualString += actualTok.first;
            varsV.push_back(id);
            ReadParamSet();
        }
        else if (actualTok.first == "[")
        {
            ReadDimension();
            varsV.push_back(id);
            ReadParamSet();
        }
        else
        {
            //m_lexicAnalizer->TokenIndex = resp;
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba :", actualString);
            varsV.clear();
        }
    }
    else if (actualTok.first == ":")
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
                for (auto &ID : varsV){
                    if (m_context == "gvar")
                    {
                        SymbolTable::GlobalNode node;
                        node.dimension = actualDim;
                        node.type = "gvar";
                        node.ptrVal = 0;
                        node.name = ID;
                        node.varType = type;

                        symTable.AddGlobalNode(node);
                    }
                    else
                    {
                        SymbolTable::LocalNode node;
                        node.dimension = actualDim;
                        node.type = "lvar";
                        node.ptrVal = 0;
                        node.name = ID;
                        node.varType = type;
                        node.context = m_context;

                        symTable.AddLocalNode(node);
                    }
                }
              varsV.clear();
              actualDim = 0;
            }
            else
            {
                ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ;", actualString);
                //m_lexicAnalizer->TokenIndex = resp;
                PanicMode();
                varsV.clear();
            }
        }
        else
        {
            //m_lexicAnalizer->TokenIndex = resp;
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo de variable", actualString);
            varsV.clear();
        }
    }
    else
    {
        //m_lexicAnalizer->TokenIndex = resp;
        PanicMode();
        ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ID", actualString);
        varsV.clear();
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
      //actualTok = m_lexicAnalizer->GetToken();
    //if (actualTok.first == "=")
    //{
        actualString += actualTok.first;
        ReadExpr();
        //actualTok = m_lexicAnalizer->GetToken();
//		if (actualTok.first == ";")
//		{
//			actualString += actualTok.first;
//		}
//		else
//		{
//			PanicMode();
//			ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ; despues de la asignacion", actualString);
//		}
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
            actualDim =  stoi(actualTok.first);
            actualString += actualTok.first;
            actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.first == "]")
            {
                actualString += actualTok.first;
            }
            else
            {
                PanicMode();
                ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ]", actualString);
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
            else
            {
                //actualTok = m_lexicAnalizer->GetToken();
                if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
                    || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
                    || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
                {
                    ReadOper();
                }
            }
        }
        else
        {
            //actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
                || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
                || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
            {
                ReadOper();
            }
        }

    } while (actualTok.second == LexicAnalizer::ETokenType::ID || actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.first == "("
             || (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
             || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
             || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP));
    if (actualTok.first != ";" && actualTok.first != ")")
    {
        PanicMode();
        //ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ; después de la expresion", actualString);
    }else if (actualTok.first == ")")
        actualTok = m_lexicAnalizer->GetToken();
}

void SyntacticAnalizer::ReadReturnExpr()
{
    do {
        actualTok = m_lexicAnalizer->GetToken();
        if (m_lexicAnalizer->TokenIndex < m_lexicAnalizer->m_tokens.size() - 1)
        {
            if (actualTok.first == ";")
            {
               return;
            }
            //actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
                || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
                || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
            {
                ReadOper();
            }
        }
        else
        {
            ReadTerm();
            //actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
                || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
                || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
            {
                ReadOper();
            }
        }

    } while (actualTok.second == LexicAnalizer::ETokenType::ID || actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.first == "("
         || (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
         || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
         || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP));
    if (actualTok.first != ";" && actualTok.first != ")")
    {
        PanicMode();
        ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ; después de la expresion de retorno", actualString);
    }else if (actualTok.first == ")")
        actualTok = m_lexicAnalizer->GetToken();
}

void SyntacticAnalizer::ReadStatement()
{
    actualTok = m_lexicAnalizer->GetToken();
    if (actualTok.first == "(")
    {
        ReadExpr();
        m_lexicAnalizer->TokenIndex-=2;
        actualTok = m_lexicAnalizer->GetToken();
        if (actualTok.first == ")")
        {
            ReadBlock();
        }
        else
        {
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ) if/while/for", actualString);
        }
    }
    else
    {
        PanicMode();
        ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un (  if/while/for", actualString);
    }
}

void SyntacticAnalizer::ReadTerm()
{
    //actualTok = m_lexicAnalizer->GetToken();
    if (actualTok.first == "(")
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
        ReadTerm();
        if (actualTok.first == ")")
        {
            actualString += actualTok.first;
            actualTok = m_lexicAnalizer->GetToken();
        }    else
        {
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ) después del termino", actualString);
        }
    }

    if (actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.second == LexicAnalizer::ETokenType::FLOAT
            || actualTok.second == LexicAnalizer::ETokenType::BOOL || actualTok.second == LexicAnalizer::ETokenType::STRING)
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
        return;
    }
    else if (actualTok.second == LexicAnalizer::ETokenType::ID)//Dimen && func
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
//		if (actualTok.first == ")")
//		{
//			actualString += actualTok.first;
//		}
        if (actualTok.first == "(")
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
                }else{
                    PanicMode();
                     ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un ).", actualString);
                }
            }
        }
    }
    else
    {
        PanicMode();
        ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un termino", actualString);
    }
}

void SyntacticAnalizer::ReadOper()
{
    if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
        || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
        || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
        ReadTerm();
        noTrow = true;
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
            do{

                ReadParamList();
            }while(actualTok.first == ";");
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
                        ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba un tipo de retorno", actualString);
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
                ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ) en la funcion", actualString);
            }
        }
        else
        {
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba ( en la funcion", actualString);
        }
    }
    else
    {
        PanicMode();
        ErrorModule::PushError("<SYNTACTIC>", 0, "Token invalido", actualString);
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
            do{

                ReadParamList();
            }while(actualTok.first == ";");
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
        ErrorModule::PushError("<SYNTACTIC>", 0, "token invalido", actualString);
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
    else{
            PanicMode();
            ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba {", actualString);
    }

}
void SyntacticAnalizer::ReadFuncBlock()
{
    //actualTok = m_lexicAnalizer->GetToken();
    if (actualTok.first == "{")
    {
        ReadVarDeclRecur();
        ReadExprBlock();
        if (actualTok.first == "return")
        {
            //actualTok = m_lexicAnalizer->GetToken();
            ReadReturnExpr();
            actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.first != "}")
            {
                PanicMode();
                ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba } después del return", actualString);
            }

        }
    }
    else{
     PanicMode();
     ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba {", actualString);
    }
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
            if (actualTok.first == "if")
            {
                ReadStatement();
                actualTok = m_lexicAnalizer->GetToken();
                if (actualTok.first == "else")
                            ReadBlock();


            }else if (actualTok.first == "while"){
                ReadStatement();
            }
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
    symTable.Destroy();
    m_set.clear();
    index = 0;
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
                else
                        if (actualTok.second == LexicAnalizer::ETokenType::ID)
                        {
                            PanicMode();
//                            ErrorModule::PushError("<SYNTACTIC>", 0, "Token invalido", actualString);
                        }
            }else
            if (actualTok.first == "main")
            {
                actualTok = m_lexicAnalizer->GetToken();
                if (actualTok.first == "(")
                {
                    actualTok = m_lexicAnalizer->GetToken();
                    if (actualTok.first == ")")
                    {
                        ReadBlock();
                    }
                    else{
                     PanicMode();
                     ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba )", actualString);
                    }
                }
                else{
                     PanicMode();
                    ErrorModule::PushError("<SYNTACTIC>", 0, "Se esperaba (", actualString);
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
   actualDim = 0;
}


SyntacticAnalizer::~SyntacticAnalizer()
{
}
