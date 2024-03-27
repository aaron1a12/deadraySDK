#include "stdafx.h"
#include "Deadray/Core.h"
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