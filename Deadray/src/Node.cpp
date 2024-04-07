#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/Scene.h"
#include "Deadray/Node.h"

using namespace Deadray;

REGISTER_NODE_TYPE(Types::Node, Node);

uint32 Node::GetNodeType()
{
	return Types::Node;
}

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

Node::Node()
{
	pEngine = nullptr;
	pParentNode = nullptr;
	bIsTickable = false;
}

Node::~Node(void)
{
	Engine::Get()->log("Node gone");
}

void Node::AttachNode(Node* childNode)
{
	childNodes.push_back(childNode);
	childNode->OnAttach(this);
}

void Node::OnAttach(Node* parent)
{
}

void Node::RegisterSceneNode(Node* node)
{
	Engine::Get()->log("Register scene node");

	if (node->bIsTickable)
		Engine::Get()->GetScene()->RegisterAsTickable(node);
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

void Node::OnGameStart()
{
}

void Node::OnGameEnd()
{
}

void Node::OnTick(float dt)
{
}