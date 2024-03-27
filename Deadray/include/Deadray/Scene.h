#ifndef   DEADRAY_SCENE_H
#define   DEADRAY_SCENE_H

#include "Deadray/Core.h"
#include <vector>

namespace Deadray {
	class SceneNode;
	class Camera;
	class Primitive;
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
		std::vector<Primitive*> primitives;
		std::vector<Node*> tickableNodes;

		Camera* activeCamera;
		Engine* inst;

	public:
		Scene(Engine* engine);
		//~Scene();

		void OnDeviceStart();
		void OnDeviceShutdown();

		void Destroy();

		void Read(Scene* buffer);
		void Write(Scene* buffer);


		virtual NodeType GetNodeType() override
		{
			return Types::Scene;
		}

	public:

		// For editor
		EventSceneUpdate EvtSceneUpdate;


		/*void SpawnSceneNode( SceneNode* actor );
		inline uint32 GetSceneNodeCount() { return sceneSceneNodes.size(); };

		template <class T>
		T* SpawnSceneNode(void* extra = nullptr)
		{
			T* ptr = new T( this, extra );

			SpawnSceneNode((SceneNode*)ptr);

			return ptr;
		}*/


		std::vector<Primitive*>& GetPrimitiveList();
		uint32 RegisterPrimitive(Primitive* newPrimitive);

		// TODO: Find a way to make subcomponents (don't even exist) tick.
		// TODO: Rename to AddToScene
		uint32 RegisterAsTickable(Node* node);

		void Tick(float dt);
	};

}

#endif