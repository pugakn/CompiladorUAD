#ifndef SEMANTICANALIZER_H
#define SEMANTICANALIZER_H
#include "SymbolTable.h"
#include "LexicAnalizer.h"
class SemanticAnalizer
{
public:
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
    SemanticAnalizer();

    void PanicMode();
    bool ActualIDExist();
    bool IsActualIDIndexOnRange();
    bool IsActualIDArray();

    SymbolTable* m_symbTable;

    std::vector<std::string> m_set;
    std::string actualString;
    std::string actualID;
    int actualIndex;
    bool actualIsArray;
    LexicAnalizer::ETokenType actualType;
    void Analize(SymbolTable* symbTable , LexicAnalizer*lexicAnalizer );

    LexicAnalizer* m_lexicAnalizer;
    std::pair<std::string, LexicAnalizer::ETokenType> actualTok;

    enum EType{
        BOOL,
        STRING,
        INT,
        FLOAT,
        PROCFUNC
    };
    enum EOperator{
        PLUS,
        MINUS,
        DIV,
        AND,
        OR,
        EQUAL,
        MORE,
        LESS,
        NOT
    };
    enum EOpType{
        RELATIONAL,
        ARITMETIC,
        LOGIC
    };
    struct nodo_hoja{
        EType type;
        EType result;
        std::string id;
        std::string constant;
    };
    struct nodo_expr{
        EOperator operatorId;
        EOpType   operatorType;
        nodo_hoja* child1;
        nodo_hoja* child2;
        EType predictedResult[2];
        EType result;
    };

};

#endif // SEMANTICANALIZER_H
