#include "semanticanalizer.h"

#include <iostream>
#include <fstream>
#include "ErrorModule.h"
using namespace std;

void SemanticAnalizer::ReadVarDeclRecur()
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
            m_lexicAnalizer->TokenIndex-=2;
            if (m_lexicAnalizer->TokenIndex < 0)
                m_lexicAnalizer->TokenIndex = 0;
            break;
        }
    }
}

bool SemanticAnalizer::ReadFunctionPropCall()
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
                }
            }
            else
            {
                PanicMode();
            }
        }
        else
        {
            PanicMode();
        }
        return false;
    //}
}



void SemanticAnalizer::ReadParamList()
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
        }
    }
}
void SemanticAnalizer::PanicMode()
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

bool SemanticAnalizer::ActualIDExist()
{
    for (auto &it : m_symbTable->m_hashTable)
    {
        for (auto &global : it.second)
        {
            if ( global.name == actualID )
                return true;

            for (auto &local : global.localNode)
            {

                if (local.name == actualID)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool SemanticAnalizer::IsActualIDIndexOnRange()
{
    for (auto &it : m_symbTable->m_hashTable)
    {
        for (auto &global : it.second)
        {
            if ( global.name == actualID)
                if (global.dimension > actualIndex)
                     return true;

            for (auto &local : global.localNode)
            {

                if (local.name == actualID && local.context == m_context)
                {
                    if (local.dimension > actualIndex)
                        return true;
                }
            }
        }
    }
    return false;
}

bool SemanticAnalizer::IsActualIDArray()
{
    for (auto &it : m_symbTable->m_hashTable)
    {
        for (auto &global : it.second)
        {
            if ( global.name == actualID)
                if (global.dimension > 0)
                     return true;

            for (auto &local : global.localNode)
            {

                if (local.name == actualID && local.context == m_context)
                {
                    if (local.dimension > 0)
                        return true;
                }
            }
        }
    }
    return false;
}


void SemanticAnalizer::ReadParamSet()
{

    actualTok= m_lexicAnalizer->GetToken();
    if (actualTok.second == LexicAnalizer::ETokenType::ID)
    {
        std::string id = actualTok.first;
        actualString += actualTok.first;
        actualTok= m_lexicAnalizer->GetToken();
        if (actualTok.first == ":")
        {
            actualTok= m_lexicAnalizer->GetToken();
            if (actualTok.second == LexicAnalizer::ETokenType::KEYWORD)//Type
            {
                std::string type = actualTok.first;
                actualTok= m_lexicAnalizer->GetToken();
                if (actualTok.first == ";")
                {
                }
                else
                {
                    //m_lexicAnalizer->TokenIndex = resp;
                    PanicMode();
                }
            }
            else
            {
                //m_lexicAnalizer->TokenIndex = resp;
                PanicMode();
            }
        }
        else if (actualTok.first == ",")
        {
            ReadParamSet();
        }
        else if (actualTok.first == "[")
        {
            ReadDimension();
            ReadParamSet();
        }
        else
        {
            //m_lexicAnalizer->TokenIndex = resp;
            PanicMode();
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

            }
            else
            {
                PanicMode();
            }
        }
        else
        {
            PanicMode();
        }
    }
    else
    {
        PanicMode();
    }
}

void SemanticAnalizer::ReadPassParamList()
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

void SemanticAnalizer::ReadAsignation()
{
    if (actualType != LexicAnalizer::ETokenType::ID)
    {
        ErrorModule::PushError("<SEMANTIC>",0,"Tipo "+actualID +" no asignable en "+m_context,"");
        return;
    }
    if (IsActualIDArray() == false && actualIsArray == true){
        ErrorModule::PushError("<SEMANTIC>",0,"Variable "+actualID +"  es un array en "+m_context,"");
        return;
    }
    if (IsActualIDArray() == true && actualIsArray == false){
        ErrorModule::PushError("<SEMANTIC>",0,"Variable "+actualID +" no es un array en "+m_context,"");
        return;
    }
    if (IsActualIDArray() == true && actualIsArray == true){
        if (!IsActualIDIndexOnRange())
            ErrorModule::PushError("<SEMANTIC>",0,"Variable "+actualID +" tiene un indice fuera de rango en "+m_context,"");
        return;
    }
    if (!ActualIDExist()){
        ErrorModule::PushError("<SEMANTIC>",0,"Variable "+actualID +" no declarada en "+m_context,"");
    }

    ReadExpr();
}

void SemanticAnalizer::ReadDimension()
{
    if (actualTok.first == "[")
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
        if (actualTok.second == LexicAnalizer::ETokenType::INT)
        {

            actualString += actualTok.first;
            actualIndex = atoi(actualTok.first.c_str());
            actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.first == "]")
            {
                actualString += actualTok.first;
            }
            else
            {
                PanicMode();
            }
        }
        else
        {
            PanicMode();
        }
    }
}

void SemanticAnalizer::ReadExpr()
{
    if (actualTok.second == LexicAnalizer::ETokenType::ID)
    {
        actualTok = m_lexicAnalizer->GetToken();
        if (actualTok.first == "["){
            actualIsArray = true;
            ReadDimension();
        }
        else
        {
            m_lexicAnalizer->TokenIndex-=2;
            actualTok = m_lexicAnalizer->GetToken();
        }
    }
    do {
        actualID = actualTok.first;
        actualType = actualTok.second;
        actualTok = m_lexicAnalizer->GetToken();
        if (m_lexicAnalizer->TokenIndex < m_lexicAnalizer->m_tokens.size() - 1)
        {
            if (actualTok.first == "=")
            {
                ReadAsignation();
            }
            else if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
                    || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
                    || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
                {
                    ReadOper();
                }

            else if (actualTok.second == LexicAnalizer::ETokenType::ID){
                actualIsArray = false;
                actualID = actualTok.first;
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
            else if (actualTok.second == LexicAnalizer::ETokenType::ID){
                actualIsArray = false;
                actualID = actualTok.first;
            }
        }

    } while (actualTok.second == LexicAnalizer::ETokenType::ID || actualTok.second == LexicAnalizer::ETokenType::INT || actualTok.first == "("
             || (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
             || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
             || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP));
    if (actualTok.first != ";" && actualTok.first != ")")
    {
        PanicMode();
    }else if (actualTok.first == ")")
        actualTok = m_lexicAnalizer->GetToken();
}

void SemanticAnalizer::ReadReturnExpr()
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
    }else if (actualTok.first == ")")
        actualTok = m_lexicAnalizer->GetToken();
}

void SemanticAnalizer::ReadStatement()
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
        }
    }
    else
    {
        PanicMode();
    }
}

SemanticAnalizer::SemanticAnalizer()
{

}


void SemanticAnalizer::ReadTerm()
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
        actualID = actualTok.first;
        actualIsArray = false;
        actualType = actualTok.second;
        actualTok = m_lexicAnalizer->GetToken();
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
                }
            }
        }
    }
    else
    {
        PanicMode();
    }
}

void SemanticAnalizer::ReadOper()
{
    if (actualTok.second == LexicAnalizer::ETokenType::ARITHMETIC_OP
        || actualTok.second == LexicAnalizer::ETokenType::RELATIONAL_OP
        || actualTok.second == LexicAnalizer::ETokenType::LOGIC_OP)
    {
        actualString += actualTok.first;
        actualTok = m_lexicAnalizer->GetToken();
        ReadTerm();
    }
}

void SemanticAnalizer::ReadFunction()
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

                        actualTok = m_lexicAnalizer->GetToken();
                        ReadFuncBlock();
                    }
                    else
                    {
                        PanicMode();
                    }
                }
                else
                {
                    PanicMode();
                }
            }
            else
            {
                PanicMode();
            }
        }
        else
        {
            PanicMode();
        }
    }
    else
    {
        PanicMode();
    }
}

void SemanticAnalizer::ReadProcedure()
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
                ReadBlock();
            }
            else
            {
                PanicMode();
            }
        }
        else
        {
            PanicMode();
        }
    }
    else
    {
        PanicMode();
    }
}

void SemanticAnalizer::ReadBlock()
{
    actualTok = m_lexicAnalizer->GetToken();
    if (actualTok.first == "{")
    {
        ReadVarDeclRecur();
        ReadExprBlock();
        if (actualTok.first == "}")
        {

        }
    }
    else{
            PanicMode();
    }

}
void SemanticAnalizer::ReadFuncBlock()
{
    if (actualTok.first == "{")
    {
        ReadVarDeclRecur();
        ReadExprBlock();
        if (actualTok.first == "return")
        {
            ReadReturnExpr();
            actualTok = m_lexicAnalizer->GetToken();
            if (actualTok.first != "}")
            {
                PanicMode();
            }

        }
    }
    else{
     PanicMode();
    }
}


void SemanticAnalizer::ReadExprBlock()
{
    actualTok = m_lexicAnalizer->GetToken();
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
            }else
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


void SemanticAnalizer::Analize(SymbolTable* symbTable,LexicAnalizer * lexicAnalizer)
{
    m_set.clear();
    m_lexicAnalizer = lexicAnalizer;
    m_lexicAnalizer->TokenIndex = 0;
    m_symbTable = symbTable;
    m_context = "gvar";
    actualID = "";
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
                    }
                }
                else{
                     PanicMode();
                }
                break;
            }
            else
            {
                break;
            }
        }
    }
}


