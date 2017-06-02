//#pragma once
//#include <string>
//#include <unordered_map>
//#include <memory>
//class SymbolTable
//{
//public:
//	enum EType
//	{
//		VAR,
//		PROCEDURE,
//		FUNTION,
//		INDEF
//	};
//	enum EVarType
//	{
//		INT,
//		FLOAT,
//		STRING,
//		BOOL
//	};
//	struct LocalNode
//	{
//		std::string name;
//		EType type;
//		EVarType varType;
//		size_t dimension;
//		void* ptrVal;
//		std::unique_ptr<LocalNode> nextNode;
//	};
//	struct GlobalNode
//	{
//		std::string name;
//		EType type;
//		EVarType varType;
//		size_t dimension;
//		void* ptrVal;
//		std::unique_ptr<LocalNode> localNode;
//		std::unique_ptr<GlobalNode> nextNode;
//	};
//	void AddGlobalNode(std::string name, EType type, EVarType varType, size_t dimension,void* ptrVal);
//	void AddLocalNode(std::string name, EType type, EVarType varType, size_t dimension, void* ptrVal);
//	void Destroy();
//	SymbolTable();
//	~SymbolTable();
//private:
//	std::unordered_map<std::string, std::unique_ptr<GlobalNode>> m_hashTable;
//};

