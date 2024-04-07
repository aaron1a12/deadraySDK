#ifndef   DEADRAY_NODE_H
#define   DEADRAY_NODE_H

#include "Deadray/Core.h"
#include "Deadray/Types.h"
#include <vector>

namespace Deadray {

	class Engine;
	class Scene;
	class Event;

	class DEADRAY_API Node {

	private:
		
		Engine* pEngine;
		Node* pParentNode;
		std::vector<Node*> childNodes;
		std::vector<uint16> typeFamily;	

	public:
		//typedef Node Super;
		Node(Engine* engine, void* params = nullptr);
		Node(Node* parent, void* params = nullptr);
		Node();
		~Node();

		// Used for dynamic creation from the factory
		template <typename T>
		static void* CreateObject() {
			return new T();
		}

		// Must be overriden on every child class
		virtual uint32 GetNodeType();

		virtual void OnDeviceStart();
		virtual void OnDeviceShutdown();

		virtual void Destroy();

		// Attaches an existing node to this one
		void AttachNode(Node* childNode);

		// Event when being attached. Good time to update matrix?
		virtual void OnAttach(Node* parent);

		// Associates this node with the scene.
		void RegisterSceneNode(Node* node);

		//T* _CreateChildNode (void* params = nullptr)  // ... = flexible constructor possible!?

		template <class T>
		T* CreateChildNode ()  // ... = flexible constructor possible!?
		{
			T* ptr = new T();

			AttachNode((Node*)ptr);
			RegisterSceneNode((Node*)ptr);

			return ptr;
		}

		
		bool bIsTickable;

		void AddEventListener(uint16 evtType, Node* listener);
		virtual void OnEvent(const Event& evt);

	public:
		virtual void OnGameStart();
		virtual void OnGameEnd();
		virtual void OnTick(float dt);

	};

	//REGISTER_NODE_TYPE(Types::Node, Node);
}


#endif