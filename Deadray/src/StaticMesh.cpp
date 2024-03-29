#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/StaticMesh.h"

#include "Deadray/MeshNode.h"
#include "Deadray/Engine.h"

using namespace Deadray;

StaticMesh::StaticMesh(Node* parent) : SceneNode(parent)
{
	GetEngine()->log("Static mesh");

	mesh = CreateChildNode<MeshNode>();
	//mesh->SetMesh(L"data\\warlock.obj");
}

// Node type registration

const bool StaticMesh::bRegistered = Engine::RegisterNewType(1003, "StaticMesh");
NodeType StaticMesh::GetNodeType()
{
	return 1003;
}