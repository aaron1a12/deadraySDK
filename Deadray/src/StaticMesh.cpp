#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/StaticMesh.h"

#include "Deadray/MeshNode.h"
#include "Deadray/Engine.h"

using namespace Deadray;

REGISTER_NODE_TYPE(Types::StaticMesh, StaticMesh);

StaticMesh::StaticMesh(Node* parent) : SceneNode(parent)
{
	GetEngine()->log("Static mesh");

	mesh = CreateChildNode<MeshNode>();
	mesh->SetMesh(L"data\\warlock.obj");
}