#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/MeshNode.h"

using namespace Deadray;

MeshNode::MeshNode(Node* parent, void* params) : Node(parent)
{
	if(params)
	{
		MeshNodeParams* myParams = (MeshNodeParams*)params;
	}
}

MeshNode::~MeshNode()
{
}

// Node type registration

const bool MeshNode::bRegistered = Engine::RegisterNewType(1002, "MeshNode");
NodeType MeshNode::GetNodeType()
{
	return 1002;
}