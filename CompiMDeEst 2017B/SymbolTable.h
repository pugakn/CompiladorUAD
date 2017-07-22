#pragma once
#include <string>
#include <unordered_map>
#include <memory>
class SymbolTable
{
public:
	struct LocalNode
	{
		std::string name;
		std::string type;
		std::string varType;
		size_t dimension;
		void* ptrVal;
		//LocalNode* nextNode;
		LocalNode() {};
		LocalNode(std::string name, std::string type, std::string varType, size_t dimension, void* ptrVal, LocalNode* nextNode)
		{
			this->name = name;
			this->type = type;
			this->varType = varType;
			this->dimension = dimension;
			this->ptrVal = ptrVal;
			//this->nextNode = nextNode;
		}
	};
	struct GlobalNode
	{
		std::string name;
		std::string type;
		std::string varType;
		size_t dimension;
		void* ptrVal;
		std::list<LocalNode> localNode;
		//GlobalNode* nextNode;
		GlobalNode() {};
		GlobalNode(std::string name, std::string type, std::string varType, size_t dimension, void* ptrVal, GlobalNode* nextNode)
		{
			this->name = name;
			this->type = type;
			this->varType = varType;
			this->dimension = dimension;
			this->ptrVal = ptrVal;
			//this->nextNode = nextNode;
		}
	};
	void AddGlobalNode(GlobalNode node);
	void AddLocalNode(LocalNode node);
	void Destroy();
	SymbolTable();
	~SymbolTable();
	std::unordered_map<std::string, std::vector<GlobalNode>> m_hashTable;
private:
	
};

