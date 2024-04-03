#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/Scene.h"
#include "Deadray/SceneNode.h"


using namespace Deadray;

Scene::Scene(Engine* engine) : Node(engine)
{
	//Super::Scene();

	inst = engine;
	inst->RegisterScene(this);
	inst->log("Scene created.");

	activeCamera = nullptr;
}

void Scene::OnDeviceStart()
{
	for (size_t i = 0; i < tickableNodes.size(); i++)
		tickableNodes[i]->OnDeviceStart();

}

void Scene::OnDeviceShutdown()
{
	for (size_t i = 0; i < tickableNodes.size(); i++)
		tickableNodes[i]->OnDeviceShutdown();
}

void Scene::Destroy()
{
}


/*void Scene::SpawnSceneNode( SceneNode* actor )
{
	// TODO: Sub nodes should
	if (actor->bIsTickable)
	{
		RegisterAsTickable((Node*)actor);
	}

	sceneSceneNodes.push_back(actor);

	inst->log("SpawnSceneNode");
}*/


uint32 Scene::RegisterMeshNode(MeshNode* newMesh)
{
	inst->log("RegisterMeshNode");

	meshNodes.push_back(newMesh);

	// Return primitive id (index in vector)
	return meshNodes.size() - 1;
}

std::vector<MeshNode*>& Scene::GetMeshNodeList()
{
	return meshNodes;
}


struct SceneUpdateEvent {
	uint32 foo;
};

uint32 Scene::RegisterAsTickable(Node* node)
{
	// TODO: check if registered already?
	tickableNodes.push_back(node);

	// Return id (index in vector)
	uint32 id = tickableNodes.size() - 1;

	// Is this the best place for this?
	//SceneUpdateHandler(this);
	//(*sceneUpdateHandler)(this);

	EvtSceneUpdate.newNode = node;
	EvtSceneUpdate.Broadcast();

	return id;
}

Node* Scene::CreateNodeByType(uint32 type, Node* parent)
{
	if (parent==nullptr)
		parent = this;

	Node* n = TypeManager.CreateNewObjectByType(type, parent);

	if (n->bIsTickable)
		RegisterAsTickable(n);

	return n;
}


void Scene::Tick(float dt)
{
	for (size_t i = 0; i < tickableNodes.size(); i++)
		tickableNodes[i]->OnTick( dt );
}
