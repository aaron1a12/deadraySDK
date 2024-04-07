#ifndef   DEADRAY_SCENE_H
#define   DEADRAY_SCENE_H

#include "Deadray/Core.h"
#include <vector>

namespace Deadray {
	class SceneNode;
	class Camera;
	class MeshNode;
	class Event;

	#define	EVENT_SCENEUPDATE	1

	class EventSceneUpdate : public Event {
	public:
		Node* newNode;

		EventSceneUpdate() {
			Type = EVENT_SCENEUPDATE;
			newNode = nullptr;
		}
	};

	class DEADRAY_API Scene : public Node {

	private:

		std::vector<SceneNode*> sceneSceneNodes;
		std::vector<MeshNode*> meshNodes;
		std::vector<Node*> tickableNodes;

		Camera* activeCamera;
		Engine* inst;

	public:
		Scene();
		~Scene();

		void OnDeviceStart();
		void OnDeviceShutdown();

		void Destroy();

		void Read(Scene* buffer);
		void Write(Scene* buffer);


		virtual uint32 GetNodeType() override
		{
			return Types::Scene;
		}

	public:

		// For editor
		EventSceneUpdate EvtSceneUpdate;

		std::vector<MeshNode*>& GetMeshNodeList();
		uint32 RegisterMeshNode(MeshNode* newMesh);

		// TODO: Find a way to make subcomponents (don't even exist) tick.
		// TODO: Rename to AddToScene
		uint32 RegisterAsTickable(Node* node);

		// Creates a node, given its id. Alternative to template version: CreateChildNode<T>()
		Node* CreateNodeByType(uint32 type);

		void OnGameStart();
		void OnGameEnd();
		void Tick(float dt);
	};

}

#endif