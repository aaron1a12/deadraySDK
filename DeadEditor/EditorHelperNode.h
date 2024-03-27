#pragma once

#include "stdafx.h"

#include "Deadray.h"
using namespace Deadray;

#include "DeadEditor.h"

/*
	EditorHelperNode is a Deadray node designed to receive and
	process events for the editor.
*/

class EditorHelperNode : public Node {
public:
	EditorHelperNode(Engine* engine) : Node(engine) 
	{
	}
	~EditorHelperNode();

	void EditorHelperNode::OnEvent(const Event& evt) override
	{
		switch(evt.Type)
		{
			case EVENT_SCENEUPDATE:
			{
				editorApp.OnSceneUpdate(static_cast<const EventSceneUpdate&>(evt));
			}
			break;
		}
	}
};