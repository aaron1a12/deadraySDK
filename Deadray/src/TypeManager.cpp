#include "stdafx.h"
#include "Deadray/Core.h"

using namespace Deadray;

TypeManagerClass::TypeManagerClass()
{
}

TypeManagerClass::~TypeManagerClass()
{
}

const bool TypeManagerClass::RegisterNewType(NodeClassInfo nodeClass)
{
	GetRegistry().Insert(nodeClass.Id, nodeClass);

	return true;
}

const char* TypeManagerClass::GetNodeTypeName(uint32 type)
{
	NodeClassInfo* nodeInfo = GetRegistry().Find(type);

	if (nodeInfo)
	{
		return nodeInfo->Name;
	}
	else
	{
		return "Unknown node";
	}
}

Node* TypeManagerClass::CreateNewObjectByType(uint32 type, Node* parent)
{
	NodeClassInfo* nodeInfo = GetRegistry().Find(type);

	if (nodeInfo)
	{
		return (Node*)nodeInfo->Creator(parent);
	}
	else
	{
		MessageBox(NULL, L"Class not found", L"Error", MB_ICONERROR);
		return new Node(parent);
	}
}

TypeManagerClass TypeManager;