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


uint32 Scene::RegisterPrimitive(Primitive* newPrimitive)
{
	inst->log("RegisterPrimitive");

	primitives.push_back(newPrimitive);

	// Return primitive id (index in vector)
	return primitives.size() - 1;
}

std::vector<Primitive*>& Scene::GetPrimitiveList()
{
	return primitives;
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


void Scene::Tick(float dt)
{
	for (size_t i = 0; i < tickableNodes.size(); i++)
		tickableNodes[i]->OnTick( dt );
}
