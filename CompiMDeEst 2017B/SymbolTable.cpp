#include "stdafx.h"
#include "SymbolTable.h"


void SymbolTable::AddGlobalNode(GlobalNode node)
{
	auto noden = m_hashTable.find(node.name);
	if ( noden == m_hashTable.end()) { //NOT FOUND
		m_hashTable[node.name] = node ;
	}
	else {
		//node->second->nextNode = GlobalNode(name, type, varType, dimension, ptrVal,0);
	}
}

void SymbolTable::AddLocalNode(std::string name, std::string type, std::string varType, size_t dimension, void * ptrVal)
{
	auto node = m_hashTable.find(name);
	if (node == m_hashTable.end()) { //NOT FOUND

		m_hashTable[name] = GlobalNode("", "", "", 0, 0, 0);
		m_hashTable[name].localNode.push_back(LocalNode(name, type, varType, dimension, ptrVal,0));
	}
	else {
		//auto localNode = &node->second->localNode;
		//if (node->second->localNode == nullptr)
		//{
			node->second.localNode.push_back(LocalNode(name, type, varType, dimension, ptrVal, 0));
	//	}
		//else
		//{
	/*		while (localNode->get()->nextNode != nullptr)
			{
				localNode = &localNode->get()->nextNode;
			}
			localNode->get()->nextNode = std::make_unique<LocalNode>(name, type, varType, dimension, ptrVal);*/
		//}
	}
}

void SymbolTable::Destroy()
{

}

SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}
