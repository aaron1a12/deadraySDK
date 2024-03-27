#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/Scene.h"
#include "Deadray/Node.h"

using namespace Deadray;

//bool result = Engine::RegisterNewType("Node");

/*

REGISTER_NODE_TYPE( TYPE_STATICMESHNODE, "StaticMesh")

*/

Node::Node(Engine* engine, void* extra)
{
	pEngine = engine;
	pParentNode = nullptr;
	bIsTickable = false;

	//if(ShouldRegisterAsTickable())
	//	RegisterAsTickable();

	//engine->log("A root node was created.");
}

Node::Node(Node* parent, void* extra)
{
	pEngine = nullptr;
	pParentNode = parent;
	bIsTickable = false;

	//if(ShouldRegisterAsTickable())
	//	RegisterAsTickable();

	//GetEngine()->log("A sub node was created.");
}

Node::~Node(void)
{
	GetEngine()->log("Node gone");
}
 
Engine* Node::GetEngine()
{
	return (pParentNode != nullptr) ? pParentNode->GetEngine() : pEngine;
}

Scene* Node::GetScene()
{
	// Root nodes are assumed to be always a scene node.
	return (pParentNode == nullptr) ? (Scene*)this : pParentNode->GetScene();
}

void Node::AttachNode(Node* childNode)
{
	childNodes.push_back(childNode);
}

void Node::RegisterSceneNode(Node* node)
{
	GetEngine()->log("Register scene node");
	GetScene()->RegisterAsTickable(node);
}

void Node::OnDeviceStart()
{
}

void Node::OnDeviceShutdown()
{
}

void Node::Destroy()
{
}

void Node::OnEvent(const Event& evt)
{
	// Example implementation:

    // if (evt.EventType == ON_GAME_START)
    // {
    //     const GameStartEvent& gameStart = static_cast<const GameStartEvent&>(evt);
    // }
}

void Node::OnTick(float dt)
{
}