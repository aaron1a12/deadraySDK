#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Event.h"
#include "Deadray/Node.h"

#include <deque>

using namespace Deadray;

Event::Event()
{
	Type = 0;//;EventTypes::Generic;
}

Event::~Event()
{
}

void Event::AddListener(Node* node)
{
	listeners.push_back(node);
}

void Event::Broadcast()
{
	for (uint32 i = 0; i < listeners.size(); i++)
		listeners[i]->OnEvent(*this);
}