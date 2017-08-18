#include "SymbolTable.h"


void SymbolTable::AddGlobalNode(GlobalNode node)
{
	auto noden = m_hashTable.find(node.name);
	m_hashTable[node.name].push_back( node) ;

}

void SymbolTable::AddLocalNode(LocalNode node)
{
	auto nodex = m_hashTable.find(node.name);
	if (nodex == m_hashTable.end()) { //NOT FOUND

		m_hashTable[node.name].push_back(GlobalNode(node.name, "NULL", "", 0, 0, 0));
		m_hashTable[node.name].back().localNode.push_back(node);
	}
	else {
		//auto localNode = &node->second->localNode;
		//if (node->second->localNode == nullptr)
		//{
			nodex->second.front().localNode.push_back(LocalNode(node));
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
    m_hashTable.clear();
}

SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}
