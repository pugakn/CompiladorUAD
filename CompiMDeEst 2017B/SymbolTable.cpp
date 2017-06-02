#include "stdafx.h"
//#include "SymbolTable.h"
//
//
//void SymbolTable::AddGlobalNode(std::string name, EType type, EVarType varType, size_t dimension, void * ptrVal)
//{
//	auto node = m_hashTable.find(name);
//	if ( node == m_hashTable.end()) { //NOT FOUND
//		m_hashTable[name] = std::make_unique<GlobalNode>( name, type, varType, dimension, ptrVal ) ;
//	}
//	else {
//		node->second->nextNode = std::make_unique<GlobalNode>(name, type, varType, dimension, ptrVal);
//	}
//}
//
//void SymbolTable::AddLocalNode(std::string name, EType type, EVarType varType, size_t dimension, void * ptrVal)
//{
//	auto node = m_hashTable.find(name);
//	if (node == m_hashTable.end()) { //NOT FOUND
//
//		m_hashTable[name] = std::make_unique<GlobalNode>(0, 0, 0, 0, 0, std::make_unique<LocalNode>(name, type, varType, dimension, ptrVal));
//	}
//	else {
//		auto localNode = &node->second->localNode;
//		if (node->second->localNode == nullptr)
//		{
//			node->second->localNode = std::make_unique<LocalNode>(name, type, varType, dimension, ptrVal);;
//		}
//		else
//		{
//			while (localNode->get()->nextNode != nullptr)
//			{
//				localNode = &localNode->get()->nextNode;
//			}
//			localNode->get()->nextNode = std::make_unique<LocalNode>(name, type, varType, dimension, ptrVal);
//		}
//	}
//}
//
//void SymbolTable::Destroy()
//{
//
//}
//
//SymbolTable::SymbolTable()
//{
//}
//
//
//SymbolTable::~SymbolTable()
//{
//}
